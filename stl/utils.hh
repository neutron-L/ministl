/* Logging */
#include <stdio.h>

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