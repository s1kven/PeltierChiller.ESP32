#pragma once

#ifndef _UpdateTempConfigurationCommand_
#define _UpdateTempConfigurationCommand_ 

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "../../../Abstractions/BaseDeserializableObject.h"
#include "../BaseRequest.h"
#include "../../Configurations/Configuration.h"
#include "../../../Services/CommunicationService.h"

namespace Services {
	class ConfigurationService;
}

extern Services::ConfigurationService* _configurationService;
extern Communication::Services::CommunicationService* _communicationService;

namespace Communication
{
	namespace Models
	{
		namespace Requests
		{
			namespace Commands
			{
				class UpdateTempConfigurationCommand :
					public Communication::Models::Requests::BaseRequest
				{
				private:
					Communication::Models::Configurations::Configuration* _configuration;

				protected:
					void init() override;

				public:
					void invoke() override;
					void clear() override;
					UpdateTempConfigurationCommand(Communication::Models::Configurations::Configuration* configuration);
				};
			}
		}
	}
}
#endif
