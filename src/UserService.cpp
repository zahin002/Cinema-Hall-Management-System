#include "UserService.h"
#include "FileManager.h"
#include "SeatMap.h"
#include <iostream>
#include <sstream>
#include <set>

using namespace std;

/* ================= USER MENU ================= */

void UserService::userMenu(const User&) {
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

/*
 * Displays all available movies for users.
 */
void UserService::viewMovies() {
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

/* Displays all available showtimes */

void UserService::viewShowtimes() {
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


/* Atomic multi-seat booking */

void UserService::bookSeat() {
    int showId;
    string input;

    cout << "Enter show ID: ";
    cin >> showId;
    cin.ignore();

    SeatMap map = FileManager::loadSeatMap(showId);
    map.display();

    cout << "Enter seats (A1 A2 B5 or A1,B5): ";
    getline(cin, input);

    for (char &c : input)
        if (c == ',') c = ' ';

    stringstream ss(input);
    set<string> used;
    vector<pair<int,int>> selected;

    string seat;
    while (ss >> seat) {
        seat[0] = toupper(seat[0]);

        if (used.count(seat)) {
            cout << "Duplicate seat: " << seat << endl;
            return;
        }
        used.insert(seat);

        int row = seat[0] - 'A';
        int col = stoi(seat.substr(1)) - 1;

        if (!map.isSeatAvailable(row, col)) {
            cout << "Seat unavailable: " << seat << endl;
            return;
        }

        selected.push_back({row, col});
    }

    for (auto &s : selected)
        map.bookSeat(s.first, s.second);

    FileManager::saveSeatMap(showId, map);

   /* Display updated seat map after booking */
    cout << "\nUpdated Seat Map:\n";
    SeatMap updatedMap = FileManager::loadSeatMap(showId);
    updatedMap.display();

    cout << "\nSeats booked successfully: ";
    for (auto &s : selected)
        cout << char('A' + s.first) << s.second + 1 << " ";
    cout << endl;
}


/* ================= SEAT RECOMMENDATION ================= */

/* Suggests best seat(s) using center-priority algorithm */

void UserService::recommendSeat() {
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
    for (auto &s : seats)
        cout << char('A' + s.first) << s.second + 1 << " ";
    cout << endl;
}


