#pragma once

#ifndef _UpdateConfigurationCommand_
#define _UpdateConfigurationCommand_ 

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "../../../Abstractions/BaseDeserializableObject.h"
#include "../BaseRequest.h"
#include "../../../Models/Configurations/Configuration.h"
#include "../../../Services/CommunicationService.h"
#include "Services/FileService.h"

namespace Services {
	class JsonService;
	class ConfigurationService;
	class FileService;
}

extern Services::ConfigurationService* _configurationService;
extern Services::JsonService* _jsonService;
extern Communication::Services::CommunicationService* _communicationService;
extern Services::FileService* _fileService;

namespace Communication
{
	namespace Models
	{
		namespace Requests
		{
			namespace Commands
			{
				class UpdateConfigurationCommand :
					public Communication::Models::Requests::BaseRequest
				{
				private:
					const char* _failedWriteConfigMessage = "Failed write configuration to SD.";
					const char* _failedResetConfigFromSdMessage = "Failed reset configuration from SD while update.";
					Communication::Models::Configurations::Configuration* _configuration;

				protected:
					void init() override;

				public:
					void invoke() override;
					void clear() override;
					UpdateConfigurationCommand(Communication::Models::Configurations::Configuration* configuration,
						Communication::Enums::RequestType request);
				};
			}
		}
	}
}
#endif
