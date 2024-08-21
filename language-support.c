#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "language-support.h"
/// C/CPP Programs
void c_cpp_compiler(char* filename) {
      char command[100] = "g++ ";
      char* fullCmd = strcat(command, filename);
      int compiled = system(fullCmd);
	
      if (compiled > 0) {
	  printf("Command execution has failed.\n");
	  exit(1);
	  return;
      }
}

void c_cpp_executer() {
      char* arg_Ptr[] = {"a", NULL}; 
      execv("./a.out", arg_Ptr);
      return;
}

// add functionality for Java
void java_compiler(char* filename) {
      int compiled = system("javac Solution.java");
      
      if (compiled > 0) {
          printf("Command execution has failed.\n");
          exit(1);
          return;
      }
}

void java_executer() {
    int executed = system("java Solution");
    if (executed > 0) {
      printf("Command execution has failed.\n");
      exit(1);
      return;
    }
}
