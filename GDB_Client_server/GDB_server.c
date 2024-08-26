#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h> // For wait()
#include <string.h>

int main() {
	int fd;
	char* fifo = "gdb_server";
	mkfifo(fifo, 0666);
	
	fd = open(fifo, O_RDONLY);
	//int p_file = open("addNums.cpp", O_RDONLY | O_WRONLY);
	 FILE *server_p_file = fopen("p.cpp", "w");
    	if (server_p_file == NULL) {
		perror("fopen");
		close(fd);
		exit(EXIT_FAILURE);
    	}
	
	int n = 0;
	char buffer[1024];
    	while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
    		buffer[n] = '\0';
        	fprintf(server_p_file, "%s", buffer);
    	}

    	if (n < 0) {
        	perror("read");
    	}
    	
    	fclose(server_p_file);
    	close(fd);
    	//// compiling p.cpp

    	int c = 0;
	int fdin, fdout;
    	int compiled = system("g++ p.cpp");
	
	if (compiled > 0) {
		printf("Command execution has failed.\n");
		return 1;
	}
	
	fdin = open("input.txt", O_RDONLY);
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
		wait(NULL);
		// compare out.txt with the testcases
		printf("Sucessful operation.\n");
		// now we need to match the values

		int status = system("diff out.txt outtest.txt");
		if (status == -1) {
			printf("Error\n");
			return 1;
		}
		
		fd = open(fifo, O_WRONLY);
		
		int exit_status = WEXITSTATUS(status);
		if (exit_status == 0) {
			char* t_msg = "All testcases passed.";
			write(fd, t_msg, strlen(t_msg) + 1);
		}
		else if (exit_status == 1) {
			char* t_msg = "All testcases did not passed.";
			write(fd, t_msg, strlen(t_msg) + 1);
		}
		else printf("Some error!\n");
		
		close(fd);
		unlink(fifo);
		// we can rm out.txt the file after we are done. It is taking up storage even after use
		
		close(fdin);
		close(fdout);
		return 0;
	} else {
		dup2(fdin, 0);
		dup2(fdout, 1);
		char* arg_Ptr[] = {"a", NULL}; 
		execv("./a.out", arg_Ptr); 
	}
	
	/////

    	// Close file descriptors

}
