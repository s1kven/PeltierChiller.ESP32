#include "ChillerService.h"

Services::ChillerService::ChillerService(uint8_t _temperatureSensorsPin, DeviceAddress _tSensors[], float _targetCircuitTemperature)
{
	_temperatureService = new Services::TemperatureService(_temperatureSensorsPin, _tSensors, _targetCircuitTemperature);
	Serial.println();
}

void Services::ChillerService::handleChillerState()
{
	//Serial.println((*_temperatureService).getColdCircuitTemperature());
	/*Serial.print((*_temperatureService).getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget::none));
	Serial.println((*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::none));

	Serial.print((*_temperatureService).getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget::room));
	Serial.println((*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::room));

	Serial.print((*_temperatureService).getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget::pcCase));
	Serial.println((*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::pcCase));

	Serial.print((*_temperatureService).getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget::coldCircuit));
	Serial.println((*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::coldCircuit));

	Serial.print((*_temperatureService).getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget::hotCircuit));
	Serial.println((*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::hotCircuit));*/

	(*_temperatureService).getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::coldCircuit);

}
