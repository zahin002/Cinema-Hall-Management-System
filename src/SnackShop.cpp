#include "SnackShop.h"
#include <iostream>
#include <iomanip>

using namespace std;

void SnackShop::start() {

    int choice;
    double total = 0;

    cout << "\n========== 🍿 SNACK SHOP ==========\n";

    while (true) {
        cout << "\n1. Burger\n";
        cout << "2. Chicken Sandwich\n";
        cout << "3. Popcorn\n";
        cout << "4. Cola\n";
        cout << "0. Finish Order\n";
        cout << "Select item: ";
        cin >> choice;

        if (choice == 0) break;

        int size;
        double price = 0;

        switch (choice) {

        case 1: { // Burger
            cout << "1. Chicken Burger (Medium)\n";
            cout << "2. Beef Burger (Medium)\n";
            cout << "Choose: ";
            cin >> size;

            price = (size == 1) ? 250 : 320;
            break;
        }

        case 2: { // Sandwich
            cout << "1. Small\n2. Medium\n3. Large\nChoose: ";
            cin >> size;

            if (size == 1) price = 180;
            else if (size == 2) price = 220;
            else price = 260;
            break;
        }

        case 3: { // Popcorn
            cout << "1. Small (100)\n2. Medium (150)\n3. Large (200)\nChoose: ";
            cin >> size;

            if (size == 1) price = 100;
            else if (size == 2) price = 150;
            else price = 200;
            break;
        }

        case 4: { // Cola
            cout << "1. Small\n2. Medium\n3. Large\nChoose: ";
            cin >> size;

            if (size == 1) price = 60;
            else if (size == 2) price = 80;
            else price = 100;
            break;
        }

        default:
            cout << "Invalid choice!\n";
            continue;
        }

        total += price;
        cout << "Added: " << price << " Tk\n";
    }

    if (total == 0) {
        cout << "\nNo snacks ordered.\n";
        return;
    }

    double vat = total * 0.03;
    double grandTotal = total + vat;

    cout << "\n========== 🧾 SNACK INVOICE ==========\n";
    cout << fixed << setprecision(2);
    cout << "Subtotal : " << total << " Tk\n";
    cout << "VAT (3%) : " << vat << " Tk\n";
    cout << "TOTAL    : " << grandTotal << " Tk\n";
    cout << "=====================================\n";
}
