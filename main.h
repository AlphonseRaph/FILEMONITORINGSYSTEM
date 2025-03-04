#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/fanotify.h>

// Function declarations
void block_unauthorized_access(int fanotify_fd, const char *restricted_directory);
void log_file_changes_and_alert(int fanotify_fd, const char *sensitive_file);

#endif // MAIN_H
