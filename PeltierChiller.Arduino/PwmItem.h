#pragma once

#include <LinkedList.h>
#include "Arduino.h"
#include "PwmValue.h"
#include "PwmType.cpp"

namespace Models
{
	class PwmItem
	{
	private:

		uint8_t _tachoPin; 
		uint8_t _pwmPin; 
		String _name;
		LinkedList<Models::PwmValue*>* _values;
		Models::Enums::PwmType _controlType;
		uint16_t _rpm;
		uint16_t _count = 0; 
		uint32_t _rpmTimer;

		void init();
		static void interruptHandler(void* ptr);
		void counter();

	protected:

	public:

		PwmItem(uint8_t tachoPin, uint8_t pwmPin, String name, LinkedList<Models::PwmValue*>* values, Models::Enums::PwmType controlType);
		uint8_t getTachoPin();
		uint8_t getPwmPin();
		String getName();
		LinkedList<Models::PwmValue*>* getValues();
		Models::Enums::PwmType getControlType();
		uint16_t getRpm(uint16_t getRpmDelay);
	};
}