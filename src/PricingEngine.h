#ifndef PRICING_ENGINE_H
#define PRICING_ENGINE_H

#include <string>

class PricingEngine {
public:
    static int calculateFinalPrice(int seatCount, const std::string& userKey);
};

#endif
