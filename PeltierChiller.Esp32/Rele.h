#pragma once
#ifndef _Rele_
#define _Rele_ 

#include "ReleState.cpp"
#include "Arduino.h"
#include <stdint.h>
#include "BaseSerializableObject.h"

namespace Models
{
	class Rele :
		public Communication::Abstractions::BaseSerializableObject
	{
	private:
		const uint16_t _payloadSize = JSON_OBJECT_SIZE(2);

		int8_t _signalPin;
		Models::Enums::ReleState _state;

	protected:

		void setReleState(Models::Enums::ReleState _releState);

	public:

		Rele();
		Rele(int8_t _relePin);
		int8_t getSignalPin();
		Models::Enums::ReleState getReleState();
		DynamicJsonDocument createPayload() override;
	};
}
#endif