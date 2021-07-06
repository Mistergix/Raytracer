#pragma once
#include "json.hpp"

using json = nlohmann::json;

class JsonReader {
public:
	json GetJsonFile(const std::string& filepath);
};