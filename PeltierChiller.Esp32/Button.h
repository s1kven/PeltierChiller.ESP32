#pragma once

#include "Arduino.h"
#include <stdint.h>

namespace Models
{
	class Button
	{
	private:

		uint8_t _signalPin;
		String _name;
		uint32_t _lastPressTime = 0;
		uint32_t _lastPressMillis = 0;
		uint32_t _lastTime = 0;
		bool _pressed = false;
		bool _flag = false;

	protected:

	public:

		Button(uint8_t signalPin, String name);
		uint8_t getSignalPin();
		String getName();
		void setLastPressTime(uint32_t lastPressTime);
		uint32_t getLastPressTime();
		void setLastPressMillis(uint32_t lastPressMillis);
		uint32_t getLastPressMillis();
		void setLastTime(uint32_t lastTime);
		uint32_t getLastTime();
		bool getState();
		void setState(bool pressed);
		bool getFlag();
		void setFlag(bool flag);
	};
}