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
