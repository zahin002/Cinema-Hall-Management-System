#include "SnackShop.h"
#include "TerminalColors.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

using namespace std;

struct Item {
    string name;
    int qty;
    double price;
};

void SnackShop::start() {

    int choice;
    double total = 0;
    vector<Item> cart;

    cout << "\n" << BOLD << CYAN
         << "========================================\n"
         << "             SNACK SHOP\n"
         << "========================================\n"
         << RESET;

    while (true) {

        cout << "\n" << BOLD << WHITE << "MENU\n" << RESET;
        cout << CYAN << "----------------------------------------\n" << RESET;

        cout << GREEN << "[1]" << RESET << " Burger\n";
        cout << GREEN << "[2]" << RESET << " Chicken Sandwich\n";
        cout << GREEN << "[3]" << RESET << " Popcorn\n";
        cout << GREEN << "[4]" << RESET << " Cola\n";
        cout << GREEN << "[5]" << RESET << " French Fries\n";
        cout << RED   << "[0]" << RESET << " Finish Order\n";

        cout << CYAN << "----------------------------------------\n" << RESET;

        cout << BOLD << YELLOW << "Select item: " << RESET;

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input!\n" << RESET;
            continue;
        }

        if (choice == 0) break;

        int size, qty;
        double price = 0;
        string itemName;

        switch (choice) {

        case 1: {
            cout << CYAN << "\nBurger Options\n" << RESET;
            cout << "1. Chicken Burger (250 Tk)\n";
            cout << "2. Beef Burger (320 Tk)\n";
            cout << YELLOW << "Choose: " << RESET;
            cin >> size;

            if (size == 1) { price = 250; itemName = "Chicken Burger"; }
            else if (size == 2) { price = 320; itemName = "Beef Burger"; }
            else { cout << RED << "Invalid option!\n" << RESET; continue; }

            break;
        }

        case 2: {
            cout << CYAN << "\nSandwich Sizes\n" << RESET;
            cout << "1. Small (180 Tk)\n2. Medium (220 Tk)\n3. Large (260 Tk)\n";
            cout << YELLOW << "Choose: " << RESET;
            cin >> size;

            if (size == 1) { price = 180; itemName = "Sandwich (Small)"; }
            else if (size == 2) { price = 220; itemName = "Sandwich (Medium)"; }
            else if (size == 3) { price = 260; itemName = "Sandwich (Large)"; }
            else { cout << RED << "Invalid option!\n" << RESET; continue; }

            break;
        }

        case 3: {
            cout << CYAN << "\nPopcorn Sizes\n" << RESET;
            cout << "1. Small (100 Tk)\n2. Medium (150 Tk)\n3. Large (200 Tk)\n";
            cout << YELLOW << "Choose: " << RESET;
            cin >> size;

            if (size == 1) { price = 100; itemName = "Popcorn (Small)"; }
            else if (size == 2) { price = 150; itemName = "Popcorn (Medium)"; }
            else if (size == 3) { price = 200; itemName = "Popcorn (Large)"; }
            else { cout << RED << "Invalid option!\n" << RESET; continue; }

            break;
        }

        case 4: {
            cout << CYAN << "\nCola Sizes\n" << RESET;
            cout << "1. Small (60 Tk)\n2. Medium (80 Tk)\n3. Large (100 Tk)\n";
            cout << YELLOW << "Choose: " << RESET;
            cin >> size;

            if (size == 1) { price = 60; itemName = "Cola (Small)"; }
            else if (size == 2) { price = 80; itemName = "Cola (Medium)"; }
            else if (size == 3) { price = 100; itemName = "Cola (Large)"; }
            else { cout << RED << "Invalid option!\n" << RESET; continue; }

            break;
        }

        case 5: {
            cout << CYAN << "\nFrench Fries Sizes\n" << RESET;
            cout << "1. Small (50 Tk)\n2. Medium (80 Tk)\n3. Large (120 Tk)\n";
            cout << YELLOW << "Choose: " << RESET;
            cin >> size;

            if (size == 1) { price = 50; itemName = "Fries (Small)"; }
            else if (size == 2) { price = 80; itemName = "Fries (Medium)"; }
            else if (size == 3) { price = 120; itemName = "Fries (Large)"; }
            else { cout << RED << "Invalid option!\n" << RESET; continue; }

            break;
        }

        default:
            cout << RED << "Invalid choice!\n" << RESET;
            continue;
        }

        cout << YELLOW << "Enter quantity: " << RESET;
        cin >> qty;

        if (qty <= 0) {
            cout << RED << "Invalid quantity!\n" << RESET;
            continue;
        }

        double cost = price * qty;
        total += cost;

        cart.push_back({itemName, qty, cost});

        cout << GREEN << "Added: " << itemName
             << " x" << qty << " = " << cost << " Tk\n" << RESET;
    }

    if (total == 0) {
        cout << YELLOW << "\nNo snacks ordered.\n" << RESET;
        return;
    }

    double vat = total * 0.03;
    double grandTotal = total + vat;

    cout << "\n" << BOLD << CYAN
         << "========================================\n"
         << "             SNACK INVOICE\n"
         << "========================================\n"
         << RESET;

    cout << BOLD << WHITE << "Items:\n" << RESET;

    for (const auto& item : cart) {
        cout << " - " << item.name
             << " x" << item.qty
             << " = " << GREEN << item.price << " Tk\n" << RESET;
    }

    cout << CYAN << "----------------------------------------\n" << RESET;

    cout << "Subtotal : " << GREEN << total << " Tk\n" << RESET;
    cout << "VAT (3%) : " << YELLOW << vat << " Tk\n" << RESET;

    cout << BOLD << CYAN
         << "TOTAL    : " << grandTotal << " Tk\n"
         << "========================================\n"
         << RESET;
}