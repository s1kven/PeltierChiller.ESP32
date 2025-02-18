#pragma once
#ifndef _FileService_
#define _FileService_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#include <queue>
#else
#include "WProgram.h"
#endif

#include <SPI.h>
#include <SdFat.h>

namespace Services
{
	class FileService
	{
    private:
        SdFat _sd;

		bool createDirectory(const char* directory);

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
	};
}
#endif