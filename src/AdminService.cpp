#include "AdminService.h"
#include "FileManager.h"
#include "Movie.h"
#include "SeatMap.h"
#include <iostream>

using namespace std;

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


/* ================= SHOWTIME MANAGEMENT ================= */

void AdminService::addShowtime() {
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

void AdminService::viewShowtimes() {
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
