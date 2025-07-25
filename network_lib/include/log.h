/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** log
*/

#ifndef LOG_H_
    #define LOG_H_

            /* ================== MACROS ================== */

    #define RED     "\033[1;31m"
    #define GREEN   "\033[1;32m"
    #define YELLOW  "\033[1;33m"
    #define BLUE    "\033[1;34m"
    #define RESET   "\033[0m"

    #define LOG_INFO(...)  log_message(LOG_LEVEL_INFO, __VA_ARGS__)
    #define LOG_DEBUG(...) log_message(LOG_LEVEL_DEBUG, __VA_ARGS__)
    #define LOG_WARN(...)  log_message(LOG_LEVEL_WARN, __VA_ARGS__)
    #define LOG_ERROR(...) log_message(LOG_LEVEL_ERROR, __VA_ARGS__)

            /* ================== INCLUDES ================== */

    #include <stdio.h>
    #include <stdarg.h>
    #include <stddef.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <errno.h>

            /* ================== ENUM ================== */

typedef enum log_level_e {
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
} log_level_t;

            /* ================== STRUCT ================== */

typedef struct log_meta_s {
    const char *color;
    const char *label;
} log_meta_t;

            /* ================== UTILS ================== */

/**
 * @brief Affiche un message de log formaté avec un niveau donné.
 *
 * Cette fonction agit comme un printf amélioré, permettant d'afficher
 * des messages de journalisation (log) avec un niveau
 * (INFO, DEBUG, ERROR, WARN).
 * Le message est affiché sur la sortie standard ou une sortie adaptée selon
 * l'implémentation.
 *
 * @param level Le niveau de log
 * (LOG_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_WARN, LOG_ERROR).
 * @param fmt   La chaîne de format (comme pour printf).
 * @param ...   Les arguments variables correspondant au format.
 */
void log_message(log_level_t level, const char *fmt, ...);

#endif /* !LOG_H_ */
