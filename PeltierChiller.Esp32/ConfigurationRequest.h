#pragma once
#ifndef _ConfigurationRequest_
#define _ConfigurationRequest_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "BaseRequest.h"
#include "Configuration.h"

namespace Communication
{
	namespace Models
	{
		namespace Requests
		{
			class ConfigurationRequest :
				public Communication::Models::Requests::BaseRequest
			{
			private:
				Communication::Models::Configurations::Configuration* _configuration;

			protected:
				void init() override;

			public:
				ConfigurationRequest(Communication::Models::Configurations::Configuration* configuration);
				void invoke() override;
				void clear() override;
				Communication::Models::Configurations::Configuration* getConfiguration();
			};
		}
	}
}
#endif