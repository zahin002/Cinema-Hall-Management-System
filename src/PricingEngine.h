#ifndef PRICINGENGINE_H
#define PRICINGENGINE_H

#include <string>

using namespace std;

/*
 * Handles ticket pricing and group-based discounts.
 * Base price is fixed at 500 Tk per ticket.
 */

class PricingEngine {
public:
    // Base ticket price
    static const int BASE_PRICE = 500;

    /*
     * Calculates final price based on number of seats.
     * Applies group discount rules only.
     */
    static int calculateFinalPrice(int seatCount);

    /*
     * Returns discount percentage based on group size.
     * Used for displaying discount info to the user.
     */
    static int getGroupDiscountPercent(int seatCount);
};

#endif
