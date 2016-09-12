//SOURCE: https://github.com/TeamHypersomnia/Augmentations/blob/master/augs/templates.cpp

#include "templates.h"

std::wstring to_wstring(std::string val) {
	return std::wstring(val.begin(), val.end());
}

template <>
std::string to_string(std::wstring val) {
	return std::string(val.begin(), val.end());
}