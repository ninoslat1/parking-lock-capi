#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

#define _CLR_RESET   "\033[0m"
#define _CLR_GREY    "\033[90m"
#define _CLR_GREEN   "\033[32m"
#define _CLR_YELLOW  "\033[33m"
#define _CLR_RED     "\033[31m"
#define _CLR_BLUE    "\033[34m"
#define _CLR_CYAN    "\033[36m"
#define _CLR_BOLD    "\033[1m"

static inline void _log_timestamp(char *buf, size_t size) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

#define _LOG(level, label_color, label, fmt, ...) do {  \
    char _ts[20];                                        \
    _log_timestamp(_ts, sizeof(_ts));                    \
    fprintf(                                             \
        stderr,                                          \
        _CLR_GREY "%s " _CLR_RESET                       \
        label_color _CLR_BOLD "%-7s" _CLR_RESET          \
        fmt "\n",                                        \
        _ts, label, ##__VA_ARGS__                        \
    );                                                   \
} while (0)


#define LOG_INFO(fmt, ...)  _LOG("INFO",  _CLR_GREEN,  "[INFO]",  fmt, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...)  _LOG("WARN",  _CLR_YELLOW, "[WARN]",  fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) _LOG("ERROR", _CLR_RED,    "[ERROR]", fmt, ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) _LOG("DEBUG", _CLR_CYAN,   "[DEBUG]", fmt, ##__VA_ARGS__)

#define LOG_DB(fmt, ...)    _LOG("DB",    _CLR_BLUE,   "[DB]",    fmt, ##__VA_ARGS__)

#define LOG_HTTP(fmt, ...)  _LOG("HTTP",  "\033[35m",  "[HTTP]",  fmt, ##__VA_ARGS__)

#define LOG_REQUEST(method, path, status) do {                              \
    const char *_color = (status) < 300 ? _CLR_GREEN :                     \
                         (status) < 400 ? _CLR_CYAN  :                     \
                         (status) < 500 ? _CLR_YELLOW : _CLR_RED;          \
    char _ts[20];                                                           \
    _log_timestamp(_ts, sizeof(_ts));                                       \
    fprintf(                                                                \
        stderr,                                                             \
        _CLR_GREY "%s " _CLR_RESET                                          \
        "\033[35m" _CLR_BOLD "%-7s" _CLR_RESET                             \
        "[HTTP]  "                                                          \
        _CLR_BOLD "%s" _CLR_RESET                                          \
        " → " _color "%d" _CLR_RESET "\n",                                 \
        _ts, "[HTTP]", path, status                                         \
    );                                                                      \
} while (0)

#endif // LOGGER_H