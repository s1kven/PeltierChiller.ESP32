#include "FileService.h"

void Services::FileService::init(uint8_t cs)
{
	SD.begin(cs);
}

String Services::FileService::readFile(const char* path)
{
	String content = "";
	File file = SD.open(path);
	if (file) 
	{
		content = file.readString();
		file.close();
	}
	return content;
}

uint32_t Services::FileService::appendFile(const char* path, String content)
{
	uint32_t bytesWrite = 0;
	File file = SD.open(path, FILE_APPEND);
	if (file)
	{
		bytesWrite = file.println(content);
		file.close();
	}
	return bytesWrite;
}

uint32_t Services::FileService::writeFile(const char* path, String content)
{
	uint32_t bytesWrite = 0;
	File file = SD.open(path, FILE_WRITE);
	if (file)
	{
		bytesWrite = file.println(content);
		file.close();
	}
	return bytesWrite;
}
