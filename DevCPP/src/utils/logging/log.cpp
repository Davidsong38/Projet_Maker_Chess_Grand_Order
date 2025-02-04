//
// Created by sebas on 30/12/2024.
//

#include "log.h"

#include <iostream>

int log_pos = 0;

void log(int level, const char* msg) {
    switch (level) {
        case LOG_DEBUG: {
            if (doNotLogDebug) break;
            std::cout << "DEBUG : " << msg << std::endl;
            break;
        }
        case LOG_INFO: {
            if (doNotLogInfo) break;
            std::cout << "INFO : " << msg << std::endl;
            break;
        }
        case LOG_WARNING: {
            if (doNotLogWarning) break;
            std::cout << "WARNING : " << msg << std::endl;
            break;
        }
        case LOG_ERROR: {
            if (doNotLogError) break;
            std::cerr << "ERROR : " << msg << " at pos " << log_pos << std::endl;
            break;
        }
        case LOG_FATAL: {
            if (doNotLogFatal) break;
            std::cerr << "FATAL : " << msg << std::endl;
            break;
        }
        default: {
            if (doNotLogDefault) break;
            std::cout << msg << std::endl;
        }
    }
}