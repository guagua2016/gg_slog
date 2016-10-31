//
//  slog_api.h
//  slog
//
//  Created by luoning on 15-1-6.
//  Copyright (c) 2015年 luoning. All rights reserved.
//

#ifndef __slog__slog_api__
#define __slog__slog_api__

#include <stdio.h>
#include <string>

#define WATCH_DELAY_TIME     10 * 1000

enum enLevel{
	enFATAL,
	enERROR,
	enWARN,
	enINFO,
	enDEBUG,
	enTRACE,
};

class CSLogObject;

class CSLog
{
public:
    CSLog(const char* module_name, int delay = WATCH_DELAY_TIME);
    virtual ~CSLog();
    
    void Trace(const char* format, ...);
    void Debug(const char* format, ...);
    void Info(const char* format, ...);
    void Warn(const char* format, ...);
    void Error(const char* format, ...);
    void Fatal(const char* format, ...);
    bool SetLevel(enLevel level);
    void GetLevel(std::string& strLevel);
    bool RaiseLevel();
    bool DecreaseLevel();
private:
    CSLogObject* m_log;
};



#endif /* defined(__slog__slog_api__) */
