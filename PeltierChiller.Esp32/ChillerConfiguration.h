#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "BaseDeserializableObject.h"

namespace Communication
{
	namespace Models
	{
		namespace Configurations 
		{
			class ChillerConfiguration :
				public Abstractions::BaseDeserializableObject
			{
			private:
				uint16_t _potentiometerAddress;
				uint16_t _maxPotentiometerValue;
				uint16_t _minPotentiometerValue;
				float _kp;
				float _ki;
				float _kd;
				float _dt;
				float _pidRatio;
				float _minIntegral;
				float _maxIntegral;
				uint32_t _computePidDelay;

			protected:
				void init() override;

			public:
				void init(uint16_t potentiometerAddress, uint16_t maxPotentiometerValue, uint16_t minPotentiometerValue,
					float kp, float ki, float kd, float dt, float pidRatio, float minIntegral, float maxIntegral,
					uint32_t computePidDelay);


				uint16_t getPotentiometerAddress();
				uint16_t getMaxPotentiometerValue();
				uint16_t getMinPotentiometerValue();
				float getKp();
				float getKi();
				float getKd();
				float getDt();
				float getPidRatio();
				float getMinIntegral();
				float getMaxIntegral();
				uint32_t getComputePidDelay();
			};
		}
	}
}