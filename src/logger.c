/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Richard Hawkins
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "logger.h"


FILE *log_debug;
FILE *log_info;
FILE *log_warning;
FILE *log_error;

char LOG_STR_DEBUG[] = "DEBUG";
char LOG_STR_INFO[] = "INFO";
char LOG_STR_WARNING[] = "WARNING";
char LOG_STR_ERROR[] = "ERROR";

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
    char *log_str = NULL;

    switch(mode) {
    case DEBUG:
        out = log_debug;
        log_str = LOG_STR_DEBUG;
        break;
    case INFO:
        out = log_info;
        log_str = LOG_STR_INFO;
        break;
    case WARNING:
        out = log_warning;
        log_str = LOG_STR_WARNING;
        break;
    case ERROR:
        out = log_error;
        log_str = LOG_STR_ERROR;
        break;
        break;
    }

    if (out == NULL) {
        return;
    }

    va_list arg;
    int done;

    va_start (arg, msg);
    if (mode != INFO || log_debug != NULL) {
        fprintf(out, "%s %d %s %d: ", log_str, (int)time(NULL), file, line);
    }
    done = vfprintf (out, msg, arg);
    fprintf(out, "\n");
    va_end (arg);
}
