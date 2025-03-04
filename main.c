#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/fanotify.h>
#include "main.h"  // Include your header file with function declarations

int main() {
    // Initialize fanotify
    int fanotify_fd = fanotify_init(FAN_CLOEXEC | FAN_CLASS_CONTENT | FAN_NONBLOCK, O_RDONLY);
    if (fanotify_fd == -1) {
        perror("Failed to initialize fanotify");
        exit(EXIT_FAILURE);
    }

    // Directory or file to monitor
    const char *restricted_directory = "/path/to/django/uploads/";
    const char *sensitive_file = "/path/to/django/uploads/sensitive_file.txt";

    // Add fanotify marks for the directory and file
    if (fanotify_mark(fanotify_fd, FAN_MARK_ADD, FAN_ACCESS | FAN_MODIFY | FAN_OPEN | FAN_CLOSE, AT_FDCWD, restricted_directory) == -1) {
        perror("Failed to add fanotify mark for directory");
        close(fanotify_fd);
        exit(EXIT_FAILURE);
    }

    if (fanotify_mark(fanotify_fd, FAN_MARK_ADD, FAN_ACCESS | FAN_MODIFY | FAN_OPEN | FAN_CLOSE, AT_FDCWD, sensitive_file) == -1) {
        perror("Failed to add fanotify mark for file");
        close(fanotify_fd);
        exit(EXIT_FAILURE);
    }

    printf("Monitoring directory: %s\n", restricted_directory);
    printf("Monitoring file: %s\n", sensitive_file);

    // Call the monitoring functions
    block_unauthorized_access(fanotify_fd, restricted_directory);
    log_file_changes_and_alert(fanotify_fd, sensitive_file);

    // Cleanup
    close(fanotify_fd);
    return 0;
}
