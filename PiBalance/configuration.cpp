#include "configuration.hpp"

TConfiguration::TConfiguration(std::string fileName)
{
    this->fileName = fileName;
}

TConfiguration::~TConfiguration()
{
    file.close();
}

std::string TConfiguration::getConfiguration(void)
{
    std::string line;
    std::string configuration = "";
    file.open(fileName);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            configuration += line;
        }
        file.close();
    }
    return configuration;
}