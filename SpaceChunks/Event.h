#pragma once

#include "Header.h"

namespace XyEngine
{
	class Event
	{
	private:
		MSG			m_pMessage;
	public:
		Event();
		~Event();

		// Getters
		MSG			GetMsg() { return this->m_pMessage; }
		bool		PollMsg();
	};
}

