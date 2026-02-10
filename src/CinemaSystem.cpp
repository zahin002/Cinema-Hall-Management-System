#include <iostream>
#include <conio.h>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <set>

#include "CinemaSystem.h"
#include "FileManager.h"
#include "Movie.h"
#include "AdminService.h"
#include "UserService.h"
#include "MenuService.h"
#include "TerminalColors.h"

using namespace std;

const string ADMIN_SECRET_CODE = "ADMIN2026";


/* ================= UI ================= */

void CinemaSystem::showWelcome() {
    cout << BOLD << CYAN;
    cout << "=====================================\n";
    cout << "        WELCOME TO CINE++             \n";
    cout << "   Cinema Hall Management System      \n";
    cout << "=====================================\n\n";
    cout << RESET;
}

void CinemaSystem::showMainMenu() {
    cout << BOLD << BLUE;
    cout << "1. Login\n";
    cout << "2. Signup\n";
    cout << "3. Guest Booking\n";
    cout << "4. Exit\n";
    cout << RESET;
}


/* ================= MAIN CONTROLLER ================= */

void CinemaSystem::run() {

    showWelcome();

    int choice;
    User currentUser;

    do {
        showMainMenu();
        cout << YELLOW << "Enter choice: " << RESET;

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input. Please enter a number.\n" << RESET;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {

            case 1: {   // LOGIN
                if (login(currentUser)) {
                    if (currentUser.getRole() == "admin") {
                        AdminService adminService;
                        adminService.adminMenu(currentUser);
                    } else {
                        MenuService::showUserMenu(currentUser);
                    }
                }
                break;
            }

            case 2: {   // SIGNUP
                if (signup(currentUser)) {
                    if (currentUser.getRole() == "admin") {
                        AdminService adminService;
                        adminService.adminMenu(currentUser);
                    } else {
                        MenuService::showUserMenu(currentUser);
                    }
                }
                break;
            }

            case 3: {   // GUEST
                if (guestLogin(currentUser)) {
                    // guests are always users
                    MenuService::showUserMenu(currentUser);
                }
                break;
            }

            case 4:
                cout << GREEN << "Exiting CINE++...\n" << RESET;
                break;

            default:
                cout << RED << "Invalid choice.\n" << RESET;
        }

        cout << endl;

    } while (choice != 4);
}

/* ================= HIDDEN PASSWORD ================= */

string getHiddenPassword() {
    string password;
    char ch;

    while (true) {
        ch = _getch();

        if (ch == '\r') {
            cout << endl;
            break;
        }
        else if (ch == '\b') {
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

/* ================= VALIDATION ================= */

bool isValidEmail(const string& email) {

    if (email.find(' ') != string::npos) return false;

    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0) return false;

    size_t dotPos = email.rfind('.');
    if (dotPos == string::npos || dotPos < atPos + 2) return false;

    if (dotPos == email.length() - 1) return false;

    return true;
}

bool isValidPassword(const string& password) {

    if (password.length() < 8) return false;

    bool hasLetter = false, hasDigit = false;

    for (char c : password) {
        if (isalpha(c)) hasLetter = true;
        if (isdigit(c)) hasDigit = true;
    }

    return hasLetter && hasDigit;
}

/* ================= SIGNUP ================= */

bool CinemaSystem::signup(User& user) {

    string fullName, email, password;
    int roleChoice;

    cout << CYAN << "Enter full name: " << RESET;
    getline(cin, fullName);

    if (fullName.empty()) {
        cout << RED << "Full name cannot be empty.\n" << RESET;
        return false;
    }

    cout << CYAN << "Enter email: " << RESET;
    cin >> email;

    if (!isValidEmail(email)) {
        cout << RED << "Invalid email format.\n" << RESET;
        return false;
    }

    cout << CYAN << "Enter password: " << RESET;
    password = getHiddenPassword();

    if (!isValidPassword(password)) {
        cout << RED
             << "Password must be at least 8 characters long and contain letters and numbers.\n"
             << RESET;
        return false;
    }

    cout << BOLD << "\nSignup as:\n" << RESET;
    cout << "1. Admin\n2. User\n";
    cout << YELLOW << "Enter choice: " << RESET;
    cin >> roleChoice;

    string role = (roleChoice == 1) ? "admin" : "user";

    if (role == "admin") {
        string adminCode;
        cout << YELLOW << "Enter admin secret code: " << RESET;
        adminCode = getHiddenPassword();

        if (adminCode != ADMIN_SECRET_CODE) {
            cout << RED << "Unauthorized admin signup attempt!\n" << RESET;
            return false;
        }
    }

    string encrypted = User::encryptPassword(password);
    user = User(fullName, email, encrypted, role);

    FileManager::saveUser(user);

    cout << GREEN << "Signup successful as " << role << "!\n" << RESET;
    return true;
}


/* ================= LOGIN ================= */

bool CinemaSystem::login(User& user) {

    string email, password;

    cout << CYAN << "Enter email: " << RESET;
    cin >> email;

    cout << CYAN << "Enter password: " << RESET;
    password = getHiddenPassword();

    string encrypted = User::encryptPassword(password);
    vector<User> users = FileManager::loadUsers();

    for (const User& u : users) {
        if (u.getEmail() == email && u.getPassword() == encrypted) {
            user = u;  // role preserved 
            cout << GREEN << "Login successful!\n" << RESET;
            return true;
        }
    }

    cout << RED << "Invalid email or password.\n" << RESET;
    return false;
}

/* ================= GUEST ================= */

bool CinemaSystem::guestLogin(User& user) {

    string rest;
    cout << CYAN << "Enter phone number: +880" << RESET;
    cin >> rest;

    string phone = "+880" + rest;

    if (!isValidBangladeshPhone(phone)) {
        cout << RED << "Invalid phone number.\n" << RESET;
        return false;
    }

    int guestNo = getNextGuestNumber();
    user = User("Guest " + to_string(guestNo), phone, "", "guest");

    cout << GREEN << "Guest login successful.\n" << RESET;
    return true;
}

/* ================= GUEST COUNTER ================= */

int CinemaSystem::getNextGuestNumber() {

    ifstream in("../data/guest_counter.txt");
    int count = 0;

    if (in.is_open()) {
        in >> count;
        in.close();
    }

    count++;

    ofstream out("../data/guest_counter.txt");
    out << count;
    out.close();

    return count;
}

/* ================= PHONE VALIDATION ================= */

bool CinemaSystem::isValidBangladeshPhone(const string& phone) {

    if (phone.length() != 14) return false;
    if (phone.substr(0, 4) != "+880") return false;
    if (phone[4] != '1') return false;

    for (int i = 5; i < 14; i++) {
        if (!isdigit(phone[i])) return false;
    }

    return true;
}
