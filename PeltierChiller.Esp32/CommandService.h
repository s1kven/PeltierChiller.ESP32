#pragma once

#include "BaseCommand.h"

namespace Services
{
	class CommandService
	{
	private:

	public:
		void handleCommand(Models::Abstractions::BaseCommand* command);
	};
}