#pragma once
#include "Button.h"
#include <LinkedList.h>


namespace Services
{
	class PowerButtonService
	{
	private:
		LinkedList<Models::Button*>* _buttons;

	protected:


	public:

		PowerButtonService(Models::Button* buttons[]);
		LinkedList<Models::Button*>* getButtons();
		void handleButtonsPress();
	};
}