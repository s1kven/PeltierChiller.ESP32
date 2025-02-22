#include "LogService.h"

void Services::LogService::setNewFileToLog()
{
    char buffer[100];
    struct tm startupTime;
    if(_isOnTimer)
    {
        startupTime = _timeService->getCurrentTime();
    }
    else
    {
        startupTime = _timeService->getStartupTime();
    }

    if(_timeService->isValidTime(startupTime))
    {
        strftime(buffer, sizeof(buffer), "/%Y%m%d_%H%M%S", &startupTime);
        _currentFileToLog = strcat(buffer, ".txt");
    }
    else
    {
        _currentFileToLog = _defaultLogFile;
    }
    _fileService->createFile(_logsFolder.c_str(), _currentFileToLog.c_str());
    if(!_currentFileToLog.equals(_defaultLogFile))
    {
        _fileService->setFileTimestamp((_logsFolder + _currentFileToLog).c_str(), startupTime);
    }
}

void Services::LogService::deleteExpiredLogs()
{
    struct tm startupTime = _timeService->getStartupTime();
    if(_timeService->isValidTime(startupTime))
    {
        uint64_t expirationMillis = (uint64_t)_logExpiration * 24 * 60 * 60 * 1000;//days * hours * minutes * sec * ms
        struct tm expirationTime = _timeService->subtractMilliseconds(startupTime, expirationMillis);
        _fileService->deleteFilesOlderThanDate(_logsFolder, expirationTime, _filesIgnoredToDelete, _filesIgnoredToDeleteCount);
    }
}

Services::LogService::LogService(Communication::Models::Configurations::LogConfiguration *logConfig, uint32_t logFileInitDelay)
{
    _isEnabled = logConfig->getIsEnabled();
    _logDelay = logConfig->getLogDelay();
    _logExpiration = logConfig->getLogExpiration();
    strptime(logConfig->getCreateNewLogTime().c_str(), "%H:%M:%S", &_createNewLogTime);
    _logFileInitDelay = logFileInitDelay;
}

Services::LogService::~LogService()
{
    releaseCurrentTimer();
}

void Services::LogService::addToLogQueue(String content)
{
    if(!_isEnabled)
    {
        return;
    }

    String logContent;
    char buffer[100];
    struct tm currentTime = _timeService->getCurrentTime();
    if(_timeService->isValidTime(currentTime))
    {
        strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &currentTime);
        logContent = String(buffer) + " | " + String(millis()) + " -> " + content;
    }
    else
    {
        logContent = String(millis()) + " -> " + content;
    }
    _logQueue.push(logContent);
}

void Services::LogService::pushContentToLog()
{
    if(!_isEnabled)
    {
        return;
    }

    if(!_isLogFileInit)
    {
        if(millis() - _initMillis > _logFileInitDelay)
        {
            deleteExpiredLogs();

            setNewFileToLog();
            if(_timeService->isValidTime(_timeService->getCurrentTime()))
            {
                _timeService->startTimer(this, _createNewLogTime);
            }
            _isLogFileInit = true;
        }
    }
    else
    {
        _fileService->appendFile(_logsFolder.c_str(), _currentFileToLog.c_str(), _logQueue);
    }
}

void Services::LogService::onTimer()
{
    _isLogFileInit = false;
    _isOnTimer = true;
}

uint32_t Services::LogService::getLogDelay()
{
    return _logDelay;
}

bool Services::LogService::getIsEnabled()
{
    return _isEnabled;
}
