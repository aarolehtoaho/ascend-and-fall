#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <ctime>

class Logger {
private:
    std::ofstream* logFile;
public:
    Logger(const std::string &fileName) {
        logFile = new std::ofstream(fileName, std::ios::app);
    }
    void log(const std::string &message) {
        (*logFile) << message << std::endl;
    }
    void logTime() {
        time_t timestamp;
        time(&timestamp);
        log(ctime(&timestamp));
    }
    static Logger& getGlobal() {
        static Logger globalLogger("debug.log");
        return globalLogger;
    }
};
#endif