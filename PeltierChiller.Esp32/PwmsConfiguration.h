#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <LinkedList.h>
#include "BaseDeserializableObject.h"
#include "PwmConfiguration.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations
		{
			class PwmsConfiguration :
				public Abstractions::BaseDeserializableObject
			{
			private:
				uint16_t _updatePwmTimer;
				LinkedList<PwmConfiguration*>* _pwmsListConfiguration;

			protected:
				void init() override;

			public:
				~PwmsConfiguration();
				void init(LinkedList<PwmConfiguration*>* pwmsListConfiguration);

				LinkedList<PwmConfiguration*>* getPwmsListConfiguration();
			};
		}
	}
}