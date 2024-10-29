#include "TemperatureSensorsConfiguration.h"

void Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::init()
{
}

Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::~TemperatureSensorsConfiguration()
{
	delete _bmeListConfiguration;
	delete _ntcListConfiguration;
	delete _ds18b20ListConfiguration;
}

void Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::init(
	Bme280ListConfiguration* bmeListConfiguration, NtcListConfiguration* ntcListConfiguration, 
	Ds18b20ListConfiguration* ds18b20ListConfiguration)
{
	_bmeListConfiguration = bmeListConfiguration;
	_ntcListConfiguration = ntcListConfiguration;
	_ds18b20ListConfiguration = ds18b20ListConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::Bme280ListConfiguration* 
Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::getBmeListConfiguration()
{
	return _bmeListConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::NtcListConfiguration*
Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::getNtcListConfiguration()
{
	return _ntcListConfiguration;
}

Communication::Models::Configurations::TemperatureSensors::Ds18b20ListConfiguration*
Communication::Models::Configurations::TemperatureSensors::TemperatureSensorsConfiguration::getDs18b20ListConfiguration()
{
	return _ds18b20ListConfiguration;
}
