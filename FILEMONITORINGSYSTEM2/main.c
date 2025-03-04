#include "main.h"

int main(int argc, char *argv[]) {
    int fanotify_fd;

    // Initialize fanotify for access control
    fanotify_fd = fanotify_init(FAN_CLOEXEC | FAN_CLASS_CONTENT | FAN_NONBLOCK, O_RDONLY | O_CLOEXEC);
    if (fanotify_fd < 0) {
        perror("fanotify_init");
        exit(EXIT_FAILURE);
    }

    // Monitor for directory access events
    if (fanotify_mark(fanotify_fd, FAN_MARK_ADD | FAN_MARK_MOUNT, FAN_OPEN_PERM | FAN_EVENT_ON_CHILD, AT_FDCWD, "/home/hp/restricted_directory") < 0) {
        perror("fanotify_mark for restricted directory");
        close(fanotify_fd);
        exit(EXIT_FAILURE);
    }

    // Monitor for file modification events
    if (fanotify_mark(fanotify_fd, FAN_MARK_ADD | FAN_MARK_MOUNT, FAN_MODIFY | FAN_EVENT_ON_CHILD, AT_FDCWD, "/home/hp/sensitive_file") < 0) {
        perror("fanotify_mark for sensitive file");
        close(fanotify_fd);
        exit(EXIT_FAILURE);
    }

    // Fork to handle both functions
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: Handle unauthorized access
        block_unauthorized_access(fanotify_fd, "/home/hp/restricted_directory");
    } else if (pid > 0) {
        // Parent process: Handle logging and alerts
        log_file_changes_and_alert(fanotify_fd, "/home/hp/sensitive_file");
    } else {
        perror("fork");
        close(fanotify_fd);
        exit(EXIT_FAILURE);
    }

    // Close fanotify file descriptor
    close(fanotify_fd);

    return 0;
}

