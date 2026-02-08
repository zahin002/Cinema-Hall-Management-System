#include "PricingEngine.h"
#include <fstream>

/* ================= GROUP DISCOUNT ================= */

int PricingEngine::getGroupDiscountPercent(int seatCount) {
    if (seatCount == 2) return 10;
    if (seatCount == 3) return 15;
    if (seatCount >= 4) return min(20 + (seatCount - 4) * 5, 50);
    return 0;
}

int PricingEngine::calculateFinalPrice(int seatCount) {
    int base = seatCount * BASE_PRICE;
    int percent = getGroupDiscountPercent(seatCount);
    return base - (base * percent / 100);
}

/* ================= GLOBAL DISCOUNT ================= */

bool PricingEngine::hasGlobalDiscount() {
    ifstream file("../data/global_discount.txt");
    return file.good();
}

int PricingEngine::getGlobalDiscountPercent() {
    ifstream file("../data/global_discount.txt");
    int percent;
    char sep;
    if (file >> percent >> sep)
        return percent;
    return 0;
}

string PricingEngine::getGlobalDiscountMessage() {
    ifstream file("../data/global_discount.txt");
    int percent;
    char sep;
    string msg;
    if (file >> percent >> sep) {
        getline(file, msg);
        return msg;
    }
    return "";
}