#pragma once

#include <Windows.h>

namespace MegaEngine
{
	namespace Core
	{
		namespace Initialization
		{
			bool IsOnlyInstance(LPCTSTR gameTitle);
			bool CheckStorage(const DWORDLONG diskSpaceNeeded);
			bool CheckMemory(const DWORDLONG ramNeeded, const DWORDLONG virtualRamNeeded);

			DWORD ReadCPUSpeed();
		}
	}
}