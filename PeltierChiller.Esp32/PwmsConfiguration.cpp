#include "PwmsConfiguration.h"

Communication::Models::Configurations::PwmsConfiguration::~PwmsConfiguration()
{
	delete _pwmsListConfiguration;
}

void Communication::Models::Configurations::PwmsConfiguration::init()
{
}

void Communication::Models::Configurations::PwmsConfiguration::init(LinkedList<PwmConfiguration*>* pwmsListConfiguration)
{
	_pwmsListConfiguration = pwmsListConfiguration;
}

LinkedList<Communication::Models::Configurations::PwmConfiguration*>*
Communication::Models::Configurations::PwmsConfiguration::getPwmsListConfiguration()
{
	return _pwmsListConfiguration;
}
