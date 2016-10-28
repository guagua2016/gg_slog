//
//  slog_api.cpp
//  slog
//
//  Created by luoning on 15-1-6.
//  Copyright (c) 2015年 luoning. All rights reserved.
//

#include "slog_api.h"
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"
#include <stdarg.h>
#include <vector>
#include <algorithm>
using namespace log4cxx;

#define MAX_LOG_LENGTH   1024 * 10

class CSLogObject
{
public:
    CSLogObject(const char* module_name, int delay = WATCH_DELAY_TIME) {}
    virtual ~CSLogObject() {}
    
    virtual void Trace(const char* loginfo) {}
    virtual void Debug(const char* loginfo) {}
    virtual void Info(const char* loginfo) {}
    virtual void Warn(const char* loginfo) {}
    virtual void Error(const char* loginfo) {}
    virtual void Fatal(const char* loginfo) {}
    virtual bool SetLevel(enLevel level) {}
    virtual bool RaiseLevel() {return true;}
    virtual bool DecreaseLevel() {return true;}
};

class CLog4CXX : public CSLogObject
{
public:
    CLog4CXX(const char* module_name, int delay = WATCH_DELAY_TIME);
    virtual ~CLog4CXX();
    
    void Trace(const char* loginfo);
    void Debug(const char* loginfo);
    void Info(const char* loginfo);
    void Warn(const char* loginfo);
    void Error(const char* loginfo);
    void Fatal(const char* loginfo);
    bool SetLevel(enLevel level);
    bool RaiseLevel();
    bool DecreaseLevel();
private:
    LoggerPtr m_logger;
    std::vector<int> m_levels;
};

CLog4CXX::CLog4CXX(const char* module_name, int delay) : CSLogObject(module_name, delay)
{
    PropertyConfigurator::configureAndWatch("log4cxx.properties", delay);
    m_logger = Logger::getLogger(module_name);

    m_levels.push_back(Level::ALL_INT);
    m_levels.push_back(Level::TRACE_INT);
    m_levels.push_back(Level::DEBUG_INT);
    m_levels.push_back(Level::INFO_INT);
    m_levels.push_back(Level::WARN_INT);
    m_levels.push_back(Level::ERROR_INT);
    m_levels.push_back(Level::FATAL_INT);
    m_levels.push_back(Level::OFF_INT);
}

CLog4CXX::~CLog4CXX()
{
}

void CLog4CXX::Trace(const char *loginfo)
{
    m_logger->trace(loginfo);
    //LOG4CXX_TRACE(m_logger, loginfo);
}

void CLog4CXX::Debug(const char *loginfo)
{
    m_logger->debug(loginfo);
}

void CLog4CXX::Info(const char *loginfo)
{
    m_logger->info(loginfo);
}

void CLog4CXX::Warn(const char *loginfo)
{
    m_logger->warn(loginfo);
}

void CLog4CXX::Error(const char *loginfo)
{
    m_logger->error(loginfo);
}

void CLog4CXX::Fatal(const char *loginfo)
{
    m_logger->fatal(loginfo);
}

bool CLog4CXX::SetLevel(enLevel level)
{
	switch(level) {
		case enFATAL:
			m_logger->setLevel(Level::getFatal());
			break;

		case enERROR:
			m_logger->setLevel(Level::getError());
			break;

		case enWARN:
			m_logger->setLevel(Level::getWarn());
			break;

		case enINFO:
			m_logger->setLevel(Level::getInfo());
			break;

		case enDEBUG:
			m_logger->setLevel(Level::getDebug());
			break;

		case enTRACE:
			m_logger->setLevel(Level::getTrace());
			break;

		default:
			return false;
	}
	return true;
}

bool CLog4CXX::RaiseLevel()
{
	int nLevel = m_logger->getLevel()->toInt();
	std::vector<int>::iterator pIterFound =
			std::find(m_levels.begin(), m_levels.end(),nLevel);
	if (*++pIterFound != Level::OFF_INT) {
		m_logger->setLevel(Level::toLevel(*pIterFound));
		return true;
	}

	return false;
}


bool CLog4CXX::DecreaseLevel()
{
	int nLevel = m_logger->getLevel()->toInt();
	std::vector<int>::iterator pIterFound =
			std::find(m_levels.begin(), m_levels.end(),nLevel);
	int nMin = Level::ALL_INT;
	if (*--pIterFound != Level::ALL_INT) {
		m_logger->setLevel(Level::toLevel(*pIterFound));
		return true;
	}

	return false;
}



CSLog::CSLog(const char* module_name, int delay)
{
    m_log = new CLog4CXX(module_name, delay);
}

CSLog::~CSLog()
{
    delete m_log;
}

void CSLog::Trace(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char szBuffer[MAX_LOG_LENGTH];
    vsnprintf(szBuffer, sizeof(szBuffer), format, args);
    va_end(args);
    m_log->Trace(szBuffer);
}

void CSLog::Debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char szBuffer[MAX_LOG_LENGTH];
    vsnprintf(szBuffer, sizeof(szBuffer) , format, args);
    va_end(args);
    m_log->Debug(szBuffer);
}

void CSLog::Info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char szBuffer[MAX_LOG_LENGTH];
    vsnprintf(szBuffer, sizeof(szBuffer), format, args);
    va_end(args);
    m_log->Info(szBuffer);
}

void CSLog::Warn(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char szBuffer[MAX_LOG_LENGTH];
    vsnprintf(szBuffer, sizeof(szBuffer), format, args);
    va_end(args);
    m_log->Warn(szBuffer);
}

void CSLog::Error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char szBuffer[MAX_LOG_LENGTH];
    vsnprintf(szBuffer, sizeof(szBuffer), format, args);
    va_end(args);
    m_log->Error(szBuffer);
}

void CSLog::Fatal(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char szBuffer[MAX_LOG_LENGTH];
    vsnprintf(szBuffer, sizeof(szBuffer), format, args);
    va_end(args);
    m_log->Fatal(szBuffer);
}

bool CSLog::RaiseLevel()
{
	return m_log->RaiseLevel();
}


bool CSLog::DecreaseLevel()
{
	return m_log->DecreaseLevel();
}


bool CSLog::SetLevel(enLevel enLevel)
{
	return m_log->SetLevel(enLevel);
}




/*
int main(int argc, char* argv[])
{
    CLog a("test2");

    for (int i = 0; i < 100000; i++) {
        a.Warn("aaa,%s", "bbb");
    }
    //CLog4CXX b("test2");
    //b.DEBUG("bbbbbb");
    return 0;
}
*/

