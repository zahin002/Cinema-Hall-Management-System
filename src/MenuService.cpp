#include "MenuService.h"
#include "UserService.h"
#include "TerminalColors.h"

#include <iostream>

using namespace std;

/* ================= USER MENU ================= */

void MenuService::showUserMenu(const User& user) {

    UserService userService;
    bool isGuest = (user.getRole() == "guest");
    int choice;

    do {
        cout << "\n";
        cout << BOLD << CYAN << "=========== USER MENU ===========" << RESET << "\n";
        cout << "1. Browse Movies\n";
        cout << "2. View Showtimes\n";
        cout << "3. Book Seat\n";
        cout << "4. Recommend Best Seat\n";
        cout << "5. Cancel Ticket\n";
        cout << "6. Logout\n";

        if (isGuest)
            cout << YELLOW << "(Guest Mode - Email: " << user.getEmail() << ")\n" << RESET;

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                browseMoviesMenu(user);
                break;

            case 2:
                userService.viewShowtimes();
                break;

            case 3:
                userService.bookSeat(user);
                break;

            case 4:
                userService.recommendSeat();
                break;

            case 5:
                userService.cancelTicket();
                break;

            case 6:
                cout << GREEN << "Logged out.\n" << RESET;
                break;

            default:
                cout << RED << "Invalid choice.\n" << RESET;
        }

    } while (choice != 6);
}

/* ================= BROWSE MOVIES ================= */

void MenuService::browseMoviesMenu(const User& user) {

    UserService userService;
    int choice;

    do {
        cout << "\n";
        cout << BOLD << CYAN << "======= BROWSE MOVIES =======" << RESET << "\n";
        cout << "1. Trending Movies\n";
        cout << "2. All Movies\n";
        cout << "3. Filter Movies\n";
        cout << "4. Search by Keyword\n";
        cout << "5. Back\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << YELLOW << "Trending movies will appear here.\n" << RESET;
                // (implemented later using ratings.txt)
                selectMovieForDetails(user);
                break;

            case 2:
                userService.viewMovies();
                selectMovieForDetails(user);
                break;

            case 3:
                userService.filterMovies();
                selectMovieForDetails(user);
                break;

            case 4:
                userService.searchMovieByName();
                selectMovieForDetails(user);
                break;

            case 5:
                break;

            default:
                cout << RED << "Invalid choice.\n" << RESET;
        }

    } while (choice != 5);
}

/* ================= MOVIE CODE ENTRY ================= */

void MenuService::selectMovieForDetails(const User& user) {
    int movieCode;
    cout << "\nEnter Movie Code to view details (0 to go back): ";
    cin >> movieCode;

    if (movieCode == 0)
        return;

    UserService userService;
    userService.showMovieDetails(movieCode);
}

