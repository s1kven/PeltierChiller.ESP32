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
#include "Communication/Models/Configurations/LogConfiguration.h"
#include "Abstractions/BaseTimerCallback.h"

extern Services::FileService* _fileService;
extern Services::TimeService* _timeService;
extern uint64_t _initMillis;

namespace Services
{
	class LogService :
		public Abstractions::BaseTimerCallback
	{
    private:
		String _defaultLogFile = "/Latest.txt";
        String _logsFolder = "/Logs";
		String _currentFileToLog;
		bool _isEnabled;
        uint32_t _logDelay;
		uint16_t _logExpiration;
		struct tm _createNewLogTime;
		uint32_t _logFileInitDelay;
		bool _isLogFileInit = false;
		bool _isOnTimer = false;
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
		void onTimer() override;

		uint32_t getLogDelay();
		bool getIsEnabled();
	};
}
#endif