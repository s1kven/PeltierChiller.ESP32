#pragma once
#include "BaseDeserializableObject.h"

namespace Communication
{
	namespace Models
	{
		class ChillerConfiguration :
			public Abstractions::BaseDeserializableObject
		{
		private:
			float _targetCircuitTemperature;

		protected:
			void init() override;

		public:
			ChillerConfiguration(float targetCircuitTemperature);

			float getTargetCircuitTemperature();
		};
	}
}