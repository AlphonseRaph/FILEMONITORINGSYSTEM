#include <stdio.h>
#include <dirent.h>

int list_files(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;

    // Open the directory
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Failed to open directory");
        return -1;
    }

    // Read the directory entries
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);  // Print each file name
    }

    // Close the directory
    closedir(dir);
    return 0;
}

int main() {
    // List files in the current directory
    list_files(".");
    return 0;
}

