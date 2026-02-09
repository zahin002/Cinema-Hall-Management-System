#include <iostream>
#include <conio.h>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>     // Used for parsing multiple seat inputs
#include <set>         // Used to prevent duplicate seat selection
#include "CinemaSystem.h"
#include "FileManager.h"
#include "Movie.h"
#include "AdminService.h"
#include "UserService.h"
#include "MenuService.h"

using namespace std;

const string ADMIN_SECRET_CODE = "ADMIN2026";


/*
 * Controls the overall application lifecycle.
 * Displays welcome message once, then repeatedly shows
 * the main menu until the user chooses to exit.
 */
void CinemaSystem::run() {
    showWelcome();

    int choice;
    User currentUser;   

    do {
        showMainMenu();
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                login(currentUser);
                break;
            case 2:
                signup(currentUser);
                break;
            case 3:
                guestLogin(currentUser);
                break;
            case 4:
                cout << "Exiting CINE++...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }


        if (choice >= 1 && choice <= 3) {
            MenuService menu;
            menu.showUserMenu(currentUser);
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
 * Reads password from console while showing '*'
 * Supports backspace and enter
 */

string getHiddenPassword() {
    string password;
    char ch;

    while (true) {
        ch = _getch();   // get character without echo

        if (ch == '\r') {          // Enter key
            cout << endl;
            break;
        }
        else if (ch == '\b') {     // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else {
            password.push_back(ch);
            cout << "*";
        }
    }
    return password;
}

bool isValidEmail(const string& email) {
    // no spaces
    if (email.find(' ') != string::npos)
        return false;

    // exactly one '@'
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0)
        return false;

    // last dot must be AFTER '@'
    size_t lastDot = email.rfind('.');
    if (lastDot == string::npos || lastDot < atPos + 2)
        return false;

    // must not end with dot
    if (lastDot == email.length() - 1)
        return false;

    // extension length >= 2
    size_t extLen = email.length() - lastDot - 1;
    if (extLen < 2)
        return false;

    // extension must be alphabetic
    for (size_t i = lastDot + 1; i < email.length(); i++) {
        if (!isalpha(email[i]))
            return false;
    }

    return true;
}



bool isValidPassword(const string& password) {
    if (password.length() < 8)
        return false;

    bool hasLetter = false;
    bool hasDigit = false;

    for (char c : password) {
        if (isalpha(c))
            hasLetter = true;
        else if (isdigit(c))
            hasDigit = true;
    }

    return hasLetter && hasDigit;
}



/*
 * Registers a new user by storing encrypted credentials.
 * Password encryption ensures raw passwords are never saved.
 */

void CinemaSystem::signup(User& user) {

    string fullName, email, password, role;

    // FULL NAME FIRST
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter full name: ";
    getline(cin, fullName);

    cout << "Enter email: ";
    cin >> email;

    // EMAIL VALIDATION
    if (!isValidEmail(email)) {
        cout << "Invalid email format.\n";
        return;
    }

    cout << "Enter password: ";
    password = getHiddenPassword();

    // PASSWORD VALIDATION
    if (!isValidPassword(password)) {
        cout << "Password must be at least 8 characters long and contain letters and numbers.\n";
        return;
    }

    cout << "Signup as (user/admin): ";
    cin >> role;

    if (role != "user" && role != "admin") {
        cout << "Invalid role selection.\n";
        return;
    }

    //  ADMIN CHECK
    if (role == "admin") {
        string adminCode;
        cout << "Enter admin secret code: ";
        cin >> adminCode;

        if (adminCode != ADMIN_SECRET_CODE) {
            cout << "Unauthorized admin signup attempt!\n";
            return;
        }
    }

    // SAVE USER (WITH FULL NAME)
    string encrypted = User::encryptPassword(password);

    User newUser(fullName, email, encrypted, role);
    FileManager::saveUser(newUser);

    user = newUser;   // 

    cout << "Signup successful as " << role << "!\n";
}



/*
 * Authenticates user credentials by comparing encrypted values.
 * Redirects user to admin or user menu based on role.
 */

void CinemaSystem::login(User& user) {
    string email, password;

    cout << "Enter email: ";
    cin >> email;

    if (!isValidEmail(email)) {
        cout << "Invalid email format.\n";
        return;
    }

    cout << "Enter password: ";
    password = getHiddenPassword();

    string encrypted = User::encryptPassword(password);
    vector<User> users = FileManager::loadUsers();

    for (const User& u : users) {
        if (u.getEmail() == email &&
            u.getPassword() == encrypted) {

            if (u.getRole() == "admin") {
                AdminService adminService;
                adminService.adminMenu(u);
            } else {
                UserService userService;
                userService.userMenu(u);
            }
            return;
        }
    }

    cout << "Invalid email or password.\n";
}


/* Guest login using Bangladeshi phone number validation.*/

void CinemaSystem::guestLogin(User& user) {

    string rest;
    cout << "Enter phone number: +880";
    cin >> rest;

    string phone = "+880" + rest;

    if (!isValidBangladeshPhone(phone)) {
        cout << "Please input valid phone number.\n";
        return;
    }

    int guestNo = getNextGuestNumber();
    string guestName = "Guest " + to_string(guestNo);

    User guest(guestName, phone, "", "guest");
    user = guest;

    cout << "Guest login successful as " << guestName << ".\n";
}


int CinemaSystem::getNextGuestNumber() {
    ifstream in("../data/guest_counter.txt");
    int count = 0;

    if (in.is_open()) {
        in >> count;
        in.close();
    }

    count++; // next guest

    ofstream out("../data/guest_counter.txt");
    out << count;
    out.close();

    return count;
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

