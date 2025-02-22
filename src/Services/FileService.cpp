#include "FileService.h"

bool Services::FileService::createDirectory(const char *directory)
{
	if (_sd.exists(directory) || (!_sd.exists(directory) && _sd.mkdir(directory))) 
	{
		return true;
	}
	return false;
}

LinkedList<Helpers::FileInfo>* Services::FileService::getFilesOlderThanDate(String directory, struct tm selectedDate)
{
	LinkedList<Helpers::FileInfo>* files = new LinkedList<Helpers::FileInfo>();

	FsFile fatDirectory = _sd.open(directory);
	if (!fatDirectory)
	{
		return files;
	}

	SdFile file;
	while (file.openNext(&fatDirectory, O_RDONLY)) 
	{
		if (!file.isHidden() && file.isFile()) 
		{
			uint16_t date, time;
			if(file.getCreateDateTime(&date, &time))
			{
				struct tm creationDate;
				_timeService->convertToTmStruct(date, time, creationDate);
				time_t selectedDateTime = mktime(&selectedDate);
				time_t creationDateTime = mktime(&creationDate);
				double timeDifference = difftime(creationDateTime, selectedDateTime);
				if(timeDifference < 0)
				{
					Helpers::FileInfo info;
					char nameBuffer[100];
					file.getName(nameBuffer, sizeof(nameBuffer));
					info.name = "/" + String(nameBuffer);
					info.size = file.fileSize();
					info.creationTime = creationDate;
					files->add(info);
				}
			}
		}
		file.close();
	  }

	return files;
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
    FsFile file;
    if(file.open(fileName, O_RDWR | O_CREAT | O_AT_END | O_TRUNC))
    {
        file.close();
        return true;
    }
    return false;
}

bool Services::FileService::createFile(const char *path, const char *fileName)
{
    return createDirectory(path) && _sd.open(path, O_DIRECTORY) && createFile((String(path) + String(fileName)).c_str());
}

bool Services::FileService::setFileTimestamp(const char* fileName, tm time)
{
	SdFile file;
	if (file.open(fileName, O_WRITE)) 
	{
		file.timestamp(T_CREATE, (uint16_t)time.tm_year + 1900, (uint8_t)time.tm_mon + 1, (uint8_t)time.tm_mday, (uint8_t)time.tm_hour, (uint8_t)time.tm_min, (uint8_t)time.tm_sec);
		file.timestamp(T_WRITE, (uint16_t)time.tm_year + 1900, (uint8_t)time.tm_mon + 1, (uint8_t)time.tm_mday, (uint8_t)time.tm_hour, (uint8_t)time.tm_min, (uint8_t)time.tm_sec);
		file.close();
		return true;
	}
	return false;
}

void Services::FileService::deleteFilesOlderThanDate(String directory, tm selectedDate, String ignoreFiles [], uint16_t ignoreFilesCount)
{
	LinkedList<Helpers::FileInfo>* filesForDelete = getFilesOlderThanDate(directory, selectedDate);

	FsFile fatDirectory = _sd.open(directory);
	if(fatDirectory && filesForDelete->size() > 0)
	{
		for(int i = 0; i < filesForDelete->size(); i++)
		{
			if(_sd.exists(directory + filesForDelete->get(i).name))
			{
				bool isIgnoredFile = false;
				for(int j = 0; j < ignoreFilesCount; j++)
				{
					if(ignoreFiles[j] == filesForDelete->get(i).name)
					{
						isIgnoredFile = true;
					}
				}
				if(!isIgnoredFile)
				{
					_sd.remove(directory + filesForDelete->get(i).name);
				}
			}
		}
		fatDirectory.close();
	}
	filesForDelete->clear();
	delete filesForDelete;
}
