#include <iostream>
#include "riskCalc.h"
#include "util.hpp"

int main()
{
    std::cout << "Prog: Main.cpp \n";

    static const int NUM_INSTRUMENTS = 5000;
    std::vector<std::string> instrumentList;
    for(unsigned int i = 0; i < NUM_INSTRUMENTS; i++)
    {
        instrumentList.emplace_back(random_string(64));
    }

    riskCalc::Init(instrumentList);
    std::vector<std::string> instIdList = riskCalc::GetInstrumentVec();
    std::cout << "InstIDList size: " << instIdList.size() << "\n";

    size_t n = 10;
    unsigned int maxIdx = std::min(n, instIdList.size());
    for(unsigned int i=0; i < maxIdx; i++)
    {
        std::cout << "i: " << instIdList[i] << "\n";
    }
}