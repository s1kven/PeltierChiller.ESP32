#pragma once

#include "ReleState.cpp"
#include "Arduino.h"
#include <stdint.h>

namespace Models
{
	class Rele
	{
	private:

		int8_t _signalPin;
		Models::Enums::ReleState _state;

	protected:

		void setReleState(Models::Enums::ReleState _releState);

	public:

		Rele();
		Rele(int8_t _relePin);
		int8_t getSignalPin();
		Models::Enums::ReleState getReleState();
	};
}