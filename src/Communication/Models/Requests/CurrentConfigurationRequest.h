#pragma once

#ifndef _CurrentConfigurationRequest_
#define _CurrentConfigurationRequest_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "BaseRequest.h"
#include "../Configurations/Configuration.h"
#include "../../Services/CommunicationService.h"
#include "../Responses/ResponsesData/ConfigurationData.h"

namespace Services {
	class JsonService;
	class ConfigurationService;
}

extern Services::ConfigurationService* _configurationService;
extern Services::JsonService* _jsonService;
extern Communication::Services::CommunicationService* _communicationService;

namespace Communication
{
	namespace Models
	{
		namespace Requests
		{
			class CurrentConfigurationRequest :
				public Communication::Models::Requests::BaseRequest
			{
			private:

			protected:
				void init() override;

			public:
                CurrentConfigurationRequest();
				void invoke() override;
				void clear() override;
				bool isIndependentToConfig() override;
			};
		}
	}
}
#endif