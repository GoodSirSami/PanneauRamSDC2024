#ifndef TCONFIGURATION_HPP
#define TCONFIGURATION_HPP

#include <iostream>
#include <fstream>

class TConfiguration
{
private:
    std::string fileName;
    std::ifstream file;

public:
    TConfiguration(std::string fileName);
    ~TConfiguration();

    std::string getConfiguration(void);
};

#endif // TCONFIGURATION_HPP