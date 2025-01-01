#include "PwmService.h"

Services::PwmService::PwmService(uint32_t updatePwmDelay, Communication::Models::Configurations::PwmsConfiguration* configuration)
{
	_updatePwmDelay = updatePwmDelay;
	_configuration = configuration;
	_pwmItems = new LinkedList<Models::PwmItem*>();
	initConfiguration();
}

void Services::PwmService::handlePwms(uint8_t currentChillerLoad)
{
	float hotToRoomDeltaT = getCurrentHotToRoomDeltaT();
	for (uint8_t i = 0; i < _pwmItems->size(); i++)
	{
		Models::PwmItem* currentItem = _pwmItems->get(i);
		if (currentItem->getSetToMaxWhenChillerLoad() != -1 && currentChillerLoad > currentItem->getSetToMaxWhenChillerLoad())
		{
			analogWrite(currentItem->getPwmPin(), 255);
		}
		else
		{
			analogWrite(currentItem->getPwmPin(), getCurrentPwmValue(currentItem, hotToRoomDeltaT));
		}
	}
}

LinkedList<Models::PwmItem*>* Services::PwmService::getPwmItems()
{
	return _pwmItems;
}

uint32_t Services::PwmService::getUpdatePwmDelay()
{
	return _updatePwmDelay;
}

void Services::PwmService::clear()
{
	for (int i = 0; i < _pwmItems->size(); i++)
	{
		_pwmItems->get(i)->clear();
		delete _pwmItems->get(i);
	}
	_pwmItems->clear();
	delete _pwmItems;
}

void Services::PwmService::initConfiguration()
{
	LinkedList<Communication::Models::Configurations::PwmConfiguration*>* pwmConfigurations = _configuration->getPwmsListConfiguration();
	for (uint8_t i = 0; i < pwmConfigurations->size(); i++)
	{
		Communication::Models::Configurations::PwmConfiguration* currentItemConfiguration = pwmConfigurations->get(i);
		LinkedList<Models::PwmValue*>* pwmValues =
			new LinkedList<Models::PwmValue*>();
		for (uint8_t i = 0; i < currentItemConfiguration->getPwmValues()->size(); i++)
		{
			pwmValues->add(new Models::PwmValue(
				currentItemConfiguration->getPwmValues()->get(i)->getTemperature(),
				currentItemConfiguration->getPwmValues()->get(i)->getLoad()));
		}
		Models::PwmItem* currentItem = new Models::PwmItem(
			currentItemConfiguration->getTachoPin(),
			currentItemConfiguration->getPwmPin(),
			currentItemConfiguration->getName(),
			currentItemConfiguration->getSetToMaxWhenChillerLoad(),
			pwmValues,
			currentItemConfiguration->getPwmType(),
			_updatePwmDelay);
		_pwmItems->add(currentItem);
	}
}

float Services::PwmService::getCurrentHotToRoomDeltaT()
{
	return _temperatureService->getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::hotCircuit) -
		_temperatureService->getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::room);
}

uint8_t Services::PwmService::getCurrentPwmValue(Models::PwmItem* currentItem, float deltaT)
{
	if (currentItem->getControlType() == Models::Enums::PwmType::unknown || isnan(deltaT))
	{
		return 255;
	}

	if (currentItem->getValues()->size() == 0)
	{
		// disable fans if "values" list empty.
		if (currentItem->getControlType() == Models::Enums::PwmType::fan)
		{
			return 0;
		}

		return 255;
	}

	if (deltaT < currentItem->getValues()->get(0)->getTemperature())
	{
		// disable fan if temperature less then first value.
		if (currentItem->getControlType() == Models::Enums::PwmType::fan)
		{
			return 0;
		}

		// set pump to lowest value 
		return convertPercentageToPwm(currentItem->getValues()->get(0)->getLoad());
	}

	// set to max available value
	if (deltaT > currentItem->getValues()->get(currentItem->getValues()->size() - 1)->getTemperature())
	{
		return convertPercentageToPwm(currentItem->getValues()->get(currentItem->getValues()->size() - 1)->getLoad());
	}

	Models::PwmValue* firsPoint;
	Models::PwmValue* secondPoint;
	for (int i = 0; i < currentItem->getValues()->size() - 1; i++)
	{
		if (deltaT >= currentItem->getValues()->get(i)->getTemperature() &&
			deltaT <= currentItem->getValues()->get(i + 1)->getTemperature())
		{
			firsPoint = currentItem->getValues()->get(i);
			secondPoint = currentItem->getValues()->get(i + 1);
			break;
		}
	}

	//y = kx + b   y == load in percentage, x == deltaT
	float k = (float)(firsPoint->getLoad() - secondPoint->getLoad()) / 
		(float)(firsPoint->getTemperature() - secondPoint->getTemperature());
	float b = (float)firsPoint->getLoad() - (float)(k * firsPoint->getTemperature());
	uint8_t currentLoad = (uint8_t)((k * deltaT) + b);

	return convertPercentageToPwm(currentLoad);
}

uint8_t Services::PwmService::convertPercentageToPwm(uint8_t percentageLoad)
{
	float fraction = (float)percentageLoad / 100.0;
	return 255 * fraction;
}
