#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "language-support.h"

int main(int argc, char* argv[]) {
	if (argc != 5) {
		printf("Invalid number of arguments\n");
		return 1;
	}
	
	int c = 0;
	int fdin, fdout;
	char* language = argv[1];
	char* filename = argv[2];
	char* inputFile = argv[3];
	char* testFile = argv[4];
        
        if (strcmp(language, "c") == 0) {
	    c_cpp_compiler(filename);
	} else if (strcmp(language, "j") == 0) {
	    java_compiler(filename);
	} else {
	    printf("Language argument incorrect.");
	    exit(1);
	}
	
	fdin = open(inputFile, O_RDONLY);
	if (fdin < 0) {
		printf("Some error opening input file!\n");
		return 1;
	}
	
	fdout = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC);
	if (fdout < 0) {
		printf("Some error opening output file! \n");
		return 1;
	}
	
	c = fork();
	if (c > 0) {
		// wait
		wait();
		// compare out.txt with the testcases
		printf("Sucessful operation.\n");
		// now we need to match the values
                char cmd[100] = "diff out.txt ";
	        char* fullCmd = strcat(cmd, testFile);
		int status = system(fullCmd);
		if (status == -1) {
			printf("Error\n");
			return 1;
		}
		
		
		int exit_status = WEXITSTATUS(status);
		if (exit_status == 0) printf("ALL TESTCASES PASSED!\n");
		else if (exit_status == 1) printf("All testcases did not pass.\n");
		else printf("Some error!\n");
		
		// we can rm out.txt the file after we are done. It is taking up storage even after use
		
		close(fdin);
		close(fdout);
		return 0;
	} else {
		dup2(fdin, 0);
		dup2(fdout, 1);
		
		if (strcmp(language, "c") == 0) {
	            c_cpp_executer();
	        } else if (strcmp(language, "j") == 0) {
	            java_executer();
	        } else {
	            printf("Language argument incorrect.");
	            exit(1);
	        }
	}
}
