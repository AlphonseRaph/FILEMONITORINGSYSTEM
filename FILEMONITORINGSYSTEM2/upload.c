#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int upload(const char *path_name,const char *content_of_file)
{
	int fd;//file descriptor

	fd = open(path_name, O_WRONLY|O_CREAT, 0644);//opens a file or creates it if it doesn't exists in a write only mode.0644 - gives file owner permission to read and write and group and world permission to read only
	if (fd == -1)
	{
		fprintf(stderr, "File failed to open\n");
		exit(1);// prints error and then exits
	}

	if (write(fd, content_of_file, strlen(content_of_file)) == -1)// writes content of file to file opened
	{
		fprintf(stderr, "Failed to write to file\n");
		close(fd);
		exit(1);//prints error, closes file and then exists 
	}
	close(fd);
	return 0;
}


int main() {
    const char *file_path = "test_file.txt";
    const char *file_content = "This is a test content for the file.";

    // Call the upload function to write the content to the file
    if (upload(file_path, file_content) == 0) {
        printf("File uploaded successfully to: %s\n", file_path);
    } else {
        printf("File upload failed.\n");
    }

    return 0;
}
		
