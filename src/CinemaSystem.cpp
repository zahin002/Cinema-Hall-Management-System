#include <iostream>
#include <vector>
#include <sstream>     // Used for parsing multiple seat inputs
#include <set>         // Used to prevent duplicate seat selection
#include "CinemaSystem.h"
#include "FileManager.h"
#include "Movie.h"
#include "AdminService.h"
#include "UserService.h"

using namespace std;

/*
 * Controls the overall application lifecycle.
 * Displays welcome message once, then repeatedly shows
 * the main menu until the user chooses to exit.
 */
void CinemaSystem::run() {
    showWelcome();

    int choice;
    do {
        showMainMenu();
        cout << "Enter choice: ";
        cin >> choice;

        /*
         * Menu-driven control using switch-case ensures
         * clean separation of authentication actions.
         */
        switch (choice) {
            case 1: login(); break;
            case 2: signup(); break;
            case 3: guestLogin(); break;
            case 4: cout << "Exiting CINE++...\n"; break;
            default: cout << "Invalid choice.\n";
        }
        cout << endl;
    } while (choice != 4);
}

/*
 * Displays the system banner.
 * This function is purely presentational and does not
 * interact with any application state.
 */
void CinemaSystem::showWelcome() {
    cout << "=====================================\n";
    cout << "        WELCOME TO CINE++             \n";
    cout << "   Cinema Hall Management System      \n";
    cout << "=====================================\n\n";
}

/*
 * Displays the main menu options available
 * before authentication.
 */
void CinemaSystem::showMainMenu() {
    cout << "1. Login\n";
    cout << "2. Signup\n";
    cout << "3. Guest Booking\n";
    cout << "4. Exit\n";
}


/*
 * Registers a new user by storing encrypted credentials.
 * Password encryption ensures raw passwords are never saved.
 */
void CinemaSystem::signup() {
    string email, password;
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;

    string encrypted = User::encryptPassword(password);
    FileManager::saveUser(User(email, encrypted, "user"));

    cout << "Signup successful!\n";
}

/*
 * Authenticates user credentials by comparing encrypted values.
 * Redirects user to admin or user menu based on role.
 */

void CinemaSystem::login() {
    string email, password;
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;

    string encrypted = User::encryptPassword(password);
    vector<User> users = FileManager::loadUsers();

    for (const User& u : users) {
        if (u.getEmail() == email && u.getPassword() == encrypted) {

            AdminService adminService;
            UserService userService;

            if (u.getRole() == "admin")
                adminService.adminMenu(u);
            else
                userService.userMenu(u);

            return;
        }
    }
    cout << "Invalid email or password.\n";
}

/* Guest login using Bangladeshi phone number validation.*/

void CinemaSystem::guestLogin() {
    string rest;
    cout << "Enter phone number: +880";
    cin >> rest;

    string phone = "+880" + rest;

    if (!isValidBangladeshPhone(phone)) {
        cout << "Please input valid phone number.\n";
        return;
    }

    cout << "Guest login successful!\n";

    User guest(phone, "", "guest");
    UserService userService;
    userService.userMenu(guest);
}


bool CinemaSystem::isValidBangladeshPhone(const string& phone) {
    if (phone.length() != 14) return false;

    if (phone.substr(0, 4) != "+880") return false;
    if (phone[4] != '1') return false;

    char simDigit = phone[5];
    if (simDigit < '3' || simDigit > '9') return false;

    for (int i = 6; i < 14; i++) {
        if (!isdigit(phone[i])) return false;
    }

    return true;
}





