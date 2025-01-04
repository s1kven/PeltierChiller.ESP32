#pragma once
#ifndef _BaseResponseData_
#define _BaseResponseData_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "../../../Abstractions/BaseSerializableObject.h"

namespace Communication
{
	namespace Models
	{
		namespace Responses
		{
			namespace ResponsesData
			{
				class BaseResponseData
					: public Communication::Abstractions::BaseSerializableObject
				{
				public:
					virtual DynamicJsonDocument createPayload() = 0;
					virtual void clear() = 0;
				};
			}
		}
	}
}
#endif