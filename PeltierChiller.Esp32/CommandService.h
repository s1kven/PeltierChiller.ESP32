#pragma once
#ifndef _CommandService_
#define _CommandService_ 

#include "BaseCommand.h"
#include "SoftResetCommand.h"
#include "UpdateConfigurationCommand.h"

namespace Services
{
	class CommandService
	{
	private:

	public:
		void handleCommand(Models::Abstractions::BaseCommand* command);
	};
}
#endif