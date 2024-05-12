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
