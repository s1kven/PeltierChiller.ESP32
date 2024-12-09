#include "ConfigurationError.h"

Communication::Models::Errors::ConfigurationError::ConfigurationError(Communication::Enums::ErrorCode code, const char* errorMessage, String invalidConfiguration) :
	Communication::Abstractions::BaseError(Communication::Enums::ResponseType::errorConfiguration, code, getErrorMessageByCode(code), invalidConfiguration)
{
}

void Communication::Models::Errors::ConfigurationError::init()
{
}

DynamicJsonDocument Communication::Models::Errors::ConfigurationError::createPayload()
{
	return Communication::Abstractions::BaseError::createPayload();
}

const char* Communication::Models::Errors::ConfigurationError::getErrorMessageByCode(Communication::Enums::ErrorCode code)
{
	switch (code)
	{
	case Communication::Enums::ErrorCode::emptySensors:
		return "Failed to configure chiller. Empty temperature sensors list! Add at least one sensor to cold circuit.";

	case Communication::Enums::ErrorCode::emptyColdCircuitSensors:
		return "Failed to configure chiller. No temperature sensor was found for the cold circuit.";

	case Communication::Enums::ErrorCode::emptyRoomSensors:
		return "Failed to configure chiller. No temperature sensor was found for the room. To use delta temperature chiller type (ChillerType: 2), you need to connect at least one room sensor.";

	case Communication::Enums::ErrorCode::invalidBmeConfiguration:
		return "Failed to configure chiller. No BME280 temperature sensor was found for the room. To use dew point chiller type (ChillerType: 3), you need to connect at least one room BME280 sensor.";

	default:
		return "";
	}
}
