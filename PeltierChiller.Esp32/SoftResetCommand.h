#pragma once
#ifndef _SoftResetCommand_
#define _SoftResetCommand_ 

#include "Esp.h"
#include "BaseDeserializableObject.h"
#include "BaseCommand.h"

namespace Commands
{
	class SoftResetCommand :
		public ::Models::Abstractions::BaseCommand
	{
	protected:
		void init() override;

	public:
		SoftResetCommand();
		void invoke() override;
		void clear() override;
	};
}
#endif