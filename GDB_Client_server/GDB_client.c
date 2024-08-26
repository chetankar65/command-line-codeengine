#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h> // For wait()
#include <string.h>

int main() {
	int fd;
	char* fifo = "gdb_server";
	//mkfifo(fifo, 0666);
	
	fd = open(fifo, O_WRONLY);
	int p_file = open("addNums.cpp", O_RDONLY | O_WRONLY);
	
	if (p_file < 0) {
		perror("Error opening file");
		exit(1);
	}
	
	int n;
	char buffer[1024];
	
	FILE *file = fopen("addNums.cpp", "r");
    	if (!file) {
        	perror("Failed to open P.cpp");
        	close(fd);
        	return 1;
    	}

    	char line[1024];
    	
    	while (fgets(line, sizeof(line), file)) {
        	write(fd, line, strlen(line));
    	}
    	
    	close(fd);
    	fd = open(fifo, O_RDONLY);
    	while ((n = read(fd, buffer, 1014)) > 0) {
    		printf("%s\n", buffer);
    	}
    	
    	close(fd);
    	unlink(fifo);

    // Close FIFO
    	fclose(file);
}
