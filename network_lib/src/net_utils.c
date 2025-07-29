/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net_utils
*/

#include "../include/log.h"
#include <stdio.h>
#include <unistd.h>

static log_meta_t get_log_label_color(log_level_t level)
{
    switch (level) {
        case LOG_LEVEL_INFO:
            return (log_meta_t){BLUE, "INFO"};
        case LOG_LEVEL_DEBUG:
            return (log_meta_t){GREEN, "DEBUG"};
        case LOG_LEVEL_WARN:
            return (log_meta_t){YELLOW, "WARN"};
        case LOG_LEVEL_ERROR:
            return (log_meta_t){RED, "ERROR"};
        default:
            return (log_meta_t){RESET, "LOG"};
    }
}

static FILE *get_log_output(log_level_t level)
{
    if (level == LOG_LEVEL_WARN || level == LOG_LEVEL_ERROR)
        return stderr;
    return stdout;
}

void log_message(log_level_t level, const char *fmt, ...)
{
    log_meta_t meta = get_log_label_color(level);
    FILE *output = get_log_output(level);
    va_list args;

    va_start(args, fmt);
    fprintf(output, "%s[%s]%s ", meta.color, meta.label, RESET);
    vfprintf(output, fmt, args);
    fprintf(output, "\n");
    va_end(args);
}

void close_socket(char *msg, int socket)
{
    perror(msg);
    close(socket);
}

void net_send(int fd, void *msg, size_t msg_size)
{
    int write_return = write(fd, msg, msg_size);

    if (write_return == -1)
        printf("An error occurs during the sending of the message, be better\n");
    else
        printf("The message has been sent successfully!\n");
}
// void net_send(int fd, const char *msg)
// {
//     size_t len = 0;
//     ssize_t sent = 0;
//     size_t total_sent = 0;
//
//     if (!msg || fd < 0)
//         return;
//     len = strlen(msg);
//     while (total_sent < len) {
//         sent = send(fd, msg + total_sent, len - total_sent, 0);
//         if (sent < 0) {
//             if (errno == EINTR)
//                 continue;
//             perror("send");
//             break;
//         }
//         if (sent == 0)
//             break;
//         total_sent += sent;
//     }
// }
