#include "ConfigurationRequest.h"

Communication::Models::Requests::ConfigurationRequest::ConfigurationRequest(
	Communication::Models::Configurations::Configuration* configuration) 
	: Communication::Models::Requests::BaseRequest(Communication::Enums::RequestType::configuration)
{
	_configuration = configuration;
}

void Communication::Models::Requests::ConfigurationRequest::init()
{
}

void Communication::Models::Requests::ConfigurationRequest::invoke()
{
}

void Communication::Models::Requests::ConfigurationRequest::clear()
{
}

Communication::Models::Configurations::Configuration* Communication::Models::Requests::ConfigurationRequest::getConfiguration()
{
	return _configuration;
}
