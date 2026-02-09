#include "UserService.h"
#include "FileManager.h"
#include "SeatMap.h"
#include "PricingEngine.h"
#include "TerminalColors.h"
#include "SnackShop.h"
#include "TicketService.h"

#include <iostream>
#include <sstream>
#include <set>
#include <limits>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

/* ================= UTIL ================= */

static string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

/* ================= MOVIES ================= */

void UserService::viewMovies() {
    vector<Movie> movies = FileManager::loadMovies();

    if (movies.empty()) {
        cout << "No movies available.\n";
        return;
    }

    cout << "\n--- MOVIE LIST ---\n";
    for (size_t i = 0; i < movies.size(); i++) {
        cout << i + 1 << ". "
             << movies[i].getTitle() << " | "
             << movies[i].getGenre() << " | "
             << movies[i].getDuration() << " mins | "
             << movies[i].getLanguage() << endl;
    }
}

void UserService::showMovieDetails(int movieCode) {
    vector<Movie> movies = FileManager::loadMovies();
    vector<Showtime> shows = FileManager::loadShowtimes();

    // ---- find movie ----
    Movie selected;
    bool found = false;

    for (const Movie& m : movies) {
        if (m.getCode() == movieCode) {
            selected = m;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << RED << "❌ Invalid movie code.\n" << RESET;
        return;
    }

    // ---- header ----
    cout << "\n";
    cout << BOLD << CYAN << "========== MOVIE DETAILS ==========\n" << RESET;
    cout << BOLD << YELLOW << selected.getTitle() << RESET << "\n";
    cout << BOLD << CYAN << "===================================\n" << RESET;

    // ---- basic info ----
    cout << BOLD << "Duration : " << RESET << selected.getDuration() << " minutes\n";
    cout << BOLD << "Language : " << RESET << selected.getLanguage() << "\n";
    cout << BOLD << "Genre    : " << RESET << selected.getGenre() << "\n";

    // ---- showtimes ----
    bool hasShow = false;
    cout << "\n" << BOLD << CYAN << "Showtimes:\n" << RESET;

    for (const Showtime& s : shows) {
        if (s.getMovieCode() == movieCode) {
            if (!hasShow) hasShow = true;
            cout << " • "
                 << s.getDate()
                 << " | " << s.getTime()
                 << " | Hall " << s.getHallNo() << "\n";
        }
    }

    if (!hasShow) {
        cout << YELLOW << "⚠ Currently not in showtime\n" << RESET;
    }

    cout << BOLD << CYAN << "===================================\n" << RESET;
}


void UserService::filterMovies() {

    vector<Movie> movies = FileManager::loadMovies();
    if (movies.empty()) {
        cout << "No movies available.\n";
        return;
    }

    set<string> genreSet, languageSet;
    for (const Movie& m : movies) {
        genreSet.insert(m.getGenre());
        languageSet.insert(m.getLanguage());
    }

    vector<string> genres(genreSet.begin(), genreSet.end());
    vector<string> languages(languageSet.begin(), languageSet.end());

    cout << "\nChoose Genre:\n0. All\n";
    for (size_t i = 0; i < genres.size(); i++)
        cout << i + 1 << ". " << genres[i] << endl;

    int g;
    cin >> g;
    string selectedGenre = (g > 0 && g <= genres.size()) ? genres[g - 1] : "*";

    cout << "\nChoose Language:\n0. All\n";
    for (size_t i = 0; i < languages.size(); i++)
        cout << i + 1 << ". " << languages[i] << endl;

    int l;
    cin >> l;
    string selectedLang = (l > 0 && l <= languages.size()) ? languages[l - 1] : "*";

    cout << "\n--- FILTERED MOVIES ---\n";
    bool found = false;

    for (const Movie& m : movies) {
        if ((selectedGenre == "*" || m.getGenre() == selectedGenre) &&
            (selectedLang == "*" || m.getLanguage() == selectedLang)) {

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

void UserService::searchMovieByName() {

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string query;
    cout << "Enter movie name: ";
    getline(cin, query);
    query = toLower(query);

    vector<Movie> movies = FileManager::loadMovies();
    vector<Showtime> shows = FileManager::loadShowtimes();

    bool foundAny = false;
    cout << "\n--- SEARCH RESULTS ---\n";

    for (const Movie& m : movies) {
        if (toLower(m.getTitle()).find(query) != string::npos) {
            foundAny = true;
            cout << "\n🎬 " << m.getTitle() << " (" << m.getLanguage() << ")\n";

            bool hasShow = false;
            for (const Showtime& s : shows) {
                if (s.getMovieCode() == m.getCode()) {
                    if (!hasShow) cout << "   ▶ Showtimes:\n";
                    hasShow = true;
                    cout << "     - " << s.getDate()
                         << " | " << s.getTime()
                         << " | Hall " << s.getHallNo() << endl;
                }
            }

            if (!hasShow)
                cout << "   ⚠ Not currently scheduled\n";
        }
    }

    if (!foundAny)
        cout << "No matching movies found.\n";
}



/* ================= TICKET CANCELLATION ================= */

void UserService::cancelTicket() {
    string ticketId;
    int choice;

    // Take ticket ID
    cout << "Enter Ticket ID: ";
    cin >> ticketId;

    // Validate ticket existence
    if (!TicketService::ticketExists(ticketId)) {
        cout << "❌ Invalid Ticket ID. Cancellation failed.\n";
        return;
    }

    // Confirmation prompt
    cout << "\n⚠ This action cannot be undone.\n";
    cout << "Press 1 to confirm cancellation\n";
    cout << "Press 2 to abort\n";
    cout << "Your choice: ";

    // Input validation loop
    while (true) {
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter 1 or 2: ";
        }
        else if (choice == 1) {
            TicketService::cancelTicket(ticketId);
            cout << "✅ Ticket cancelled successfully.\n";
            break;
        }
        else if (choice == 2) {
            cout << "❎ Ticket cancellation aborted.\n";
            break;
        }
        else {
            cout << "Please press 1 (Yes) or 2 (No): ";
        }
    }
}


/* ================= SHOWTIMES ================= */

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

/* ================= BOOK SEAT ================= */

void UserService::bookSeat(const User& user) {

    vector<Showtime> shows = FileManager::loadShowtimes();
    vector<Movie> movies = FileManager::loadMovies();

    if (shows.empty()) {
        cout << "No showtimes available.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string customerName;
    cout << "Enter your full name: ";
    getline(cin, customerName);

    cout << "\n--- AVAILABLE SHOWTIMES ---\n";
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

    int choice;
    cout << "Select showtime: ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > shows.size()) {
        cout << "Invalid selection.\n";
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
    string input;
    getline(cin, input);

    for (char& c : input)
        if (c == ',') c = ' ';

    stringstream ss(input);
    set<string> used;
    vector<pair<int,int>> selectedSeats;

    string seat;
    while (ss >> seat) {
        seat[0] = toupper(seat[0]);
        if (used.count(seat)) return;
        used.insert(seat);

        int r = seat[0] - 'A';
        int c = stoi(seat.substr(1)) - 1;

        if (!map.isSeatAvailable(r, c)) return;
        selectedSeats.push_back({r, c});
    }

    if (selectedSeats.empty()) return;

    int basePrice = selectedSeats.size() * 500;
    int finalPrice = basePrice;
    int discountTk = 0;
    string discountLabel = "None";

    if (PricingEngine::hasGlobalDiscount()) {
        int p = PricingEngine::getGlobalDiscountPercent();
        finalPrice -= basePrice * p / 100;
        discountTk = basePrice - finalPrice;
        discountLabel = "Global Discount";
    } else {
        int p = PricingEngine::getGroupDiscountPercent(selectedSeats.size());
        if (p > 0) {
            char ch;
            cout << "Apply group discount? (y/n): ";
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                finalPrice -= basePrice * p / 100;
                discountTk = basePrice - finalPrice;
                discountLabel = "Group Discount";
            }
        }
    }

    for (auto& s : selectedSeats)
        map.bookSeat(s.first, s.second);

    FileManager::saveSeatMap(
        FileManager::getSeatMapFilename(
            selectedShow.getHallNo(),
            selectedShow.getDate(),
            selectedShow.getTime()
        ),
        map
    );

    string movieName = "Unknown";
    for (const Movie& m : movies)
        if (m.getCode() == selectedShow.getMovieCode())
            movieName = m.getTitle();

    string ticketId = TicketService::generateTicketId();

    TicketService::printTicket(
        ticketId,
        customerName,
        movieName,
        selectedShow.getDate(),
        selectedShow.getTime(),
        selectedShow.getHallNo(),
        selectedSeats,
        basePrice,
        discountTk,
        discountLabel,
        finalPrice
    );

    TicketService::saveTicket(
        ticketId,
        customerName,
        movieName,
        selectedShow.getDate(),
        selectedShow.getTime(),
        selectedShow.getHallNo(),
        selectedSeats,
        finalPrice
    );

    int snackChoice;

    cout << "\nWould you like to order snacks?\n";
    cout << "Press 1 for Yes\n";
    cout << "Press 2 for No\n";
    cout << "Your choice: ";

    while (true) {
        cin >> snackChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter 1 or 2: ";
        }
        else if (snackChoice == 1) {
            SnackShop::start();
            break;
        }
        else if (snackChoice == 2) {
            cout << "Skipping snack order.\n";
            break;
        }
        else {
            cout << "Please press 1 (Yes) or 2 (No): ";
        }
    }

}

/* ================= SEAT RECOMMENDATION ================= */

void UserService::recommendSeat() {

    vector<Showtime> shows = FileManager::loadShowtimes();
    if (shows.empty()) return;

    cout << "\n--- SHOWTIMES ---\n";
    for (size_t i = 0; i < shows.size(); i++)
        cout << i + 1 << ". "
             << shows[i].getDate() << " | "
             << shows[i].getTime()
             << " | Hall " << shows[i].getHallNo() << endl;

    int choice;
    cout << "Select showtime: ";
    cin >> choice;

    if (choice < 1 || choice > shows.size()) return;

    Showtime s = shows[choice - 1];

    int count;
    cout << "Enter seat count: ";
    cin >> count;

    SeatMap map = FileManager::loadOrCreateSeatMap(
        s.getHallNo(), s.getDate(), s.getTime()
    );

    map.display();
    auto seats = map.recommendBestSeats_Custom(count);

    if (seats.empty()) {
        cout << "No recommended seats.\n";
        return;
    }

    cout << "Recommended seats: ";
    for (auto& p : seats)
        cout << char('A' + p.first) << p.second + 1 << " ";
    cout << endl;
}
