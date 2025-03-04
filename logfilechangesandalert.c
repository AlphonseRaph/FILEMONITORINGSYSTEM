void log_file_changes_and_alert(int fanotify_fd, const char *sensitive_file) {
    char buffer[1024];
    struct fanotify_event_metadata *metadata;
    ssize_t length;

    while (1) {
        length = read(fanotify_fd, buffer, sizeof(buffer));
        if (length < 0 && errno != EAGAIN) {
            perror("Failed to read fanotify events");
            exit(EXIT_FAILURE);
        }

        metadata = (struct fanotify_event_metadata *)buffer;
        while (FAN_EVENT_OK(metadata, length)) {
            if (metadata->fd >= 0) {
                // Log the event to a file
                FILE *log_file = fopen("file_monitor_log.txt", "a");
                if (log_file) {
                    fprintf(log_file, "File change detected (PID: %d)\n", metadata->pid);
                    fclose(log_file);
                } else {
                    perror("Failed to open log file");
                }

                // Close the file descriptor
                close(metadata->fd);
            }
            metadata = FAN_EVENT_NEXT(metadata, length);
        }
    }
}
