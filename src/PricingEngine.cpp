#include "PricingEngine.h"

/*
 * Returns discount percentage based on group size.
 */
int PricingEngine::getGroupDiscountPercent(int seatCount) {

    if (seatCount < 2)
        return 0;

    if (seatCount == 2)
        return 10;

    if (seatCount == 3)
        return 15;

    if (seatCount >= 4) {
        int discount = seatCount * 5;   // 4→20, 5→25, ...
        if (discount > 50)
            discount = 50;              // Cap at 50%
        return discount;
    }

    return 0;
}

/*
 * Calculates final price after applying group discount.
 */
int PricingEngine::calculateFinalPrice(int seatCount) {

    int baseTotal = seatCount * BASE_PRICE;
    int discountPercent = getGroupDiscountPercent(seatCount);

    int discountAmount = (baseTotal * discountPercent) / 100;
    int finalPrice = baseTotal - discountAmount;

    return finalPrice;
}
