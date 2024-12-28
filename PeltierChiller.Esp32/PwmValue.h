#pragma once
#ifndef _PwmValue_
#define _PwmValue_ 

#include "Arduino.h"

namespace Models
{
	class PwmValue
	{
	private:

		uint8_t _load; // 0-100%
		float _temperature;

	protected:

	public:

		PwmValue(float temperature, uint8_t load);
		float getTemperature();
		uint8_t getLoad();
	};
}
#endif