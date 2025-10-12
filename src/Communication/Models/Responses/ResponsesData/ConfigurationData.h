#pragma once

#ifndef _ConfigurationData_
#define _ConfigurationData_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "BaseResponseData.h"

#include "../../Configurations/Configuration.h"

namespace Communication
{
	namespace Models
	{
		namespace Responses
		{
			namespace ResponsesData
			{
				class ConfigurationData
					: public BaseResponseData
				{
				private:
					const uint16_t _payloadSize = JSON_OBJECT_SIZE(1);
				    Communication::Models::Configurations::Configuration* _configuration;

				public:
                    ConfigurationData(Communication::Models::Configurations::Configuration* configuration);
					DynamicJsonDocument createPayload() override;
					void clear() override;
				};
			}
		}
	}
}
#endif