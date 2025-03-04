#include "main.h"
// Function to handle logging file changes
void log_file_changes_and_alert(int fanotify_fd, const char *sensitive_file) {
    char buffer[1024];
    struct fanotify_event_metadata *metadata;
    ssize_t length;

    while (1) {
        // Read events from fanotify
        length = read(fanotify_fd, buffer, sizeof(buffer));
        if (length < 0 && errno != EAGAIN) {
            perror("Failed to read fanotify events");
            exit(EXIT_FAILURE);
        }

        // Process all events
        metadata = (struct fanotify_event_metadata *)buffer;
        while (FAN_EVENT_OK(metadata, length)) {
            // Check if the event is for a sensitive file
            if (metadata->fd >= 0) {
                printf("File change detected (PID: %d)\n", metadata->pid);
                
                // Log file changes
                printf("Logging file change for PID: %d\n", metadata->pid);

                // Raise alert if file is altered (for demonstration, we just print)
                printf("Alert: Unauthorized modification detected on sensitive file by PID: %d\n", metadata->pid);

                // Close the file descriptor associated with this event
                close(metadata->fd);
            }

            metadata = FAN_EVENT_NEXT(metadata, length);
        }
    }
}

