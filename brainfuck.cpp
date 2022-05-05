#include <iostream>
#include <chrono>
using namespace std;

struct BFprogram {
  char* program;
  int size;
};

struct BFinfo {
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
  info.shifted = 0; info.traversed = -1;
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
  return info;
}

BFinfo GetBFinfo(BFprogram program) {
  BFinfo info; 
  info.shift = new int[program.size];
  info.shift_index = new int[program.size];

  int i = 0;
  while (i < program.size) {
    if (program.program[i] == '<' || program.program[i] == '>' || program.program[i] == '+' || program.program[i] == '-') {
    BFreturn return_info = index_helper(program, i);    
    info.shift[i] = return_info.shifted;
    info.shift_index[i] = return_info.traversed + i;
    // printf(" %i %i %i %c add|", info.shift[i], info.shift_index[i], i, program.program[i]);
    i += return_info.traversed;
    }
    ++i;
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
  #define DEBUG 1
  #if DEBUG == 1
  #define INFO BFinfo info = GetBFinfo(program)
  #else
  #define INFO
  #endif

  INFO;

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
  // long long int total_count = 0;

  #if DEBUG == 1
  #define ADD(x) tape[tape_pointer] += info.shift[program_pointer]; \
  program_pointer = info.shift_index[program_pointer]
  #else 
  #define ADD(x) tape[tape_pointer] += x
  #endif

  #if DEBUG == 1
  #define TAPE(x) tape_pointer += info.shift[program_pointer]; \
  program_pointer = info.shift_index[program_pointer]
  #else
  #define TAPE(x) tape_pointer += x
  #endif
  
  while (program_pointer < program.size) {

        
    switch (program.program[program_pointer]) {
      case '[':
        if (tape[tape_pointer] == 0) {program_pointer = brackets[program_pointer];};
        break;

      case ']':
        if (tape[tape_pointer] != 0) {program_pointer = brackets[program_pointer];};
        break;

      case '+':
        ADD(1);
        break;

      case '-':
        ADD(-1);
        break;

      case '<':
        TAPE(-1);
        break;

      case '>':
        TAPE(1);
        break;
      
      case '.':
        printf("%c", tape[tape_pointer] % 256);
        break;
        };
    // ++total_count;

    ++program_pointer;

    }

  // printf("\n%u total count \n", total_count);
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
  printf("\n%u duration\n", duration);
  return 0;
}