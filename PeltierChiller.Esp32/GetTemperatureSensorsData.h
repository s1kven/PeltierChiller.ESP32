#pragma once
#ifndef _GetTemperatureSensorsData_
#define _GetTemperatureSensorsData_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "BaseResponseData.h"
#include "BaseSensor.h"

namespace Communication
{
	namespace Models
	{
		namespace Responses
		{
			namespace ResponsesData
			{
				class GetTemperatureSensorsData
					: public BaseResponseData
				{
				private:
					const uint16_t _payloadSize = JSON_OBJECT_SIZE(1);
					LinkedList<::Models::TemperatureSensors::BaseSensor*>* _temperatureSensors;

				public:
					GetTemperatureSensorsData(LinkedList<::Models::TemperatureSensors::BaseSensor*>* temperatureSensors);
					DynamicJsonDocument createPayload() override;
					void clear() override;
				};
			}
		}
	}
}
#endif