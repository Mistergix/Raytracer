#include "JsonReader.h"
#include <fstream>
#include <sstream>

json JsonReader::GetJsonFile(const std::string& filepath)
{
    std::ifstream stream(filepath);
    json j  = json::parse(stream);
    return j;
}
