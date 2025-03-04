#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
int file_download(const char *pathname)
{
	int fd;
	char buffer[BUFFER_SIZE];
	
	ssize_t bytes_read;

	fd = open(pathname, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "File failed to open");
		exit(1);
	}
	
	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		if (write(STDOUT_FILENO, buffer, bytes_read) == -1)
		{
			fprintf(stderr, "Writ failed\n");
			close(fd);
			exit(1);
		}
	}
	close(fd);

	return 0;
}

int main(int argc, char *argv[])
{
    // Check if the user provided the correct number of arguments
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    // Call the file_download function with the provided file path
    if (file_download(argv[1]) != 0)
    {
        fprintf(stderr, "Failed to download file.\n");
        return 1;
    }

    printf("File downloaded successfully.\n");
    return 0;
}
