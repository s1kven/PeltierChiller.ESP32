#pragma once

#include "arduino.h"
#include "Rele.h"

namespace Models
{
	class PeltierModule : public Models::Rele
	{
	private:


	public:
		PeltierModule(int8_t _peltierPin);
		void switchingOn(); 
		void switchingOff();
	};
}