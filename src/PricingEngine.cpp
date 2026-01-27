#include "PricingEngine.h"
#include <unordered_map>
#include <algorithm>

using namespace std;

static const int BASE_PRICE = 500;

// in-memory tracking (Week 8 only)
static unordered_map<string, int> ticketsBought;
static unordered_map<string, int> lastGroupDiscount;

int PricingEngine::calculateFinalPrice(int seatCount, const string& userKey) {

    int baseTotal = seatCount * BASE_PRICE;

    // ===== GROUP DISCOUNT (PRIORITY) =====
    if (seatCount >= 2) {
        int discount = 0;

        if (seatCount == 2) discount = 10;
        else if (seatCount == 3) discount = 15;
        else discount = min(seatCount * 5, 50);

        // double only for group 2 or 3
        if (seatCount <= 3 && lastGroupDiscount[userKey] == discount) {
            discount = min(discount * 2, 50);
        }

        lastGroupDiscount[userKey] = discount;
        ticketsBought[userKey] += seatCount;

        return baseTotal - (baseTotal * discount / 100);
    }

    // ===== INDIVIDUAL DISCOUNT =====
    int bought = ticketsBought[userKey];
    int price = BASE_PRICE;

    if (bought == 2) price = BASE_PRICE / 2;        // 50%
    else if (bought == 3) price = BASE_PRICE / 4;   // 75%
    else if (bought >= 4) price = 0;                // FREE

    ticketsBought[userKey] += 1;
    return price;
}
