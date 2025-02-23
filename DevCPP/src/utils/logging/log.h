//
// Created by sebas on 30/12/2024.
//

#ifndef LOG_H
#define LOG_H

#define CONSOLE_COLOR_RESET           "\033[0m"

#define CONSOLE_COLOR_WHITE           "\033[1;37m"
#define CONSOLE_COLOR_BLACK           "\033[0;30m"
#define CONSOLE_COLOR_GRAY            "\033[0;90m"
#define CONSOLE_COLOR_LIGHT_GRAY      "\033[1;90m"

#define CONSOLE_COLOR_RED             "\033[1;31m"
#define CONSOLE_COLOR_LIGHT_RED       "\033[1;91m"
#define CONSOLE_COLOR_DARK_RED        "\033[0;31m"

#define CONSOLE_COLOR_GREEN           "\033[1;32m"
#define CONSOLE_COLOR_LIGHT_GREEN     "\033[1;92m"
#define CONSOLE_COLOR_DARK_GREEN      "\033[0;32m"

#define CONSOLE_COLOR_BLUE            "\033[1;34m"
#define CONSOLE_COLOR_LIGHT_BLUE      "\033[1;94m"
#define CONSOLE_COLOR_DARK_BLUE       "\033[0;34m"

#define CONSOLE_COLOR_YELLOW          "\033[1;33m"
#define CONSOLE_COLOR_LIGHT_YELLOW    "\033[1;93m"
#define CONSOLE_COLOR_DARK_YELLOW     "\033[0;33m"

#define CONSOLE_COLOR_MAGENTA         "\033[1;35m"
#define CONSOLE_COLOR_LIGHT_MAGENTA   "\033[1;95m"
#define CONSOLE_COLOR_DARK_MAGENTA    "\033[0;35m"

#define CONSOLE_COLOR_CYAN            "\033[1;36m"
#define CONSOLE_COLOR_LIGHT_CYAN      "\033[1;96m"
#define CONSOLE_COLOR_DARK_CYAN       "\033[0;36m"

#define LOG_COLOR_DEFAULT   CONSOLE_COLOR_WHITE
#define LOG_COLOR_DEBUG     CONSOLE_COLOR_MAGENTA
#define LOG_COLOR_INFO      CONSOLE_COLOR_BLUE
#define LOG_COLOR_GL_INFO   CONSOLE_COLOR_GREEN
#define LOG_COLOR_WARNING   CONSOLE_COLOR_YELLOW
#define LOG_COLOR_ERROR     CONSOLE_COLOR_RED
#define LOG_COLOR_FATAL     CONSOLE_COLOR_DARK_RED

#include <iostream>

#define ASSERT(condition, message) if (!(condition)) {              \
    ltr_log_error("Assertion failed: " message "\n");              \
    return EXIT_FAILURE;                                            \
}

#define FATAL_ASSERT(condition, message) if (!(condition)) {        \
    ltr_log_fatal("Assertion failed: " message "\n");              \
    exit(EXIT_FAILURE);                                             \
}

extern int log_pos;

constexpr bool doNotLogDefault = false;
constexpr bool doNotLogDebug = false;
constexpr bool doNotLogInfo = false;
constexpr bool doNotLogGlInfo = false;
constexpr bool doNotLogWarning = false;
constexpr bool doNotLogError = false;
constexpr bool doNotLogFatal = false;

void log_in_console();

template<typename T, typename... Args>
void log_in_console(T first, Args... rest) {
    std::cout << first;
    log_in_console(rest...);
}

template<typename T, typename... Args>
void ltr_log(T first, Args... rest) {
    if (doNotLogDefault) return;
    std::cout << LOG_COLOR_DEFAULT << first;
    log_in_console(rest...);
    std::cout << CONSOLE_COLOR_RESET << std::endl;
}

template<typename T, typename... Args>
void ltr_log_debug(T first, Args... rest) {
    if (doNotLogDebug) return;
    std::cout << LOG_COLOR_DEBUG "DEBUG : " << first;
    log_in_console(rest...);
    std::cout << CONSOLE_COLOR_RESET << std::endl;
}

template<typename T, typename... Args>
void ltr_log_info(T first, Args... rest) {
    if (doNotLogInfo) return;;
    std::cout << LOG_COLOR_INFO "INFO : " << first;
    log_in_console(rest...);
    std::cout << CONSOLE_COLOR_RESET << std::endl;
}

template<typename T, typename... Args>
void ltr_log_gl_info(T first, Args... rest) {
    if (doNotLogGlInfo) return;;
    std::cout << LOG_COLOR_GL_INFO "GL INFO : " << first;
    log_in_console(rest...);
    std::cout << CONSOLE_COLOR_RESET << std::endl;
}

template<typename T, typename... Args>
void ltr_log_warn(T first, Args... rest) {
    if (doNotLogWarning) return;;
    std::cout << LOG_COLOR_WARNING "WARN : " << first;
    log_in_console(rest...);
    std::cout << CONSOLE_COLOR_RESET << std::endl;
}

template<typename T, typename... Args>
void ltr_log_error(T first, Args... rest) {
    if (doNotLogError) return;;
    std::cout << LOG_COLOR_ERROR "ERROR : " << first;
    log_in_console(rest...);
    std::cout << CONSOLE_COLOR_RESET << std::endl;
}

template<typename T, typename... Args>
void ltr_log_fatal(T first, Args... rest) {
    if (doNotLogFatal) return;;
    std::cout << LOG_COLOR_FATAL "FATAL : " << first;
    log_in_console(rest...);
    std::cout << CONSOLE_COLOR_RESET << std::endl;
}

#endif //LOG_H
