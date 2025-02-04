//
// Created by sebas on 30/12/2024.
//

#ifndef LOG_H
#define LOG_H

#define LOG_DEFAULT 0
#define LOG_DEBUG 1
#define LOG_INFO 2
#define LOG_WARNING 3
#define LOG_ERROR 4
#define LOG_FATAL 5

#define ASSERT(condition, message) if (!(condition)) {              \
    log(LOG_ERROR, "Assertion failed: " message "\n");              \
    return EXIT_FAILURE;                                            \
}

#define FATAL_ASSERT(condition, message) if (!(condition)) {        \
    log(LOG_FATAL, "Assertion failed: " message "\n");              \
    exit(EXIT_FAILURE);                                             \
}

extern int log_pos;

static bool doNotLogDefault = false;
static bool doNotLogDebug = false;
static bool doNotLogInfo = true;
static bool doNotLogWarning = false;
static bool doNotLogError = false;
static bool doNotLogFatal = false;

void log(int level, const char* msg);

#endif //LOG_H
