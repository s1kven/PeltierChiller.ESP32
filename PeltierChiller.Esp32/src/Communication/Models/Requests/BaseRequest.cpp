#include "BaseRequest.h"

Communication::Models::Requests::BaseRequest::BaseRequest(Communication::Enums::RequestType commandType)
{
	_commandType = commandType;
}

void Communication::Models::Requests::BaseRequest::init()
{
}

Communication::Enums::RequestType Communication::Models::Requests::BaseRequest::getCommandType()
{
	return _commandType;
}
