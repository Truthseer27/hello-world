/*
 * Uses simple versions of ls and cat
 * to list directory contents and
 * then print contents of .c files
 */

#include <sys/types.h>
#include <fcntl.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 32768

int
main(int argc, char **argv) {

	DIR *dp;
	struct dirent *dirp;
	int n;
	char buf[BUFFSIZE];
	char filename[80]; 
	int  name_length;

	if (argc != 2) {
		fprintf(stderr, "usage: %s dir_name\n", argv[0]);
		exit(1);
	}

	if ((dp = opendir(argv[1])) == NULL ) {
		fprintf(stderr, "can't open '%s'\n", argv[1]);
		exit(1);
	}


	// display directory entries like simple-ls
	while ((dirp = readdir(dp)) != NULL ){
        	printf("%s\n", dirp->d_name);
        	memset(filename, '\0', sizeof(filename));  
        	strcpy(filename, dirp->d_name);
       		printf(" ** %s ", filename);
        	name_length = strlen(filename);
        	printf(" name_length=%d \n", name_length);

		// capture the last two letters of the filename
		const char *dot_c = &filename[name_length-2];
		//printf("%s\n", dot_c);
	
		// if the last two characters are .c, read file and write contents
		// in same way that simple-cat does
		if (strcmp(dot_c, ".c") == 0){
			close(STDIN_FILENO);
			open(filename, O_RDONLY);
			while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
				if (write(STDOUT_FILENO, buf, n) != n) {
					fprintf(stderr, "write error\n");
					exit(1);
				}
			}
		}

		if (n < 0) {
			fprintf(stderr, "read error\n");
			exit(1);
		}
	}

	closedir(dp);
	return(0);
}