#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "logger.h"


FILE *log_debug;
FILE *log_info;
FILE *log_warning;
FILE *log_error;

void init_logger(FILE *debug, FILE *info, FILE *warning, FILE *error)
{
    log_debug = debug;
    log_info = info;
    log_warning = warning;
    log_error = error;
}

void _log_message(enum log_mode mode, char *file, int line, char *msg, ...)
{
    FILE *out = NULL;

    switch(mode) {
    case DEBUG:
        #ifdef _DEBUG
        out = log_debug;
        #endif
        break;
    case INFO:
        out = log_info;
        break;
    case WARNING:
        out = log_warning;
        break;
    case ERROR:
        out = log_error;
        break;
    }

    if (out == NULL) {
        return;
    }

    va_list arg;
    int done;

    va_start (arg, msg);
    fprintf(out, "DEBUG %d %s %d: ", (int)time(NULL), file, line);
    done = vfprintf (out, msg, arg);
    fprintf(out, "\n");
    va_end (arg);
}
