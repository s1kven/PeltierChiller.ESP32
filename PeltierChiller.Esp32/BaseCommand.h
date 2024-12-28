#pragma once
#ifndef _BaseCommand_
#define _BaseCommand_ 

#include "BaseDeserializableObject.h"
#include "RequestType.cpp"

namespace Models
{
	namespace Abstractions
	{
		class BaseCommand : public Communication::Abstractions::BaseDeserializableObject
		{
		private:
			Communication::Enums::RequestType _commandType;

		protected:
			BaseCommand(Communication::Enums::RequestType commandType);
			void init() override;

		public:
			virtual void invoke() = 0;
			virtual void clear() = 0;
			virtual void test();
			Communication::Enums::RequestType getCommandType();
		};
	}
}
#endif