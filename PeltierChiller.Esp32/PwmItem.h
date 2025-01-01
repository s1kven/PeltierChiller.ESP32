#pragma once
#ifndef _PwmItem_
#define _PwmItem_ 

#include <LinkedList.h>
#include "Arduino.h"
#include "PwmValue.h"
#include "PwmType.cpp"
#include "BaseSerializableObject.h"

namespace Models
{
	class PwmItem :
		public Communication::Abstractions::BaseSerializableObject
	{
	private:
		const uint16_t _payloadSize = JSON_OBJECT_SIZE(3);

		uint8_t _tachoPin; 
		uint8_t _pwmPin; 
		String _name;
		int8_t _setToMaxWhenChillerLoad;
		LinkedList<Models::PwmValue*>* _values;
		Models::Enums::PwmType _controlType;
		uint16_t _rpm;
		uint16_t _count = 0; 
		uint32_t _rpmTimer;
		uint32_t _updatePwmDelay;

		void init();
		static void interruptHandler(void* ptr);
		void counter();

	protected:

	public:

		PwmItem(uint8_t tachoPin, uint8_t pwmPin, String name, int8_t setToMaxWhenChillerLoad, 
			LinkedList<Models::PwmValue*>* values, Models::Enums::PwmType controlType, uint32_t updatePwmDelay);
		uint8_t getTachoPin();
		uint8_t getPwmPin();
		String getName();
		int8_t getSetToMaxWhenChillerLoad();
		LinkedList<Models::PwmValue*>* getValues();
		Models::Enums::PwmType getControlType();
		uint16_t getRpm();
		DynamicJsonDocument createPayload() override;

		void clear();
	};
}
#endif