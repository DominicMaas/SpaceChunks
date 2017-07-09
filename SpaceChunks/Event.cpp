#include "Event.h"

namespace XyEngine
{
	Event::Event()
	{
	}


	Event::~Event()
	{
	}

	bool Event::PollMsg()
	{
		if (PeekMessage(&m_pMessage, NULL, NULL, NULL, PM_REMOVE) > 0)
		{
			TranslateMessage(&m_pMessage);
			DispatchMessage(&m_pMessage);

			return true;
		}
		else
		{
			return false;
		}
	}
}
