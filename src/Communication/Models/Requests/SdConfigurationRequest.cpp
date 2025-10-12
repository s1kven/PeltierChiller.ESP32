#include "SdConfigurationRequest.h"
#include "Services/ConfigurationService.h"

Communication::Models::Requests::SdConfigurationRequest::SdConfigurationRequest()
	: Communication::Models::Requests::BaseRequest(Communication::Enums::RequestType::sdConfiguration)
{
}

void Communication::Models::Requests::SdConfigurationRequest::init()
{
}

void Communication::Models::Requests::SdConfigurationRequest::invoke()
{
	Communication::Models::Configurations::Configuration* configuration = _configurationService->getSdConfiguration();
	Communication::Models::Responses::ResponsesData::ConfigurationData* responseData = 
		new Communication::Models::Responses::ResponsesData::ConfigurationData(configuration);
	Communication::Enums::ResponseType responseType = Communication::Enums::ResponseType::sdConfiguration;
	Communication::Models::Responses::Response* sdConfigResponse = new Communication::Models::Responses::Response(
					responseType, true,
					responseData, "");
	_communicationService->sendResponse(sdConfigResponse);
}

void Communication::Models::Requests::SdConfigurationRequest::clear()
{
}

bool Communication::Models::Requests::SdConfigurationRequest::isIndependentToConfig()
{
	return true;
}
