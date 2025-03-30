#pragma once
#ifndef _WifiConfiguration_
#define _WifiConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Communication/Abstractions/BaseDeserializableObject.h"
#include "Communication/Abstractions/BaseSerializableObject.h"
#include "Helpers/JsonHelper.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations 
		{
			class WifiConfiguration :
				public Abstractions::BaseDeserializableObject,
				public Abstractions::BaseSerializableObject
			{
			private:
				const uint16_t _payloadSize = JSON_OBJECT_SIZE(4);
                String _ssid;
                String _password;
				uint32_t _reconnectionTimeout;
				String _ntpServer;

			protected:
				void init() override;

			public:
				void init(String ssid, String password, uint32_t reconnectionTimeout, String ntpServer);

				String getSsid();
                String getPassword();
                uint32_t getReconnectionTimeout();
                String getNtpServer();

				DynamicJsonDocument createPayload() override;
			};
		}
	}
}
#endif