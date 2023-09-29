#pragma once

#include <vector>
#include <string>

namespace Xeph2D::Utility
{
	inline std::vector<const char*> CStrVect(const std::vector<std::string>& input)
	{
		std::vector<const char*> inputCStr;
		for (const std::string& item : input)
		{
			inputCStr.push_back(item.c_str());
		}
		return inputCStr;
	}
}