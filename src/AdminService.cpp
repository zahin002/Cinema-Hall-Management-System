#include "AdminService.h"
#include "FileManager.h"
#include "Movie.h"
#include "SeatMap.h"
#include "MenuService.h"
#include "TerminalColors.h"

#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <limits>

using namespace std;

vector<string> TIME_SLOTS = {
    "08:30 AM - 10:00 AM",
    "11:00 AM - 01:00 PM",
    "02:00 PM - 04:30 PM",
    "05:00 PM - 07:30 PM",
    "08:00 PM - 10:30 PM"
};

/* ================= ADMIN MENU ================= */

void AdminService::adminMenu(const User& admin) {

    int choice;

    do {
        cout << "\n" << BOLD << CYAN << "--- ADMIN MENU ---\n" << RESET;
        cout << "1. Add Movie\n";
        cout << "2. View Movies\n";
        cout << "3. Delete Movie\n";
        cout << "4. Add Showtime\n";
        cout << "5. View Showtimes\n";
        cout << "6. Delete Showtime\n";
        cout << "7. View Seat Map\n";
        cout << "8. Logout\n";
        cout << YELLOW << "Enter choice: " << RESET;

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input.\n" << RESET;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {

            case 1:
                addMovie();
                break;

            case 2: {   // REUSE USER BROWSE MOVIES
                MenuService menu;
                menu.browseMoviesMenu(admin,true);
                break;
            }

            case 3:
                deleteMovie();
                break;

            case 4:
                addShowtime();
                break;

            case 5:
                viewShowtimes();
                break;

            case 6:
                deleteShowtime();
                break;

            case 7:
                viewSeatMap();
                break;

            case 8:
                cout << GREEN << "Logged out.\n" << RESET;
                break;

            default:
                cout << RED << "Invalid choice.\n" << RESET;
        }

    } while (choice != 8);
}

/* ================= MOVIE MANAGEMENT ================= */

void AdminService::addMovie() {

    string title, genre, language;
    int duration;

    int code = FileManager::getNextMovieCode();
    cout << CYAN << "Auto-generated movie code: " << code << RESET << endl;

    cout << "Enter movie title: ";
    getline(cin, title);

    cout << "Enter genre: ";
    getline(cin, genre);

    cout << "Enter duration (minutes): ";
    cin >> duration;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter language: ";
    getline(cin, language);

    FileManager::saveMovie(Movie(code, title, genre, duration, language));
    cout << GREEN << "Movie added successfully!\n" << RESET;
}

void AdminService::deleteMovie() {

    int code;
    cout << YELLOW << "Enter movie code to delete: " << RESET;
    cin >> code;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<Movie> movies = FileManager::loadMovies();
    vector<Showtime> shows = FileManager::loadShowtimes();

    for (const Showtime& s : shows) {
        if (s.getMovieCode() == code) {
            cout << RED << "Cannot delete movie. Showtime exists.\n" << RESET;
            return;
        }
    }

    ofstream out("../data/movies_temp.txt");
    bool deleted = false;

    for (const Movie& m : movies) {
        if (m.getCode() != code) {
            out << m.getCode() << "|"
                << m.getTitle() << "|"
                << m.getGenre() << "|"
                << m.getDuration() << "|"
                << m.getLanguage() << "\n";
        } else {
            deleted = true;
        }
    }
    out.close();

    remove("../data/movies.txt");
    rename("../data/movies_temp.txt", "../data/movies.txt");

    if (deleted)
        cout << GREEN << "Movie deleted successfully.\n" << RESET;
    else
        cout << YELLOW << "Movie not found.\n" << RESET;
}

/* ================= SHOWTIME MANAGEMENT ================= */

bool AdminService::isValidFutureDate(const string& dateStr) {

    int d, m, y;
    char dash1, dash2;
    stringstream ss(dateStr);

    ss >> d >> dash1 >> m >> dash2 >> y;
    if (ss.fail()) return false;

    tm inputDate = {};
    inputDate.tm_mday = d;
    inputDate.tm_mon = m - 1;
    inputDate.tm_year = y - 1900;

    time_t inputTime = mktime(&inputDate);
    time_t now = time(nullptr);

    double diffDays = difftime(inputTime, now) / (60 * 60 * 24);
    return diffDays >= 0 && diffDays <= 30;
}

void AdminService::addShowtime() {

    int movieCode, hallNo, slotChoice;
    string date;

    vector<Movie> movies = FileManager::loadMovies();
    if (movies.empty()) {
        cout << RED << "No movies available.\n" << RESET;
        return;
    }

    cout << "\nAvailable Movies:\n";
    for (const Movie& m : movies)
        cout << m.getCode() << " - " << m.getTitle() << endl;

    cout << "Enter movie code: ";
    cin >> movieCode;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool found = false;
    for (const Movie& m : movies)
        if (m.getCode() == movieCode)
            found = true;

    if (!found) {
        cout << RED << "Invalid movie code.\n" << RESET;
        return;
    }

    cout << "Enter date (DD-MM-YYYY): ";
    getline(cin, date);

    if (!isValidFutureDate(date)) {
        cout << RED << "Invalid date.\n" << RESET;
        return;
    }

    cout << "\nChoose Time Slot:\n";
    for (size_t i = 0; i < TIME_SLOTS.size(); i++)
        cout << i + 1 << ". " << TIME_SLOTS[i] << endl;

    cout << "Enter option: ";
    cin >> slotChoice;

    if (slotChoice < 1 || slotChoice > (int)TIME_SLOTS.size()) {
        cout << RED << "Invalid slot.\n" << RESET;
        return;
    }

    cout << "Enter hall number (1-3): ";
    cin >> hallNo;

    if (hallNo < 1 || hallNo > 3) {
        cout << RED << "Invalid hall number.\n" << RESET;
        return;
    }

    FileManager::saveShowtime(
        Showtime(movieCode, date, TIME_SLOTS[slotChoice - 1], hallNo)
    );

    cout << GREEN << "Showtime added successfully!\n" << RESET;
}

/* ================= VIEW / DELETE SHOWTIME & SEAT MAP ================= */

void AdminService::viewShowtimes() {
    vector<Showtime> shows = FileManager::loadShowtimes();
    vector<Movie> movies = FileManager::loadMovies();

    if (shows.empty()) {
        cout << YELLOW << "No showtimes available.\n" << RESET;
        return;
    }

    cout << "\n" << BOLD << CYAN << "--- SHOWTIMES ---\n" << RESET;

    for (size_t i = 0; i < shows.size(); i++) {
        string title = "Unknown";
        for (const Movie& m : movies)
            if (m.getCode() == shows[i].getMovieCode())
                title = m.getTitle();

        cout << i + 1 << ". "
             << title << " | "
             << shows[i].getDate() << " | "
             << shows[i].getTime() << " | Hall "
             << shows[i].getHallNo() << endl;
    }
}

void AdminService::deleteShowtime() {
    vector<Showtime> shows = FileManager::loadShowtimes();
    if (shows.empty()) return;

    int choice;
    cout << "Enter showtime number to delete: ";
    cin >> choice;

    if (choice < 1 || choice > (int)shows.size()) {
        cout << RED << "Invalid selection.\n" << RESET;
        return;
    }

    ofstream out("../data/showtimes_temp.txt");

    for (size_t i = 0; i < shows.size(); i++) {
        if ((int)i != choice - 1)
            out << shows[i].getMovieCode() << "|"
                << shows[i].getDate() << "|"
                << shows[i].getTime() << "|"
                << shows[i].getHallNo() << "\n";
    }
    out.close();

    remove("../data/showtimes.txt");
    rename("../data/showtimes_temp.txt", "../data/showtimes.txt");

    cout << GREEN << "Showtime deleted.\n" << RESET;
}

void AdminService::viewSeatMap() {
    vector<Showtime> shows = FileManager::loadShowtimes();
    if (shows.empty()) return;

    int choice;
    cout << "Select showtime: ";
    cin >> choice;

    if (choice < 1 || choice > (int)shows.size()) return;

    Showtime s = shows[choice - 1];

    SeatMap map = FileManager::loadOrCreateSeatMap(
        s.getHallNo(), s.getDate(), s.getTime()
    );
    map.display();
}
