#pragma once
#ifndef _BaseRequest_
#define _BaseRequest_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "BaseDeserializableObject.h"
#include "RequestType.cpp"

namespace Communication
{
	namespace Models
	{
		namespace Requests
		{
			class BaseRequest : public Communication::Abstractions::BaseDeserializableObject
			{
			private:
				Communication::Enums::RequestType _commandType;

			protected:
				BaseRequest(Communication::Enums::RequestType commandType);
				void init() override;

			public:
				virtual void invoke() = 0;
				virtual void clear() = 0;
				Communication::Enums::RequestType getCommandType();
			};
		}
	}
}
#endif