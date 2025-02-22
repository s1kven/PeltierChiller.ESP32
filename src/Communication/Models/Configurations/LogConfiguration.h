#pragma once
#ifndef _LogConfiguration_
#define _LogConfiguration_ 
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
			class LogConfiguration :
				public Abstractions::BaseDeserializableObject,
				public Abstractions::BaseSerializableObject
			{
			private:
				const uint16_t _payloadSize = JSON_OBJECT_SIZE(4);
				bool _isEnabled;
				uint32_t _logDelay;
                uint16_t _logExpiration;
                String _createNewLogTime;

			protected:
				void init() override;

			public:
				void init(bool isEnabled, uint32_t logDelay, uint16_t logExpiration, String createNewLogTime);
                
                bool getIsEnabled();
                uint32_t getLogDelay();
                uint16_t getLogExpiration();
                String getCreateNewLogTime();

				DynamicJsonDocument createPayload() override;
			};
		}
	}
}
#endif