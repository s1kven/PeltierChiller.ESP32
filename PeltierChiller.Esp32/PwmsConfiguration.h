#pragma once
#ifndef _PwmsConfiguration_
#define _PwmsConfiguration_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "BaseDeserializableObject.h"
#include "BaseSerializableObject.h"
#include "PwmConfiguration.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			class PwmsConfiguration :
				public Abstractions::BaseDeserializableObject,
				public Abstractions::BaseSerializableObject
			{
			private:
				const uint16_t _payloadSize = JSON_OBJECT_SIZE(1);
				LinkedList<PwmConfiguration*>* _pwmsListConfiguration;

			protected:
				void init() override;

			public:
				void init(LinkedList<PwmConfiguration*>* pwmsListConfiguration);
				void clear();

				LinkedList<PwmConfiguration*>* getPwmsListConfiguration();

				DynamicJsonDocument createPayload() override;
			};
		}
	}
}
#endif