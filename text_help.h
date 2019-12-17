#pragma once

#include <string>

std::string ExtractString(const char* message, const char* searchStr);
bool ExtractNumber(const char* message, const char* searchStr, int& catNum);
void TrimString(std::string& jsonStr);

