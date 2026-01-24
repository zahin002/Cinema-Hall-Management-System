/*************************************************************
 * File: CinemaSystem.cpp
 * Project: CINE++ Cinema Hall Management System (SPL-1)
 *
 * Week 1:
 * - Application entry flow
 * - Welcome screen
 * - Login and signup system
 * - Role-based menu routing
 *
 * Week 2:
 * - Movie and showtime management
 * - File-based persistence integration
 *
 * Week 3:
 * - Seat map creation and visualization
 * - Seat booking with full validation
 *
 * Week 4:
 * - Best seat recommendation logic
 * - Group booking consistency handling
 *************************************************************/

#include <iostream>
#include <vector>
#include <sstream>     // Used for parsing multiple seat inputs
#include <set>         // Used to prevent duplicate seat selection
#include "CinemaSystem.h"
#include "FileManager.h"
#include "Movie.h"

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
            case 3: cout << "Exiting CINE++...\n"; break;
            default: cout << "Invalid choice.\n";
        }
        cout << endl;
    } while (choice != 3);
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
    cout << "3. Exit\n";
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

    /*
     * Linear search is used since user data size
     * is expected to be small for this system.
     */
    for (const User& u : users) {
        if (u.getEmail() == email && u.getPassword() == encrypted) {
            if (u.getRole() == "admin")
                adminMenu(u);
            else
                userMenu(u);
            return;
        }
    }
    cout << "Invalid email or password.\n";
}

/* ================= ADMIN MENU ================= */

void CinemaSystem::adminMenu(const User&) {
    int choice;
    do {
        cout << "\n--- ADMIN MENU ---\n";
        cout << "1. Add Movie\n";
        cout << "2. View Movies\n";
        cout << "3. Add Showtime\n";
        cout << "4. View Showtimes\n";
        cout << "5. Create Seat Map\n";
        cout << "6. View Seat Map\n";
        cout << "7. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addMovie(); break;
            case 2: viewMovies(); break;
            case 3: addShowtime(); break;
            case 4: viewShowtimes(); break;
            case 5: createSeatMap(); break;
            case 6: viewSeatMap(); break;
            case 7: cout << "Logged out.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);
}

/* ================= USER MENU ================= */

void CinemaSystem::userMenu(const User&) {
    int choice;
    do {
        cout << "\n--- USER MENU ---\n";
        cout << "1. Browse Movies\n";
        cout << "2. View Showtimes\n";
        cout << "3. Book Seat\n";
        cout << "4. Recommend Best Seat\n";
        cout << "5. Logout\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewMovies(); break;
            case 2: viewShowtimes(); break;
            case 3: bookSeat(); break;
            case 4: recommendSeat(); break;
            case 5: cout << "Logged out.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

/* ================= MOVIE MANAGEMENT ================= */

void CinemaSystem::addMovie() {
    string title, genre, language;
    int duration;

    /*
     * cin.ignore() is required to clear the input buffer
     * before using getline to avoid skipped input.
     */
    cin.ignore();
    cout << "Enter movie title: ";
    getline(cin, title);
    cout << "Enter genre: ";
    getline(cin, genre);
    cout << "Enter duration (minutes): ";
    cin >> duration;
    cin.ignore();
    cout << "Enter language: ";
    getline(cin, language);

    FileManager::saveMovie(Movie(title, genre, duration, language));
    cout << "Movie added successfully!\n";
}

void CinemaSystem::viewMovies() {
    vector<Movie> movies = FileManager::loadMovies();

    if (movies.empty()) {
        cout << "No movies available.\n";
        return;
    }

    cout << "\n--- MOVIE LIST ---\n";
    for (size_t i = 0; i < movies.size(); i++) {
        cout << i + 1 << ". "
             << movies[i].getTitle()
             << " | " << movies[i].getGenre()
             << " | " << movies[i].getDuration() << " mins"
             << " | " << movies[i].getLanguage() << endl;
    }
}

/* ================= SHOWTIME MANAGEMENT ================= */

void CinemaSystem::addShowtime() {
    string movieTitle, date, time;
    int hallNo;

    cin.ignore();
    cout << "Enter movie title: ";
    getline(cin, movieTitle);
    cout << "Enter date (YYYY-MM-DD): ";
    getline(cin, date);
    cout << "Enter time (HH:MM): ";
    getline(cin, time);
    cout << "Enter hall number: ";
    cin >> hallNo;

    FileManager::saveShowtime(Showtime(movieTitle, date, time, hallNo));
    cout << "Showtime added successfully!\n";
}

void CinemaSystem::viewShowtimes() {
    vector<Showtime> shows = FileManager::loadShowtimes();

    if (shows.empty()) {
        cout << "No showtimes available.\n";
        return;
    }

    cout << "\n--- SHOWTIMES ---\n";
    for (size_t i = 0; i < shows.size(); i++) {
        cout << i + 1 << ". "
             << shows[i].getMovieTitle()
             << " | " << shows[i].getDate()
             << " | " << shows[i].getTime()
             << " | Hall " << shows[i].getHallNo()
             << endl;
    }
}

/* ================= SEAT MAP & BOOKING ================= */

void CinemaSystem::createSeatMap() {
    int showId, rows, cols;

    cout << "Enter show ID: ";
    cin >> showId;
    cout << "Enter number of rows: ";
    cin >> rows;
    cout << "Enter number of columns: ";
    cin >> cols;

    SeatMap map(rows, cols);
    FileManager::saveSeatMap(showId, map);

    cout << "Seat map created successfully!\n";
}

void CinemaSystem::viewSeatMap() {
    int showId;
    cout << "Enter show ID: ";
    cin >> showId;

    SeatMap map = FileManager::loadSeatMap(showId);
    map.display();
}

/*
 * Handles multi-seat booking with full validation.
 * Booking is atomic: either all seats are booked or none.
 */
void CinemaSystem::bookSeat() {
    int showId;
    string input;

    cout << "Enter show ID: ";
    cin >> showId;
    cin.ignore();

    SeatMap map = FileManager::loadSeatMap(showId);
    map.display();

    cout << "\nEnter seats (e.g. A1 A2 B5 C7 or A1,B5,C7): ";
    getline(cin, input);

    /*
     * Normalizing input allows flexible formatting
     * while keeping parsing logic simple.
     */
    for (char &c : input) {
        if (c == ',') c = ' ';
    }

    stringstream ss(input);
    string seatCode;

    vector<pair<int,int>> selectedSeats;
    set<string> uniqueSeats;

    /*
     * Using a set ensures that the same seat
     * cannot be booked more than once in a single request.
     */
    while (ss >> seatCode) {
        seatCode[0] = toupper(seatCode[0]);

        if (uniqueSeats.count(seatCode)) {
            cout << "Duplicate seat detected: " << seatCode << endl;
            return;
        }
        uniqueSeats.insert(seatCode);

        char rowChar = seatCode[0];
        int seatNumber;

        try {
            seatNumber = stoi(seatCode.substr(1));
        } catch (...) {
            cout << "Invalid seat format: " << seatCode << endl;
            return;
        }

        int row = rowChar - 'A';
        int col = seatNumber - 1;

        if (row < 0 || row >= map.getRows() ||
            col < 0 || col >= map.getCols()) {
            cout << "Invalid seat: " << seatCode << endl;
            return;
        }

        if (!map.isSeatAvailable(row, col)) {
            cout << "Seat already booked: " << seatCode << endl;
            return;
        }

        selectedSeats.push_back({row, col});
    }

    if (selectedSeats.empty()) {
        cout << "No seats selected.\n";
        return;
    }

    /*
     * All validations are completed before booking.
     * This guarantees consistency in case of failure.
     */
    for (auto &s : selectedSeats) {
        map.bookSeat(s.first, s.second);
    }

    FileManager::saveSeatMap(showId, map);

    cout << "\nSeats booked successfully: ";
    for (auto &s : selectedSeats) {
        char rowChar = 'A' + s.first;
        cout << rowChar << (s.second + 1) << " ";
    }
    cout << endl;
}

/* ================= SEAT RECOMMENDATION ================= */

void CinemaSystem::recommendSeat() {
    int showId, count;

    cout << "Enter show ID: ";
    cin >> showId;
    cout << "Enter number of seats: ";
    cin >> count;

    SeatMap map = FileManager::loadSeatMap(showId);
    map.display();

    auto seats = map.recommendBestSeats_Custom(count);

    if (seats.empty()) {
        cout << "Sorry, Try to find manually\n";
        return;
    }

    cout << "Recommended seats: ";
    for (auto &s : seats) {
        cout << char('A' + s.first) << (s.second + 1) << " ";
    }
    cout << endl;
}
