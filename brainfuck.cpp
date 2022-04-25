#include <iostream>
#include <chrono>
using namespace std;

struct BFprogram {
  char* program;
  int size;
};

struct BFinfo {
  int* add;
  int* add_index;
  int* shift;
  int* shift_index;
};

struct BFreturn {
  int shifted;
  int traversed;
};

int file_size(FILE* file) {
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  fseek(file, 0, SEEK_SET);
  int size_begin = ftell(file);
  return size - size_begin;
}

BFprogram GetBFProgram() {
  FILE* file = fopen("program.b", "r");
  int amount = (int) file_size(file);
  void *program_p = new char[amount]; 
  int num_read = fread(program_p, sizeof(char), amount, file);
  BFprogram BFstruct;
  BFstruct.program = (char*) program_p;
  BFstruct.size = num_read;
  return BFstruct;
}

BFreturn index_helper(BFprogram program, int index) {
  BFreturn info;
  info.shifted = 0; info.traversed = 0;
  if (program.program[index] == '<' || program.program[index] == '>') {
    while (program.program[index] == '<' || program.program[index] == '>') {
      program.program[index] == '<' ? info.shifted -= 1 : info.shifted += 1; 
      ++info.traversed; ++index;
    }
    return info;
  }
  if (program.program[index] == '-' || program.program[index] == '+') {
    while (program.program[index] == '-' || program.program[index] == '+') {
      program.program[index] == '-' ? info.shifted -= 1 : info.shifted += 1; 
      ++info.traversed; ++index;
    }
    return info;
  }
}

BFinfo GetBFinfo(BFprogram program) {
  BFinfo info; 
  int i = 0;
  char last_instruction = 0; 
  while (i < program.size) {
    BFreturn return_info = index_helper(program, i);    
    if (program.program[i] == '<' || program.program[i] == '>') {
      info.shift[i] = return_info.shifted;
      info.shift_index[i] = return_info.traversed + i;
    }
    if (program.program[i] == '+' || program.program[i] == '-') {
      info.shift[i] = return_info.shifted;
      info.shift_index[i] = return_info.traversed + i;
    }
    ++i;
    printf("%u", return_info);
  }

  return info;
}

int match_bracket(BFprogram program, int index, int* brackets) {
  int i = index;
  int counter = 0;
  while (i < program.size) {
    if (program.program[i] == '[') {
      counter += 1;
    }
    else if (program.program[i] == ']') {
      counter -= 1;
    }
    if (counter == 0) {
      brackets[i] = index;
      brackets[index] = i;
      return i;
    }
    ++i;
  }    
  return -5;
}

int main () {
  auto start = chrono::high_resolution_clock::now();
  BFprogram program = GetBFProgram();
  // GetBFinfo(program);

  int* brackets = new int[program.size]; 
  int i = 0;
  while (i < program.size) {
    if (program.program[i] == '[') {
      brackets[i] = match_bracket(program, i, brackets);
    }
    else if (program.program[i] == ']') {} // case covered in match_brackets
    else {brackets[i] = 0;} 
    // else if (program.program[i] == ']') {
    //   brackets[i] = match_closing_bracket(program, i);
    // }
    // printf("%u %u %c|", brackets[i], i, program.program[i]);
    ++i;
  }

  int tape_pointer = 0;
  int tape[30000];
  int program_pointer = 0;
  long long int total_count = 0;
  while (program_pointer < program.size) {
    if (program.program[program_pointer] == '+') {
      tape[tape_pointer] += 1;
    }
    else if (program.program[program_pointer] == '-') {
      tape[tape_pointer] -= 1;
    }
    else if (program.program[program_pointer] == '<' && tape_pointer > 0) {
      tape_pointer -= 1;
    }
    else if (program.program[program_pointer] == '>') {
      tape_pointer += 1;
    }
    else if (program.program[program_pointer] == '[' && tape[tape_pointer] == 0) {
      program_pointer = brackets[program_pointer];
    }
    else if (program.program[program_pointer] == ']' && tape[tape_pointer] != 0) {
      program_pointer = brackets[program_pointer];
    }
    else if (program.program[program_pointer] == '.') {
      printf("%c", tape[tape_pointer] % 256);
    }
    // printf("%c ", program.program[program_pointer]);
    ++program_pointer;
    ++total_count;
  }

  printf("\n%u total count \n", total_count);
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
  printf("\n%u duration\n", duration);
  return 0;
}