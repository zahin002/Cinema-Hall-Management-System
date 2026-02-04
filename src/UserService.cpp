#include "UserService.h"
#include "FileManager.h"
#include "SeatMap.h"
#include "PricingEngine.h"
#include <iostream>
#include <sstream>
#include <set>
#include <limits>

using namespace std;

/* ================= USER MENU ================= */

void UserService::userMenu(const User& user) {

    bool isGuest = (user.getRole() == "guest");

    int choice;
    do {
        cout << "\n--- USER MENU ---\n";
        cout << "1. Browse Movies\n";
        cout << "2. Filter Movies (Genre/Language)\n";
        cout << "3. View Showtimes\n";
        cout << "4. Book Seat\n";
        cout << "5. Recommend Best Seat\n";
        cout << "6. Logout\n";

         if (isGuest)
            cout << "(Guest Mode - Phone: " << user.getEmail() << ")\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewMovies(); break;
            case 2: filterMovies(); break;
            case 3: viewShowtimes(); break;
            case 4: bookSeat(user); break;
            case 5: recommendSeat(); break;
            case 6: cout << "Logged out.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 6);
}

/* Displays all available movies for users.*/

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

/* Displays all filtered movies based on genre/language */

void UserService::filterMovies() {
    string genre, language;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    cout << "Enter genre (or * for any): ";
    getline(cin, genre);
    cout << "Enter language (or * for any): ";
    getline(cin, language);

    vector<Movie> movies = FileManager::loadMovies();
    bool found = false;

    cout << "\n--- FILTERED MOVIES ---\n";
    for (const Movie& m : movies) {
        if ((genre == "*" || m.getGenre() == genre) &&
            (language == "*" || m.getLanguage() == language)) {

            cout << m.getCode() << " | "
                 << m.getTitle() << " | "
                 << m.getGenre() << " | "
                 << m.getLanguage() << endl;
            found = true;
        }
    }

    if (!found)
        cout << "No movies found.\n";
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

void UserService::bookSeat(const User& user) {

    string input;

    vector<Showtime> shows = FileManager::loadShowtimes();

    if (shows.empty()) {
        cout << "No showtimes available.\n";
        return;
    }

    cout << "\n--- AVAILABLE SHOWTIMES ---\n";
    for (size_t i = 0; i < shows.size(); i++) {
        cout << i + 1 << ". "
        << shows[i].getDate() << " | "
        << shows[i].getTime() << " | Hall "
        << shows[i].getHallNo() << endl;
    }

    int choice;
    cout << "Select showtime (1-" << shows.size() << "): ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > shows.size()) {
        cout << "Invalid showtime selection.\n";
        return;
    }

    Showtime selectedShow = shows[choice - 1];


    SeatMap map = FileManager::loadOrCreateSeatMap(
    selectedShow.getHallNo(),
    selectedShow.getDate(),
    selectedShow.getTime()
    );

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
        int col;

        try {
            col = stoi(seat.substr(1)) - 1;
        } catch (...) {
            cout << "Invalid seat format: " << seat << endl;
            return;
        }

        if (!map.isSeatAvailable(row, col)) {
            cout << "Seat unavailable: " << seat << endl;
            return;
        }

        selected.push_back({row, col});
    }

    if (selected.empty()) {
        cout << "No seats selected.\n";
        return;
    }

    /*
    // ===== PRICING =====
    int seatCount = selected.size();
    int finalPrice;
    string userKey = user.getEmail();

    if (user.getRole() == "guest") {
        finalPrice = seatCount * 500; // no discount
    } else {
        finalPrice = PricingEngine::calculateFinalPrice(
            seatCount,
            userKey
        );
    }
        

    cout << "\n----- BILL -----\n";
    cout << "Tickets: " << seatCount << endl;
    cout << "Base price: " << seatCount * 500 << " Tk\n";
    cout << "Final price: " << finalPrice << " Tk\n"; */

    // ===== BOOK SEATS =====
    for (auto &s : selected)
        map.bookSeat(s.first, s.second);

    FileManager::saveSeatMap(FileManager::getSeatMapFilename(
        selectedShow.getHallNo(),
        selectedShow.getDate(),
        selectedShow.getTime()
        ),
        map
    );


   /* // ===== SAVE TICKET HISTORY =====
    FileManager::saveTicket(
        userKey,
        seatCount,
        finalPrice
    );
    
    */

    // ===== DISPLAY UPDATED SEAT MAP =====
    cout << "\nUpdated Seat Map:\n";
    map.display();


    /*
    // ===== PRINT TICKET =====
    cout << "\n================= CINE++ TICKET =================\n";
    cout << "User       : " << user.getEmail() << endl;
    cout << "Show ID    : " << showId << endl;
    cout << "Seats      : ";
    for (auto &s : selected)
        cout << char('A' + s.first) << s.second + 1 << " ";
    cout << "\nTickets    : " << seatCount << endl;
    cout << "Total Paid : " << finalPrice << " Tk\n";
    cout << "Status     : CONFIRMED\n";
    cout << "=================================================\n";
    */
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