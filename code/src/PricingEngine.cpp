#include "PricingEngine.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>   // for min(), max()
#include <cstdio>      // for remove()

using namespace std;

/* ================= GROUP DISCOUNT ================= */

int PricingEngine::getGroupDiscountPercent(int seatCount) {

    if (seatCount == 2) return 10;
    if (seatCount == 3) return 15;

    if (seatCount >= 4)
        return min(20 + (seatCount - 4) * 5, 50);

    return 0;
}

/* ================= FINAL PRICE ================= */

int PricingEngine::calculateFinalPrice(int seatCount) {

    int base = seatCount * BASE_PRICE;

    // GLOBAL DISCOUNT OVERRIDES GROUP DISCOUNT
    if (hasGlobalDiscount()) {
        int globalPercent = getGlobalDiscountPercent();

        // Safety clamp (0%–100%)
        globalPercent = max(0, min(globalPercent, 100));

        return base - (base * globalPercent / 100);
    }

    // Otherwise apply group discount
    int groupPercent = getGroupDiscountPercent(seatCount);
    return base - (base * groupPercent / 100);
}

/* ================= GLOBAL DISCOUNT ================= */

// Check if global discount file exists AND not empty
bool PricingEngine::hasGlobalDiscount() {

    ifstream file("../data/global_discount.txt");

    return file.is_open() && file.peek() != EOF;
}

// Get discount percent from file
int PricingEngine::getGlobalDiscountPercent() {

    ifstream file("../data/global_discount.txt");

    if (!file.is_open())
        return 0;

    string line;
    if (!getline(file, line))
        return 0;

    stringstream ss(line);
    string percentStr;

    getline(ss, percentStr, '|');

    try {
        return stoi(percentStr);
    } catch (...) {
        return 0;
    }
}

// Get discount message
string PricingEngine::getGlobalDiscountMessage() {

    ifstream file("../data/global_discount.txt");

    if (!file.is_open())
        return "";

    string line;
    if (!getline(file, line))
        return "";

    stringstream ss(line);
    string percentStr, msg;

    getline(ss, percentStr, '|');
    getline(ss, msg);

    return msg;
}

// Admin: Set global discount
void PricingEngine::setGlobalDiscount(int percent, const string& msg) {

    ofstream file("../data/global_discount.txt");

    if (!file.is_open()) {
        cout << "Error saving global discount.\n";
        return;
    }

    // Safety clamp before saving
    percent = max(0, min(percent, 100));

    file << percent << "|" << msg;
    file.close();
}

// Admin: Remove global discount
void PricingEngine::removeGlobalDiscount() {

    if (remove("../data/global_discount.txt") != 0) {
        cout << "No global discount to remove.\n";
    }
}