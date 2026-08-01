#include "Logger.h"

#include <iostream>

void Logger::Internal::LogInternal(const std::string& message) {
    std::cout << message << std::endl;
}
