#pragma once

#ifndef _SdConfigurationRequest_
#define _SdConfigurationRequest_ 
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
			class SdConfigurationRequest :
				public Communication::Models::Requests::BaseRequest
			{
			private:

			protected:
				void init() override;

			public:
                SdConfigurationRequest();
				void invoke() override;
				void clear() override;
				bool isIndependentToConfig() override;
			};
		}
	}
}
#endif