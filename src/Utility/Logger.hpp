//
// Created by MTesseracT on 20-9-2017.
//

#ifndef VULKANENGINE_LOG_HPP
#define VULKANENGINE_LOG_HPP

#include <iostream>
#include <string>
#include "Timer.hpp"

enum LogLevel{
    LevelAll = 0,
    LevelDebug = 1,
    LevelInfo = 2,
    LevelError = 3,
    LevelOff = 4
};

enum LogType
{
    LogDebug = 1,
    LogInfo = 2,
    LogError = 3,
};

class Logger
{
public:
    static void Log(std::string p_message, LogType p_level = LogInfo)
    {
        Logger * logger = GetInstance();

        if(static_cast<int>(p_level) < static_cast<int>(logger->m_logLevel)) return; //Not logging the message if under loglevel

        switch (p_level)
        {
            case LogDebug:
                logger->PrintLog("DEBUG", p_message);
                break;
            case LogInfo:
                logger->PrintLog("INFO", p_message);
                break;
            case LogError:
                logger->PrintError("ERROR", p_message);
                break;
        }
    }

    static void SetLoglevel(LogLevel p_level)
    {
        GetInstance()->m_logLevel = p_level;
    }

private:
    LogLevel m_logLevel;

    void PrintError(std::string m_prefix, std::string p_message)
    {
        std::string bracketTag = "[" + m_prefix + "] ";
        std::cerr << bracketTag << p_message << std::endl;
    }

    void PrintLog(std::string m_prefix, std::string p_message)
    {
        std::string bracketTag = "[" + m_prefix + "] ";
        std::cout << bracketTag << p_message << std::endl;
    }

    Logger()
    {
        m_logLevel = LogLevel::LevelAll;
    }

    virtual ~Logger()
    {
        _instance = nullptr;
    }

    static Logger * GetInstance()
    {
        if(_instance == nullptr)
        {
            _instance = new Logger();
        }
        return _instance;
    }

    static Logger * _instance;
};
#endif //VULKANENGINE_LOG_HPP
