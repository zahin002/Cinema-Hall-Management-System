#include "UserService.h"
#include "FileManager.h"
#include "SeatMap.h"
#include "PricingEngine.h"
#include "TerminalColors.h"
#include <iostream>
#include <sstream>
#include <set>
#include <limits>
#include <algorithm>   // for transform
#include <ctime>
#include <iomanip>


using namespace std;

/* ================= USER MENU ================= */

void UserService::userMenu(const User& user) {

    bool isGuest = (user.getRole() == "guest");

    int choice;
    do {
        cout << "\n--- USER MENU ---\n";
        cout << "1. Browse Movies\n";
        cout << "2. Filter Movies (Genre/Language)\n";
        cout << "3. Search Movie by Name\n";
        cout << "4. View Showtimes\n";
        cout << "5. Book Seat\n";
        cout << "6. Recommend Best Seat\n";
        cout << "7. Logout\n";


         if (isGuest)
            cout << "(Guest Mode - Phone: " << user.getEmail() << ")\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewMovies(); break;
            case 2: filterMovies(); break;
            case 3: searchMovieByName(); break;
            case 4: viewShowtimes(); break;
            case 5: bookSeat(user); break;
            case 6: recommendSeat(); break;
            case 7: cout << "Logged out.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
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

    vector<Movie> movies = FileManager::loadMovies();
    if (movies.empty()) {
        cout << "No movies available.\n";
        return;
    }

    // ===== COLLECT UNIQUE GENRES & LANGUAGES =====

    set<string> genreSet, languageSet;

    for (const Movie& m : movies) {
        genreSet.insert(m.getGenre());
        languageSet.insert(m.getLanguage());
    }

    // Convert sets to vectors (for indexed access)
    vector<string> genres(genreSet.begin(), genreSet.end());
    vector<string> languages(languageSet.begin(), languageSet.end());

    // ===== GENRE MENU =====
    cout << "\nChoose Genre:\n";
    cout << "0. All Genres\n";
    for (size_t i = 0; i < genres.size(); i++) {
        cout << i + 1 << ". " << genres[i] << endl;
    }

    int genreChoice;
    cout << "Enter choice: ";
    cin >> genreChoice;

    string selectedGenre = "*";
    if (genreChoice > 0 && genreChoice <= genres.size()) {
        selectedGenre = genres[genreChoice - 1];
    }

    // ===== LANGUAGE MENU =====
    cout << "\nChoose Language:\n";
    cout << "0. All Languages\n";
    for (size_t i = 0; i < languages.size(); i++) {
        cout << i + 1 << ". " << languages[i] << endl;
    }

    int langChoice;
    cout << "Enter choice: ";
    cin >> langChoice;

    string selectedLanguage = "*";
    if (langChoice > 0 && langChoice <= languages.size()) {
        selectedLanguage = languages[langChoice - 1];
    }

    // ===== FILTER & DISPLAY MOVIES =====
    bool found = false;
    cout << "\n--- FILTERED MOVIES ---\n";

    for (const Movie& m : movies) {
        bool genreMatch =
            (selectedGenre == "*" || m.getGenre() == selectedGenre);

        bool languageMatch =
            (selectedLanguage == "*" || m.getLanguage() == selectedLanguage);

        if (genreMatch && languageMatch) {
            cout << m.getCode() << " | "
                 << m.getTitle() << " | "
                 << m.getGenre() << " | "
                 << m.getLanguage() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No movies found for selected filters.\n";
    }
}

void UserService::searchMovieByName() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string query;
    cout << "Enter movie name to search: ";
    getline(cin, query);

    query = toLower(query);

    vector<Movie> movies = FileManager::loadMovies();
    vector<Showtime> shows = FileManager::loadShowtimes();

    bool foundAny = false;

    cout << "\n--- SEARCH RESULTS ---\n";

    for (const Movie& m : movies) {
        string titleLower = toLower(m.getTitle());

        if (titleLower.find(query) != string::npos) {
            foundAny = true;

            cout << "\n🎬 " << m.getTitle()
                 << " | " << m.getGenre()
                 << " | " << m.getLanguage() << endl;

            bool hasShowtime = false;

            for (const Showtime& s : shows) {
                if (s.getMovieCode() == m.getCode()) {
                    if (!hasShowtime) {
                        cout << "   ▶ Showtimes:\n";
                        hasShowtime = true;
                    }
                    cout << "     - "
                         << s.getDate()
                         << " | " << s.getTime()
                         << " | Hall " << s.getHallNo() << endl;
                }
            }

            if (!hasShowtime) {
                cout << "   ⚠ Available in database but currently not on showtime\n";
            }
        }
    }

    if (!foundAny) {
        cout << "No matching movies found.\n";
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

//Unique Ticket ID Generator//

string generateTicketId() {
    return "TCK" + to_string(time(nullptr)) + "_" + to_string(rand() % 1000);
}


/* Atomic multi-seat booking */

void UserService::bookSeat(const User& user) {

    vector<Showtime> shows = FileManager::loadShowtimes();
    vector<Movie> movies = FileManager::loadMovies();

    if (shows.empty()) {
        cout << "No showtimes available.\n";
        return;
    }

    // ===== ASK CUSTOMER NAME =====

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string customerName;
    cout << "Enter your full name: ";
    getline(cin, customerName);


    // ===== SHOWTIME SELECTION =====

    cout << "\n--- AVAILABLE SHOWTIMES ---\n";
    for (size_t i = 0; i < shows.size(); i++) {
        string title = "Unknown";
        for (const Movie& m : movies) {
            if (m.getCode() == shows[i].getMovieCode()) {
                title = m.getTitle();
                break;
            }
        }
        cout << i + 1 << ". "
             << title << " | "
             << shows[i].getDate() << " | "
             << shows[i].getTime() << " | Hall "
             << shows[i].getHallNo() << endl;
    }

    int choice;
    cout << "Select showtime (number): ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > shows.size()) {
        cout << "Invalid showtime selection.\n";
        return;
    }

    Showtime selectedShow = shows[choice - 1];

    // ===== LOAD SEAT MAP =====

    SeatMap map = FileManager::loadOrCreateSeatMap(
        selectedShow.getHallNo(),
        selectedShow.getDate(),
        selectedShow.getTime()
    );

    map.display();

    // ===== SEAT INPUT =====

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
        selectedSeats.push_back({row, col});
    }

    if (selectedSeats.empty()) {
        cout << "No seats selected.\n";
        return;
    }

    // ===== PRICING =====

    int seatCount = selectedSeats.size();
    int basePrice = seatCount * 500;
    int finalPrice = basePrice;
    int discountTk = 0;
    string discountLabel = "None";

    // GLOBAL DISCOUNT (TOP PRIORITY)

    if (PricingEngine::hasGlobalDiscount()) {
        int percent = PricingEngine::getGlobalDiscountPercent();
        string msg = PricingEngine::getGlobalDiscountMessage();

        cout << "\n" << BOLD << YELLOW << "🎉 GLOBAL OFFER 🎉\n"
             << msg << "\n"
             << percent << "% discount applied automatically!\n"
             << RESET;

        finalPrice = basePrice - (basePrice * percent / 100);
        discountTk = basePrice - finalPrice;
        discountLabel = "Global Discount";

    } else {

        // GROUP DISCOUNT (ASK USER)

        int groupPercent = PricingEngine::getGroupDiscountPercent(seatCount);
        if (groupPercent > 0) {
            char ch;
            cout << "You are eligible for "
                 << groupPercent << "% group discount. Apply? (y/n): ";
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                finalPrice = basePrice - (basePrice * groupPercent / 100);
                discountTk = basePrice - finalPrice;
                discountLabel = "Group Discount";
            }
        }
    }

    // ===== BOOK SEATS =====

    for (auto& s : selectedSeats)
        map.bookSeat(s.first, s.second);

    string seatFile = FileManager::getSeatMapFilename(
        selectedShow.getHallNo(),
        selectedShow.getDate(),
        selectedShow.getTime()
    );
    FileManager::saveSeatMap(seatFile, map);

    // ===== TICKET ID =====

    string ticketId = generateTicketId();

    string movieName;
    for (const Movie& m : movies) {
        if (m.getCode() == selectedShow.getMovieCode()) {
            movieName = m.getTitle();
            break;
        }
    }

    int seatsCount = selectedSeats.size();


    // ===== COLORED TICKET OUTPUT =====
    
    cout << "\n";
    cout << BOLD << CYAN << "====================================================\n" << RESET;
    cout << BOLD << YELLOW << "                 🎟  CINE++ TICKET\n" << RESET;
    cout << BOLD << CYAN << "====================================================\n" << RESET;

    cout << left;
    cout << BOLD << setw(15) << "Ticket ID" << ": " << RESET << ticketId << endl;
    cout << BOLD << setw(15) << "Name"      << ": " << RESET << customerName << endl;

    cout << BOLD << setw(15) << "Movie" << ": " << RESET;
    for (const Movie& m : movies)
        if (m.getCode() == selectedShow.getMovieCode())
            cout << m.getTitle();
    cout << endl;

    cout << BOLD << setw(15) << "Showtime" << ": " << RESET
         << selectedShow.getDate() << " | " << selectedShow.getTime() << endl;

    cout << BOLD << setw(15) << "Hall No" << ": " << RESET
         << selectedShow.getHallNo() << endl;

    cout << BOLD << setw(15) << "Seats" << ": " << RESET;
    for (auto& s : selectedSeats)
        cout << char('A' + s.first) << s.second + 1 << " ";
    cout << endl;

    cout << "----------------------------------------------------\n";
    cout << setw(15) << "Base Price" << ": " << basePrice << " Tk\n";

    if (discountTk > 0)
        cout << setw(15) << discountLabel << ": -" << discountTk << " Tk\n";

    cout << BOLD << RED << setw(15) << "Total Paid" << ": "
         << finalPrice << " Tk\n" << RESET;

    cout << BOLD << CYAN << "====================================================\n" << RESET;
    cout << GREEN << "Status: CONFIRMED ✔\n" << RESET;
    cout << BOLD << CYAN << "====================================================\n" << RESET;

    // ===== SAVE TICKET =====
    FileManager::saveTicket(
    ticketId,
    customerName,
    movieName,
    selectedShow.getDate(),
    selectedShow.getTime(),
    selectedShow.getHallNo(),
    seatsCount,
    basePrice,
    discountTk,
    finalPrice
    );

}


/* ================= SEAT RECOMMENDATION ================= */

/* Suggests best seat(s) using center-priority algorithm */

void UserService::recommendSeat() {

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
    cout << "Select showtime: ";
    cin >> choice;

    if (choice < 1 || choice > shows.size()) {
        cout << "Invalid showtime selection.\n";
        return;
    }

    Showtime selectedShow = shows[choice - 1];

    int count;
    cout << "Enter number of seats: ";
    cin >> count;

    SeatMap map = FileManager::loadOrCreateSeatMap(
        selectedShow.getHallNo(),
        selectedShow.getDate(),
        selectedShow.getTime()
    );

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

void printTicket(
    const string& customerName,
    const string& movieName,
    const Showtime& show,
    const vector<pair<int,int>>& seats,
    int basePrice,
    int discountPercent,
    int finalPrice,
    const string& ticketId
) {
    cout << endl;
    cout << BOLD << CYAN;
    cout << "============================================================\n";
    cout << "                      CINE++ MOVIE TICKET                   \n";
    cout << "============================================================\n";
    cout << RESET;

    cout << BOLD << " Ticket ID   : " << RESET << ticketId << endl;
    cout << BOLD << " Name        : " << RESET << customerName << endl;
    cout << BOLD << " Movie       : " << RESET << movieName << endl;
    cout << BOLD << " Showtime    : " << RESET
         << show.getDate() << " | " << show.getTime() << endl;
    cout << BOLD << " Hall No     : " << RESET << show.getHallNo() << endl;

    cout << BOLD << " Seats       : " << RESET;
    for (auto& s : seats)
        cout << char('A' + s.first) << s.second + 1 << " ";
    cout << endl;

    cout << "------------------------------------------------------------\n";

    cout << " Base Price  : " << basePrice << " Tk\n";
    cout << " Discount    : " << discountPercent << "%\n";

    cout << BOLD << GREEN;
    cout << " Total Paid  : " << finalPrice << " Tk\n";
    cout << RESET;

    cout << "============================================================\n";
    cout << YELLOW << "      🎬 Enjoy Your Movie at CINE++ 🎬\n" << RESET;
    cout << "============================================================\n\n";
}
