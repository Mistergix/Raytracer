#include "JsonReader.h"
#include <fstream>
#include <sstream>

json JsonReader::GetJsonFile(const std::string& filepath)
{
	std::ifstream stream(filepath);

    std::string line;
    std::stringstream ss;
    while (getline(stream, line)) {
        ss << line << "\n";
    }

    return ss.str();
}
