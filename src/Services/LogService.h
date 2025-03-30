#pragma once
#ifndef _LogService_
#define _LogService_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <queue>
#include "FileService.h"
#include "TimeService.h"
#include "WifiService.h"
#include "Communication/Models/Configurations/LogConfiguration.h"
#include "Helpers/TimerHandler.h"

extern Services::FileService* _fileService;
extern Services::TimeService* _timeService;
extern Services::WifiService* _wifiService;
extern uint64_t _initMillis;
extern Helpers::TimerHandler* _timerHandler;

namespace Services
{
	class LogService
	{
    private:
		static bool _isLogFileInit;
		static bool _isOnTimer;

		String _defaultLogFile = "/Latest.txt";
        String _logsFolder = "/Logs";
		String _currentFileToLog;
		bool _isEnabled;
        uint32_t _logDelay;
		uint16_t _logExpiration;
		struct tm _createNewLogTime;
		uint32_t _logFileInitDelay;
		std::queue<String> _logQueue;
		static const uint16_t _filesIgnoredToDeleteCount = 1;
		String _filesIgnoredToDelete[_filesIgnoredToDeleteCount] = { _defaultLogFile };

		void setNewFileToLog();
		void deleteExpiredLogs();

	public:
		LogService(Communication::Models::Configurations::LogConfiguration* logConfig, uint32_t logFileInitDelay);
		~LogService();

		void addToLogQueue(String content);
		void pushContentToLog();

		uint32_t getLogDelay();
		bool getIsEnabled();

		static void setLogFileInit(bool isLogFileInit);
		static void setOnTimer(bool isOnTimer);
	};
}
#endif