#pragma once
#ifndef _PinsConfiguration_
#define _PinsConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "BaseDeserializableObject.h"
#include "BaseSerializableObject.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations 
		{
			class PinsConfiguration :
				public Abstractions::BaseDeserializableObject,
				public Abstractions::BaseSerializableObject
			{
			private:
				const uint16_t _payloadSize = JSON_OBJECT_SIZE(5);
				uint8_t _powerButton;
				uint8_t _powerSignal;
				uint8_t _chillerPsSignal;
				uint8_t _chillerSignal;
				uint8_t _pcVoltage;

			protected:
				void init() override;

			public:
				void init(uint8_t powerButton, uint8_t powerSignal,
					uint8_t chillerPsSignal, uint8_t chillerSignal, uint8_t pcVoltage);

				uint8_t getPowerButton();
				uint8_t getPowerSignal();
				uint8_t getChillerPsSignal();
				uint8_t getChillerSignal();
				uint8_t getPcVoltage();

				DynamicJsonDocument createPayload() override;
			};
		}
	}
}
#endif

