#pragma once
#ifndef _PwmService_
#define _PwmService_ 

#include <LinkedList.h>
#include "Communication/Models/Configurations/PwmsConfiguration.h"
#include "Models/PwmItem.h"
#include "TemperatureService.h"

extern Services::TemperatureService* _temperatureService;

namespace Services
{
	class PwmService
	{
	private:

		uint32_t _updatePwmDelay;
		LinkedList<Models::PwmItem*>* _pwmItems;

		Communication::Models::Configurations::PwmsConfiguration* _configuration;

		void initConfiguration();
		float getCurrentHotToRoomDeltaT();
		uint8_t getCurrentPwmValue(Models::PwmItem* currentItem, float deltaT);
		uint8_t convertPercentageToPwm(uint8_t percentageLoad);

	public:
<<<<<<<< HEAD:PeltierChiller.Esp32/PwmService.h
		PwmService(uint32_t updatePwmDelay, Communication::Models::Configurations::PwmsConfiguration* configuration,
			Services::TemperatureService* temperatureService);
========
		PwmService(uint32_t updatePwmDelay, Communication::Models::Configurations::PwmsConfiguration* configuration);
>>>>>>>> develop:src/Services/PwmService.h
		void handlePwms(uint8_t currentChillerLoad);
		LinkedList<Models::PwmItem*>* getPwmItems();
		uint32_t getUpdatePwmDelay();
		void clear();
	};
}
#endif