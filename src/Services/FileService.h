#pragma once
#ifndef _FileService_
#define _FileService_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#include <queue>
#else
#include "WProgram.h"
#endif

#include <LinkedList.h>
#include <SPI.h>
#include <SdFat.h>
#include "FreeStack.h"
#include "Helpers/FileInfo.h"
#include "TimeService.h"

extern Services::TimeService* _timeService;

namespace Services
{
	class FileService
	{
    private:
        SdFat _sd;

		bool createDirectory(const char* directory);
		LinkedList<Helpers::FileInfo>* getFilesOlderThanDate(String directory, struct tm selectedDate);

	public:
		bool init(uint8_t cs);

		String readFile(const char* path);
		uint32_t appendFile(const char* fileName, String content);
		uint32_t appendFile(const char* path, const char* fileName, String content);
		uint32_t appendFile(const char* path, const char* fileName, std::queue<String>& content);
		uint32_t writeFile(const char* fileName, String content);
		uint32_t writeFile(const char* path, const char* fileName, String content);
		bool createFile(const char* fileName);
		bool createFile(const char* path, const char* fileName);
		bool setFileTimestamp(const char* fileName, tm time);
		void deleteFilesOlderThanDate(String directory, struct tm selectedDate, String ignoreFiles [], uint16_t ignoreFilesCount);
	};
}
#endif