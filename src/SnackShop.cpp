#include "SnackShop.h"
#include "TerminalColors.h"
#include <iostream>
#include <iomanip>

using namespace std;

void SnackShop::start() {

    int choice;
    double total = 0;

    cout << BOLD << CYAN
         << "\n========== SNACK SHOP ==========\n"
         << RESET;

    while (true) {

        cout << "\n" << BOLD << "Menu:\n" << RESET;

        cout << GREEN << "1." << RESET << " Burger\n";
        cout << GREEN << "2." << RESET << " Chicken Sandwich\n";
        cout << GREEN << "3." << RESET << " Popcorn\n";
        cout << GREEN << "4." << RESET << " Cola\n";
        cout << RED   << "0." << RESET << " Finish Order\n";

        cout << BOLD << YELLOW << "Select item: " << RESET;
        cin >> choice;

        if (choice == 0) break;

        int size;
        double price = 0;

        switch (choice) {

        case 1: { // Burger
            cout << CYAN << "\n-- Burger Menu --\n" << RESET;
            cout << "1. Chicken Burger (250 Tk)\n";
            cout << "2. Beef Burger (320 Tk)\n";
            cout << "Choose: ";
            cin >> size;

            if (size == 1) price = 250;
            else if (size == 2) price = 320;
            else {
                cout << RED << "Invalid option!\n" << RESET;
                continue;
            }
            break;
        }

        case 2: { // Sandwich
            cout << CYAN << "\n-- Sandwich Sizes --\n" << RESET;
            cout << "1. Small (180 Tk)\n";
            cout << "2. Medium (220 Tk)\n";
            cout << "3. Large (260 Tk)\n";
            cout << "Choose: ";
            cin >> size;

            if (size == 1) price = 180;
            else if (size == 2) price = 220;
            else if (size == 3) price = 260;
            else {
                cout << RED << "Invalid option!\n" << RESET;
                continue;
            }
            break;
        }

        case 3: { // Popcorn
            cout << CYAN << "\n-- Popcorn Sizes --\n" << RESET;
            cout << "1. Small (100 Tk)\n";
            cout << "2. Medium (150 Tk)\n";
            cout << "3. Large (200 Tk)\n";
            cout << "Choose: ";
            cin >> size;

            if (size == 1) price = 100;
            else if (size == 2) price = 150;
            else if (size == 3) price = 200;
            else {
                cout << RED << "Invalid option!\n" << RESET;
                continue;
            }
            break;
        }

        case 4: { // Cola
            cout << CYAN << "\n-- Cola Sizes --\n" << RESET;
            cout << "1. Small (60 Tk)\n";
            cout << "2. Medium (80 Tk)\n";
            cout << "3. Large (100 Tk)\n";
            cout << "Choose: ";
            cin >> size;

            if (size == 1) price = 60;
            else if (size == 2) price = 80;
            else if (size == 3) price = 100;
            else {
                cout << RED << "Invalid option!\n" << RESET;
                continue;
            }
            break;
        }

        default:
            cout << RED << "Invalid choice!\n" << RESET;
            continue;
        }

        total += price;

        cout << GREEN << "✔ Added: " << price << " Tk\n" << RESET;
    }

    if (total == 0) {
        cout << YELLOW << "\nNo snacks ordered.\n" << RESET;
        return;
    }

    double vat = total * 0.03;
    double grandTotal = total + vat;

    cout << "\n" << BOLD << CYAN
         << "========== SNACK INVOICE ==========\n"
         << RESET;

    cout << fixed << setprecision(2);

    cout << "Subtotal : " << GREEN << total << " Tk\n" << RESET;
    cout << "VAT (3%) : " << YELLOW << vat << " Tk\n" << RESET;
    cout << BOLD << "TOTAL    : " << CYAN << grandTotal << " Tk\n" << RESET;

    cout << BOLD << CYAN
         << "===================================\n"
         << RESET;
}