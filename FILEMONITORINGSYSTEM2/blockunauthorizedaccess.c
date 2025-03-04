#include "main.h"
// Function to handle unauthorized access
void block_unauthorized_access(int fanotify_fd, const char *restricted_directory) {
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
            // Check if the event relates to the restricted directory
            if (metadata->fd >= 0) {
                printf("Access attempt on a restricted directory detected (PID: %d)\n", metadata->pid);
                // Block access by responding with FAN_DENY
                struct fanotify_response response;
                response.fd = metadata->fd;
                response.response = FAN_DENY;
                write(fanotify_fd, &response, sizeof(response));
                printf("Access blocked for PID: %d\n", metadata->pid);

                // Notify security team (for demonstration, we just print)
                printf("Security team notified about unauthorized access by PID: %d\n", metadata->pid);

                // Close the file descriptor associated with this event
                close(metadata->fd);
            }

            metadata = FAN_EVENT_NEXT(metadata, length);
        }
    }
}

