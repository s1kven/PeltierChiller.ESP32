#pragma once

#ifndef _ResetTempConfigurationCommand_
#define _ResetTempConfigurationCommand_ 

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "BaseDeserializableObject.h"
#include "BaseRequest.h"
#include "Configuration.h"
#include "CommunicationService.h"

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
				class ResetTempConfigurationCommand :
					public Communication::Models::Requests::BaseRequest
				{
				protected:
					void init() override;

				public:
					void invoke() override;
					void clear() override;
					ResetTempConfigurationCommand();
				};
			}
		}
	}
}
#endif
