#include "FileService.h"

bool Services::FileService::createDirectory(const char *directory)
{
	if (_sd.exists(directory) || (!_sd.exists(directory) && _sd.mkdir(directory))) 
	{
		return true;
	}
	return false;
}

bool Services::FileService::init(uint8_t cs)
{
	return _sd.begin(cs, SD_SCK_MHZ(16));
}

String Services::FileService::readFile(const char* path)
{
	String content = "";
    SdFile file;
	if (file.open(path, O_READ)) 
	{
		while (file.available()) 
        {
            content += (char)file.read();
        }
        file.close();
	}
	return content;
}

uint32_t Services::FileService::appendFile(const char *fileName, String content)
{
	uint32_t bytesWrite = 0;
	SdFile file;
	if (file.open(fileName, O_RDWR | O_AT_END))
	{
		bytesWrite = file.println(content);
		file.close();
	}
	return bytesWrite;
}

uint32_t Services::FileService::appendFile(const char *path, const char *fileName, String content)
{
	if(createDirectory(path) && _sd.open(path, O_DIRECTORY))
	{
		return appendFile((String(path) + String(fileName)).c_str(), content);
	}
	return 0;
}

uint32_t Services::FileService::appendFile(const char *path, const char *fileName, std::queue<String> &content)
{
	if(createDirectory(path) && _sd.open(path, O_DIRECTORY))
	{
		uint32_t bytesWrite = 0;
		SdFile file;
		if (file.open((String(path) + String(fileName)).c_str(), O_RDWR | O_AT_END))
		{
			bytesWrite = 0;
			while(!content.empty())
			{
				bytesWrite += file.println(content.front());
				content.pop();
			}
			file.close();
		}
		return bytesWrite;
	}
	return 0;
}

uint32_t Services::FileService::writeFile(const char *fileName, String content)
{
    uint32_t writeBytes = 0;
    FsFile file = _sd.open(fileName, O_RDWR | O_CREAT | O_TRUNC);
    if (file) 
    {
        writeBytes = file.println(content);
        file.close();
    }
    return writeBytes;
}

uint32_t Services::FileService::writeFile(const char *path, const char *fileName, String content)
{
	if(createDirectory(path) && _sd.open(path, O_DIRECTORY))
	{
		return writeFile((String(path) + String(fileName)).c_str(), content);
	}
	return 0;
}

bool Services::FileService::createFile(const char *fileName)
{
	Serial.println(fileName);
    FsFile file;
    if(file.open(fileName, O_RDWR | O_CREAT | O_AT_END | O_TRUNC))
    {
        file.close();
		Serial.println("created");
        return true;
    }
	Serial.println("!created");
    return false;
}

bool Services::FileService::createFile(const char *path, const char *fileName)
{
    return createDirectory(path) && _sd.open(path, O_DIRECTORY) && createFile((String(path) + String(fileName)).c_str());
}
