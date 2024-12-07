#pragma once

#include <LinkedList.h>
#include "PwmsConfiguration.h"
#include "PwmItem.h"
#include "TemperatureService.h"

namespace Services
{
	class PwmService
	{
	private:

		uint32_t _updatePwmDelay;
		LinkedList<Models::PwmItem*>* _pwmItems;

		Communication::Models::Configurations::PwmsConfiguration* _configuration;
		Services::TemperatureService* _temperatureService;

		void initConfiguration();
		float getCurrentHotToRoomDeltaT();
		uint8_t getCurrentPwmValue(Models::PwmItem* currentItem, float deltaT);
		uint8_t convertPercentageToPwm(uint8_t percentageLoad);

	public:
		PwmService(uint32_t updatePwmDelay, Communication::Models::Configurations::PwmsConfiguration* configuration,
			Services::TemperatureService* temperatureService);
		void handlePwms(uint8_t currentChillerLoad);
		LinkedList<Models::PwmItem*>* getPwmItems();
		uint32_t getUpdatePwmDelay();
	};
}