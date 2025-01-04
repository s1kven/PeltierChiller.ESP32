#pragma once
#ifndef _GetPwmItemsData_
#define _GetPwmItemsData_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "BaseResponseData.h"
#include "Models/PwmItem.h"

namespace Communication
{
	namespace Models
	{
		namespace Responses
		{
			namespace ResponsesData
			{
				class GetPwmItemsData
					: public BaseResponseData
				{
				private:
					const uint16_t _payloadSize = JSON_OBJECT_SIZE(1);
					LinkedList<::Models::PwmItem*>* _pwmItems;

				public:
					GetPwmItemsData(LinkedList<::Models::PwmItem*>* pwmItems);
					DynamicJsonDocument createPayload() override;
					void clear() override;
				};
			}
		}
	}
}
#endif