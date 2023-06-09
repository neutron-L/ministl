#ifndef MINISTL_LOG_HH
#define MINISTL_LOG_HH


/* Logging */
#include <stdio.h>

#include <iostream>
#include <initializer_list>
#include <vector>

#ifdef DEBUG
#define debug(M, ...) \
    fprintf(stderr, "DEBUG %s:%d:%s: " M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define debug(M, ...)
#endif

#define info(M, ...) \
    fprintf(stderr, "INFO  " M "\n", ##__VA_ARGS__)

#define error(M, ...) \
    fprintf(stderr, "ERROR " M "\n", ##__VA_ARGS__)


#endif
