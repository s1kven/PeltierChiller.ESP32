#include "CurrentConfigurationRequest.h"
#include "Services/JsonService.h"
#include "Services/ConfigurationService.h"

Communication::Models::Requests::CurrentConfigurationRequest::CurrentConfigurationRequest()
	: Communication::Models::Requests::BaseRequest(Communication::Enums::RequestType::currentConfiguration)
{
}

void Communication::Models::Requests::CurrentConfigurationRequest::init()
{
}

void Communication::Models::Requests::CurrentConfigurationRequest::invoke()
{
	Communication::Models::Configurations::Configuration* currentConfiguration = _configurationService->getConfiguration();
    Communication::Models::Configurations::Configuration* configuration = _jsonService->getConfigurationDeepCopy(currentConfiguration);
	Communication::Models::Responses::ResponsesData::ConfigurationData* responseData = 
		new Communication::Models::Responses::ResponsesData::ConfigurationData(configuration);
	Communication::Enums::ResponseType responseType = Communication::Enums::ResponseType::currentConfiguration;
	Communication::Models::Responses::Response* sdConfigResponse = new Communication::Models::Responses::Response(
					responseType, true,
					responseData, "");
	_communicationService->sendResponse(sdConfigResponse);
}

void Communication::Models::Requests::CurrentConfigurationRequest::clear()
{
}

bool Communication::Models::Requests::CurrentConfigurationRequest::isIndependentToConfig()
{
    return true;
}
