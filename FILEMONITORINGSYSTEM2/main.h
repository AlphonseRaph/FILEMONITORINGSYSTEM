#ifndef MAIN_H
#define MAIN_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/fanotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
void block_unauthorized_access(int fanotify_fd, const char *restricted_directory);
void log_file_changes_and_alert(int fanotify_fd, const char *sensitive_file);
#endif
