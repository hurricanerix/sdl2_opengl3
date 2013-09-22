#include <stdio.h>
#include <time.h>

#include "logger.h"


FILE *log_out;
FILE *log_err;

void init_logger(FILE *out, FILE *err) 
{
    log_out = out;
    log_err = err;

    if (out == NULL) {
        log_out = stdout;
    }
    if (err == NULL) {
        log_err = stderr;
    }
}

void _log_debug(char *file, int line, char *msg)
{
#   ifdef LOG_DEBUG
    fprintf(log_out, "DEBUG %d %s %d: %s\n", (int)time(NULL), file, line, msg);
#   endif
}

void _log_info(char *file, int line, char *msg)
{
    fprintf(log_out, "INFO %d %s %d: %s\n", (int)time(NULL), file, line, msg);
}

void _log_warning(char *file, int line, char *msg)
{
    fprintf(log_out, "WARNING %d %s %d: %s\n", (int)time(NULL), file, line, msg);
}

void _log_error(char *file, int line, char *msg)
{
    fprintf(log_err, "ERROR %d %s %d: %s\n", (int)time(NULL), file, line, msg);
}
