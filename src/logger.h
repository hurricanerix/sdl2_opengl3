#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

enum log_mode {DEBUG, INFO, WARNING, ERROR};

void init_logger(FILE *debug, FILE *info, FILE *warning, FILE *error);
void _log_message(enum log_mode mode, char *file, int line, char *msg, ...);

#define log_debug(msg, ...) (_log_message(DEBUG, __FILE__, __LINE__, msg, ##__VA_ARGS__))
#define log_info(msg, ...) (_log_message(INFO, __FILE__, __LINE__, msg, ##__VA_ARGS__))
#define log_warning(msg, ...) (_log_message(WARNING, __FILE__, __LINE__, msg, ##__VA_ARGS__))
#define log_error(msg, ...) (_log_message(ERROR, __FILE__, __LINE__, msg, ##__VA_ARGS__))

#endif//__LOGGER_H__
