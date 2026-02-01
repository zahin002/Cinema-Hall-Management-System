#include "AdminService.h"
#include "FileManager.h"
#include "Movie.h"
#include "SeatMap.h"
#include <fstream>
#include <iostream>

using namespace std;

vector<string> TIME_SLOTS = {
    "08:30 AM - 10:00 AM",
    "11:00 AM - 01:00 PM",
    "02:00 PM - 04:30 PM",
    "05:00 PM - 07:30 PM",
    "08:00 PM - 10:30 PM"
};


/* ================= ADMIN MENU ================= */

void AdminService::adminMenu(const User&) {
    int choice;
    do {
        cout << "\n--- ADMIN MENU ---\n";
        cout << "1. Add Movie\n";
        cout << "2. View Movies\n";
        cout << "3. Delete Movie\n";
        cout << "4. Add Showtime\n";
        cout << "5. View Showtimes\n";
        cout << "6. Create Seat Map\n";
        cout << "7. View Seat Map\n";
        cout << "8. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addMovie(); break;
            case 2: viewMovies(); break;
            case 3: deleteMovie(); break;
            case 4: addShowtime(); break;
            case 5: viewShowtimes(); break;
            case 6: createSeatMap(); break;
            case 7: viewSeatMap(); break;
            case 8: cout << "Logged out.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 8);
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


    cout << "\nChoose Time Slot:\n";
    for (size_t i = 0; i < TIME_SLOTS.size(); i++) {
        cout << i + 1 << ". " << TIME_SLOTS[i] << endl;
    }

    int slotChoice;
    cout << "Enter option (1-5): ";
    cin >> slotChoice;

    if (slotChoice < 1 || slotChoice > TIME_SLOTS.size()) {
        cout << "Invalid time slot selection.\n";
        return;
    }

    time = TIME_SLOTS[slotChoice - 1];

    cout << "Enter hall number: ";
    cin >> hallNo;

    //3 Halls Fixed//

    if (hallNo < 1 || hallNo > 3) {
        cout << "Invalid hall number. Only halls 1–3 are available.\n";
        return;
    }


    vector<Showtime> existingShows = FileManager::loadShowtimes();

    for (const Showtime& s : existingShows) {
        if (s.getHallNo() == hallNo && s.getDate() == date &&s.getTime() == time) {
            cout << "Showtime conflict detected.\n";
            cout << "Hall " << hallNo << " already has a showtime at this time.\n";
            cout << "Please delete the existing showtime first.\n";
            return;
        }
    }


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
