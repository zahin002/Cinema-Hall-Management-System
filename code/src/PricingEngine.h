#ifndef PRICINGENGINE_H
#define PRICINGENGINE_H

#include <string>

using namespace std;

class PricingEngine {
public:

    // Base ticket price
    static const int BASE_PRICE = 500;

    static int getGroupDiscountPercent(int seatCount);
    
    /*
     * Calculates final price based on number of seats.
     * Applies group discount rules only.
     */

    static int calculateFinalPrice(int seatCount);

    static bool hasGlobalDiscount();
    static int getGlobalDiscountPercent();
    static string getGlobalDiscountMessage();

    static void setGlobalDiscount(int percent, const string& msg);
    static void removeGlobalDiscount();

   
};

#endif
