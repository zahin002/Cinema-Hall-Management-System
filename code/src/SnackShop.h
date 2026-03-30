#ifndef SNACKSHOP_H
#define SNACKSHOP_H

#include <string>
#include <vector>

using namespace std;

struct SnackItem {
    string name;
    int price;
};

class SnackShop {
public:
    static void start();          // Entry point
private:
    vector<SnackItem> cart;

    void showMenu();
    void addBurger();
    void addSandwich();
    void addPopcorn();
    void addCola();

    int calculateSubtotal() const;
    void printInvoice(int subtotal, int vat, int total);
};

#endif
