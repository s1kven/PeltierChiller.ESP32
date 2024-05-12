#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <FS.h>
#include <SD.h>

namespace Services
{
	class FileService
	{
	public:
		void init(uint8_t cs);

		String readFile(const char* path);
	};
}