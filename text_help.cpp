#include "text_help.h"

#include <cctype>
#include <string>


std::string ExtractString(const char* message, const char* searchStr)
{
	const char* subMsg = strstr(message, searchStr);
	if (subMsg == nullptr)
		return false;
	subMsg = strstr(subMsg, ":");
	if (subMsg == nullptr)
		return false;
	subMsg += 1;

	const char* endPos = strstr(subMsg, ",");
	if (endPos == nullptr)
		return false;

	std::string result(subMsg, endPos);
	return result;
}

bool ExtractNumber(const char* message, const char* searchStr, int& catNum)
{
	auto msg = ExtractString(message, searchStr);
	if (msg.empty()) {
		return false;

	}

	catNum = atoi(msg.c_str());
	return true;
}

void TrimString(std::string& jsonStr)
{
	for (auto c = jsonStr.back(); std::isspace(c) || c == '\"'; ) {
		jsonStr.pop_back();
		c = jsonStr.back();
	}

	int i = 0;
	char chr = jsonStr[i];
	while (std::isspace(chr) || chr == '\"') {
		chr = jsonStr[++i];
	}

	jsonStr = jsonStr.substr(i);
}
