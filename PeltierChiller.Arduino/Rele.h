#pragma once

#include "ReleState.cpp"
#include "Arduino.h"
#include <stdint.h>
#include "BaseJsonModel.h"

namespace Models
{
	class Rele :
		public Models::Abstractions::BaseJsonModel
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