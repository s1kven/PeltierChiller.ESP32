#pragma once
#ifndef _PwmConfiguration_
#define _PwmConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "Communication/Abstractions/BaseDeserializableObject.h"
#include "Communication/Abstractions/BaseSerializableObject.h"
#include "PwmValueConfiguration.h"
#include "Models/Enums/PwmType.cpp"

using namespace Models::Enums;

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			class PwmConfiguration :
				public Abstractions::BaseDeserializableObject,
				public Abstractions::BaseSerializableObject
			{
			private:
				const uint16_t _payloadSize = JSON_OBJECT_SIZE(6);
				uint8_t _tachoPin;
				uint8_t _pwmPin;
				String _name;
				int8_t _setToMaxWhenChillerLoad;
				LinkedList<Communication::Models::Configurations::PwmValueConfiguration*>* _pwmValues;
				PwmType _type;

			protected:
				void init() override;

			public:
				void init(uint8_t tachoPin, uint8_t pwmPin, String name, int8_t setToMaxWhenChillerLoad,
					LinkedList<Communication::Models::Configurations::PwmValueConfiguration*>* pwmValues, PwmType type);
				void clear();

				uint8_t getTachoPin();
				uint8_t getPwmPin();
				String getName();
				int8_t getSetToMaxWhenChillerLoad();
				LinkedList<Communication::Models::Configurations::PwmValueConfiguration*>* getPwmValues();
				PwmType getPwmType();

				DynamicJsonDocument createPayload() override;
			};
		}
	}
}
#endif