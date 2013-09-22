#ifndef __LOGGER_H__
#define __LOGGER_H__

void init_logger(FILE *out, FILE *err);

void _log_debug(char *file, int line, char *msg);
void _log_info(char *file, int line, char *msg);
void _log_warning(char *file, int line, char *msg);
void _log_error(char *file, int line, char *msg);

#define log_debug(msg) (_log_debug(__FILE__, __LINE__, (msg)))
#define log_info(msg) (_log_info(__FILE__, __LINE__, (msg)))
#define log_warning(msg) (_log_warning(__FILE__, __LINE__, (msg)))
#define log_error(msg) (_log_error(__FILE__, __LINE__, (msg)))

#endif//__LOGGER_H__
