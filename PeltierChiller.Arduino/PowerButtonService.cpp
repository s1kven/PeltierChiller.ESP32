#include "PowerButtonService.h"

Services::PowerButtonService::PowerButtonService(Models::Button* buttons[])
{
	Serial.println("PowerButtonService start");
	_buttons = new LinkedList<Models::Button*>();
	for (uint8_t index = 0; index < sizeof(buttons); index++)
	{
		(*_buttons).add(buttons[index]);
	}
	Serial.println("PowerButtonService end");
}

LinkedList<Models::Button*>* Services::PowerButtonService::getButtons()
{
	Serial.println("getButtons");
	return _buttons;
}

void Services::PowerButtonService::handleButtonsPress()
{
	Serial.println("handle");
	for (uint8_t index = 0; index < sizeof(*_buttons); index++)
	{
		Serial.println("handle1");
		(*_buttons).get(index)->setState(digitalRead((*_buttons).get(index)->getSignalPin()));
		if ((*_buttons).get(index)->getState() && !(*_buttons).get(index)->getFlag())
		{
			(*_buttons).get(index)->setFlag(true);
			(*_buttons).get(index)->setLastPressTime(millis());
		}
		else if (!(*_buttons).get(index)->getState() && (*_buttons).get(index)->getFlag() && millis() - (*_buttons).get(index)->getLastPressTime() > 20)
		{
			(*_buttons).get(index)->setFlag(false);
			(*_buttons).get(index)->setLastPressMillis(millis() - (*_buttons).get(index)->getLastPressTime());
			(*_buttons).get(index)->setLastTime(millis());
			Serial.print("pressMillis = ");
			Serial.println((*_buttons).get(index)->getLastPressMillis());
		}
	}
}
