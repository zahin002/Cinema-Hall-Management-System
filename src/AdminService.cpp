#include "AdminService.h"
#include "FileManager.h"
#include "Movie.h"
#include "SeatMap.h"
#include <fstream>
#include <ctime>
#include <sstream>
#include <iostream>

using namespace std;

/* ================= ADMIN MENU ================= */


/*
 * Validates date in DD-MM-YYYY format
 * Allows only today to next 30 days
 */
bool isValidFutureDate(const string& dateStr) {
    int d, m, y;
    char dash1, dash2;

    stringstream ss(dateStr);
    ss >> d >> dash1 >> m >> dash2 >> y;

    if (ss.fail() || dash1 != '-' || dash2 != '-')
        return false;

    // Basic range check
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 2024)
        return false;

    tm inputDate = {};
    inputDate.tm_mday = d;
    inputDate.tm_mon  = m - 1;
    inputDate.tm_year = y - 1900;
    inputDate.tm_hour = 0;

    time_t inputTime = mktime(&inputDate);
    if (inputTime == -1)
        return false;

    // Get today's date
    time_t now = time(nullptr);
    tm today = *localtime(&now);
    today.tm_hour = 0;
    today.tm_min = 0;
    today.tm_sec = 0;

    time_t todayTime = mktime(&today);

    // Difference in days
    double diffDays = difftime(inputTime, todayTime) / (60 * 60 * 24);

    // Allow only today to next 30 days
    if (diffDays < 0 || diffDays > 30)
        return false;

    return true;
}

void AdminService::adminMenu(const User&) {
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

/* ================= MOVIE MANAGEMENT ================= */


void AdminService::addMovie() {
    string title, genre, language;
    int duration;

    int code = FileManager::getNextMovieCode();
    cout << "Auto-generated movie code: " << code << endl;

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

    FileManager::saveMovie(Movie(code, title, genre, duration, language));
    cout << "Movie added successfully!\n";
}



void AdminService::viewMovies() {
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

void AdminService::deleteMovie() {
    int code;
    cout << "Enter movie code to delete: ";
    cin >> code;

    vector<Movie> movies = FileManager::loadMovies();
    vector<Showtime> shows = FileManager::loadShowtimes();

    for (const Showtime& s : shows) {
        if (s.getMovieCode() == code) {
            cout << "Cannot delete movie. Showtime exists.\n";
            return;
        }
    }

    ofstream file("../data/movies_temp.txt");
    bool deleted = false;

    for (const Movie& m : movies) {
        if (m.getCode() != code)
            file << m.getCode() << "|"
                 << m.getTitle() << "|"
                 << m.getGenre() << "|"
                 << m.getDuration() << "|"
                 << m.getLanguage() << endl;
        else
            deleted = true;
    }
    file.close();

    remove("../data/movies.txt");
    rename("../data/movies_temp.txt", "../data/movies.txt");

    if (deleted)
        cout << "Movie deleted successfully.\n";
    else
        cout << "Movie not found.\n";
}



/* ================= SHOWTIME MANAGEMENT ================= */

void AdminService::addShowtime() {
    int movieCode, hallNo;
    string date, time;

    vector<Movie> movies = FileManager::loadMovies();

    if (movies.empty()) {
        cout << "No movies available. Add movies first.\n";
        return;
    }

    cout << "\nAvailable Movies:\n";
    for (const Movie& m : movies) {
        cout << m.getCode() << " - " << m.getTitle() << endl;
    }

    cout << "Enter movie code: ";
    cin >> movieCode;
    cin.ignore();

    bool found = false;
    for (const Movie& m : movies) {
        if (m.getCode() == movieCode) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "This movie is not available yet.\n";
        return;
    }

    cout << "Enter date (DD-MM-YYYY): ";
    getline(cin, date);

    if (!isValidFutureDate(date)) {
        cout << "Invalid date. You can only add showtimes for the next 30 days.\n";
        return;
    }

    cout << "Enter time (HH:MM): ";
    getline(cin, time);
    cout << "Enter hall number: ";
    cin >> hallNo;

    FileManager::saveShowtime(
        Showtime(movieCode, date, time, hallNo)
    );

    cout << "Showtime added successfully!\n";
}


void AdminService::viewShowtimes() {
    vector<Showtime> shows = FileManager::loadShowtimes();
    vector<Movie> movies = FileManager::loadMovies();

    if (shows.empty()) {
        cout << "No showtimes available.\n";
        return;
    }

    cout << "\n--- SHOWTIMES ---\n";
    for (size_t i = 0; i < shows.size(); i++) {

        string title = "Unknown";
        for (const Movie& m : movies) {
            if (m.getCode() == shows[i].getMovieCode()) {
                title = m.getTitle();
                break;
            }
        }

        cout << i + 1 << ". "
             << title
             << " | " << shows[i].getDate()
             << " | " << shows[i].getTime()
             << " | Hall " << shows[i].getHallNo()
             << endl;
    }
}


/* ================= SEAT MAP & BOOKING ================= */

void AdminService::createSeatMap() {
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

void AdminService::viewSeatMap() {
    int showId;
    cout << "Enter show ID: ";
    cin >> showId;

    SeatMap map = FileManager::loadSeatMap(showId);
    map.display();
}