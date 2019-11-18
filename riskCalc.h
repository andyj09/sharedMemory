#ifndef RISK_CALC_H
#define RISK_CALC_H
#include <string>
#include <vector>

namespace riskCalc
{
    void Init(std::vector<std::string>&);
    void Init_Consumer();
    std::vector<std::string> GetInstrumentVec();
}
#endif // RISK_CALC_H