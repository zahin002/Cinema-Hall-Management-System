#include "UserService.h"
#include "FileManager.h"
#include "SeatMap.h"
#include "PricingEngine.h"
#include "TerminalColors.h"
#include "SnackShop.h"
#include "RatingService.h"
#include "ReviewService.h"
#include "TicketService.h"

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
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

string normalize(const string& s) {
    string result = s;

    // trim leading/trailing spaces
    result.erase(0, result.find_first_not_of(" \t\r\n"));
    result.erase(result.find_last_not_of(" \t\r\n") + 1);

    // optional: make consistent case
    transform(result.begin(), result.end(), result.begin(), ::tolower);

    return result;
}

string capitalizeFirstLetter(const string& s) {
    if (s.empty()) return s;

    string result = s;
    result[0] = toupper(result[0]);
    return result;
}

/* ================= MOVIES ================= */

void UserService::viewMovies() {

    vector<Movie> movies = FileManager::loadMovies();

    if (movies.empty()) {
        cout << YELLOW << "No movies available.\n" << RESET;
        return;
    }

    cout << "\n" << BOLD << CYAN << "--- MOVIE LIST ---\n" << RESET;

    for (const Movie& m : movies) {
        cout << BOLD << YELLOW << m.getCode() << RESET
             << " - " << m.getTitle() << "\n";
    }

    cout << BOLD << CYAN << "------------------\n" << RESET;
}

/* ================= MOVIE DETAILS ================= */

void UserService::showMovieDetails(int movieCode, const User& user) {

    vector<Movie> movies = FileManager::loadMovies();
    vector<Showtime> shows = FileManager::loadShowtimes();

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
        cout << RED << "Invalid movie code.\n" << RESET;
        return;
    }

    int ch;

    do {

        cout << "\n" << BOLD << CYAN
             << "========== MOVIE DETAILS ==========\n"
             << RESET;

        cout << BOLD << YELLOW << selected.getTitle() << RESET << "\n";
        cout << CYAN << "-----------------------------------\n" << RESET;

        cout << "Duration : " << selected.getDuration() << " minutes\n";
        cout << "Language : " << selected.getLanguage() << "\n";
        cout << "Genre    : " << selected.getGenre() << "\n";

        int ratingCount = 0;
        double avg = RatingService::getAverageRating(movieCode, ratingCount);

        cout << "\n";
        if (ratingCount > 0) {
            cout << GREEN << "Average Rating: "
                 << fixed << setprecision(1) << avg
                 << " / 5 (" << ratingCount << " ratings)\n"
                 << RESET;
        } else {
            cout << YELLOW << "No ratings yet\n" << RESET;
        }

        cout << "\n" << BOLD << CYAN << "Showtimes:\n" << RESET;

        bool hasShow = false;
        for (const Showtime& s : shows) {
            if (s.getMovieCode() == movieCode) {
                hasShow = true;
                cout << "• " << s.getDate()
                     << " | " << s.getTime()
                     << " | Hall " << s.getHallNo() << "\n";
            }
        }

        if (!hasShow)
            cout << YELLOW << "Not currently scheduled\n" << RESET;

        cout << "\n";

        // ===== SHOW REVIEWS =====
        ReviewService::showReviews(movieCode);

        // ===== USER OPTIONS =====
        if (user.getRole() == "user") {

            cout << "\n"
                 << "1. Give / Update Rating\n"
                 << "2. Add / Update Review\n"
                 << "3. Delete My Review\n"
                 << "0. Back\n"
                 << YELLOW << "Enter choice: " << RESET;

            cin >> ch;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (ch == 1)
                RatingService::giveOrUpdateRating(movieCode, user);
            else if (ch == 2)
                ReviewService::addOrUpdateReview(movieCode, user);
            else if (ch == 3)
                ReviewService::deleteOwnReview(movieCode, user);

        }
        // ===== ADMIN OPTIONS =====
        else if (user.getRole() == "admin") {

            cout << "\n" << RED << "Admin Options\n" << RESET
                 << "1. Delete a Review\n"
                 << "0. Back\n"
                 << YELLOW << "Enter choice: " << RESET;

            cin >> ch;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (ch == 1)
                ReviewService::deleteAnyReview(movieCode);
        }

        cout << BOLD << CYAN
             << "===================================\n"
             << RESET;

    } while (ch != 0);
}

/* ================= TRENDING MOVIES ================= */

void UserService::showTrendingMovies() {

    ifstream file("../data/ratings.txt");
    if (!file.is_open()) {
        cout << YELLOW << "No ratings available yet.\n" << RESET;
        return;
    }

    // movieCode -> { movieName, { totalRating, ratingCount } }
    map<int, pair<string, pair<double, int>>> ratingMap;

    string line;

    while (getline(file, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);
        string codeStr, movieName, email, ratingStr, dateTime;

        getline(ss, codeStr, '|');
        getline(ss, movieName, '|');
        getline(ss, email, '|');
        getline(ss, ratingStr, '|');
        getline(ss, dateTime);

        try {
            int code = stoi(codeStr);
            int rating = stoi(ratingStr);

            if (rating < 1 || rating > 5)
                continue;

            ratingMap[code].first = movieName;
            ratingMap[code].second.first += rating;   // total rating
            ratingMap[code].second.second += 1;       // count
        }
        catch (...) {
            continue; // skip corrupted lines
        }
    }

    file.close();

    struct Trend {
        int code;
        string name;
        double avg;
    };

    vector<Trend> trends;

    for (auto& it : ratingMap) {

        int count = it.second.second.second;

        // REQUIRE AT LEAST 10 RATINGS
        if (count < 10)
            continue;

        double avg =
            static_cast<double>(it.second.second.first) /
            static_cast<double>(count);

        trends.push_back({
            it.first,
            it.second.first,
            avg
        });
    }

    if (trends.empty()) {
        cout << YELLOW << "No movies qualify as trending yet.\n" << RESET;
        return;
    }

    sort(trends.begin(), trends.end(),
         [](const Trend& a, const Trend& b) {
             return a.avg > b.avg;
         });

    cout << "\n" << BOLD << CYAN << " TRENDING MOVIES\n" << RESET;
    cout << BOLD << CYAN << "----------------------------------\n" << RESET;

    int limit = min(10, (int)trends.size());

    for (int i = 0; i < limit; i++) {

        cout << BOLD << (i + 1) << ". "
             << YELLOW << trends[i].code << RESET
             << " | "
             << BOLD << trends[i].name << RESET
             << "  "
             << CYAN << "Rating: " << RESET
             << fixed << setprecision(2)
             << trends[i].avg
             << "\n";
    }

    cout << BOLD << CYAN << "----------------------------------\n" << RESET;
}


/* ================= FILTER MOVIES ================= */

void UserService::filterMovies() {

    vector<Movie> movies = FileManager::loadMovies();
    if (movies.empty()) {
        cout << YELLOW << "No movies available.\n" << RESET;
        return;
    }

    set<string> genreSet, languageSet;
    for (const Movie& m : movies) {
        genreSet.insert(normalize(m.getGenre()));
        languageSet.insert(normalize(m.getLanguage()));
    }

    vector<string> genres(genreSet.begin(), genreSet.end());
    vector<string> languages(languageSet.begin(), languageSet.end());

    /* ---------- GENRE SELECTION ---------- */
    cout << "\nChoose Genre:\n0. All\n";
    for (size_t i = 0; i < genres.size(); i++)
        cout << i + 1 << ". " << capitalizeFirstLetter(genres[i]) << "\n";

    int g;
    cout << YELLOW << "Enter choice: " << RESET;
    if (!(cin >> g)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input.\n" << RESET;
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string selectedGenre =
        (g > 0 && g <= (int)genres.size()) ? genres[g - 1] : "*";

    /* ---------- LANGUAGE SELECTION ---------- */
    cout << "\nChoose Language:\n0. All\n";
    for (size_t i = 0; i < languages.size(); i++)
        cout << i + 1 << ". " << capitalizeFirstLetter(languages[i]) << "\n";

    int l;
    cout << YELLOW << "Enter choice: " << RESET;
    if (!(cin >> l)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input.\n" << RESET;
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string selectedLang =
        (l > 0 && l <= (int)languages.size()) ? languages[l - 1] : "*";

    /* ---------- FILTERED RESULTS ---------- */
    cout << "\n" << BOLD << CYAN << "--- FILTERED MOVIES ---\n" << RESET;
    bool found = false;

    for (const Movie& m : movies) {
        if ((selectedGenre == "*" || normalize(m.getGenre()) == selectedGenre) &&
            (selectedLang == "*" || normalize(m.getLanguage()) == selectedLang)) {

            cout << m.getCode() << " | " << m.getTitle() << "\n";
            found = true;
        }
    }

    if (!found)
        cout << YELLOW << "No movies found.\n" << RESET;
}

/* ================= SEARCH BY KEYWORD ================= */

void UserService::searchMovieByName() {

    string query;
    cout << CYAN << "Enter movie name: " << RESET;
    getline(cin, query);

    if (query.empty()) {
        cout << RED << "Search keyword cannot be empty.\n" << RESET;
        return;
    }

    // convert full query to lowercase
    query = toLower(query);

    // split query into words
    stringstream ss(query);
    vector<string> tokens;
    string word;

    while (ss >> word) {
        tokens.push_back(word);
    }

    vector<Movie> movies = FileManager::loadMovies();
    vector<Showtime> shows = FileManager::loadShowtimes();

    bool foundAny = false;
    cout << "\n" << BOLD << CYAN << "--- SEARCH RESULTS ---\n" << RESET;

    for (const Movie& m : movies) {

        string titleLower = toLower(m.getTitle());
        bool matched = false;

        // match ANY word in query
        for (const string& token : tokens) {
            if (titleLower.find(token) != string::npos) {
                matched = true;
                break;
            }
        }

        if (!matched) continue;

        foundAny = true;
        cout << "\n[Movie] [" << m.getCode() << "] "
             << BOLD << m.getTitle() << RESET
             << " (" << m.getLanguage() << ")\n";

        bool hasShow = false;
        for (const Showtime& s : shows) {
            if (s.getMovieCode() == m.getCode()) {
                if (!hasShow) {
                    cout << "   ▶ Showtimes:\n";
                    hasShow = true;
                }
                cout << "     - " << s.getDate()
                     << " | " << s.getTime()
                     << " | Hall " << s.getHallNo() << "\n";
            }
        }

        if (!hasShow)
            cout << "   [!] Not currently scheduled\n";
    }

    if (!foundAny)
        cout << YELLOW << "No matching movies found.\n" << RESET;
}



/* ================= TICKET CANCELLATION ================= */


void UserService::cancelTicket(const User& user) {

    vector<string> tickets = FileManager::loadAllTickets();

    vector<string> userTickets;
    vector<vector<string>> parsed;

    // ===== FILTER USER TICKETS =====
    for (const string& record : tickets) {

        if (record.empty()) continue;

        stringstream ss(record);
        vector<string> parts;
        string temp;

        while (getline(ss, temp, '|'))
            parts.push_back(temp);

        if (parts.size() < 7) continue;

        // Skip cancelled tickets
        if (parts.size() >= 9 && parts[8] == "CANCELLED")
            continue;

        if (parts[1] == user.getFullName()) {
            userTickets.push_back(record);
            parsed.push_back(parts);
        }
    }

    // ===== NO TICKETS =====
    if (userTickets.empty()) {
        cout << YELLOW << "You have no active tickets.\n" << RESET;
        return;
    }

    // ===== SHOW LIST (WITH SEATS) =====
    cout << "\n" << BOLD << CYAN
         << "======= YOUR TICKETS =======\n"
         << RESET;

    for (size_t i = 0; i < parsed.size(); i++) {

        auto& p = parsed[i];

        cout << GREEN << i + 1 << ". " << RESET
             << p[0] << " | "
             << p[2] << " | "
             << p[3] << " | "
             << p[4] << " | Hall "
             << p[5] << " | Seats: "
             << CYAN << p[6] << RESET << "\n";
    }

    // ===== SELECT =====
    cout << YELLOW << "\nSelect ticket number: " << RESET;

    int selection;

    if (!(cin >> selection)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << RED << "Invalid input.\n" << RESET;
        return;
    }

    if (selection < 1 || selection > (int)userTickets.size()) {
        cout << RED << "Invalid selection.\n" << RESET;
        return;
    }

    auto& selectedParts = parsed[selection - 1];

    string ticketId = selectedParts[0];
    string movie    = selectedParts[2];
    string date     = selectedParts[3];
    string time     = selectedParts[4];
    string hall     = selectedParts[5];
    string seats    = selectedParts[6];

    // ===== VALIDATE =====
    if (!TicketService::ticketExists(ticketId)) {
        cout << RED << "Invalid Ticket ID. Cancellation failed.\n" << RESET;
        return;
    }

    // ===== SHOW WHAT WILL BE RELEASED =====
    cout << "\n" << BOLD << CYAN
         << "===== CANCELLATION DETAILS =====\n"
         << RESET;

    cout << "Ticket ID : " << ticketId << "\n";
    cout << "Movie     : " << movie << "\n";
    cout << "Showtime  : " << date << " | " << time << "\n";
    cout << "Hall      : " << hall << "\n";

    cout << "Seats to be released: "
         << GREEN << seats << RESET << "\n";

    cout << BOLD << CYAN
         << "=================================\n"
         << RESET;

    // ===== CONFIRMATION =====
    cout << RED << "\nThis action cannot be undone.\n" << RESET;

    cout << YELLOW
         << "Press 1 to confirm cancellation\n"
         << "Press 2 to abort\n"
         << "Your choice: "
         << RESET;

    int choice;

    while (true) {

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << RED << "Invalid input. Enter 1 or 2: " << RESET;
        }
        else if (choice == 1) {

            TicketService::cancelTicket(ticketId);

            cout << GREEN
                 << "\nSeats released successfully: "
                 << seats << "\n"
                 << "Ticket cancelled successfully.\n"
                 << RESET;

            break;
        }
        else if (choice == 2) {

            cout << CYAN
                 << "Ticket cancellation aborted.\n"
                 << RESET;

            break;
        }
        else {
            cout << YELLOW << "Press 1 or 2: " << RESET;
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
        cout << RED << "No showtimes available.\n" << RESET;
        return;
    }

    string customerName = user.getFullName();

    cout << "\n" << BOLD << CYAN
         << "========== AVAILABLE SHOWTIMES ==========\n"
         << RESET;

    for (size_t i = 0; i < shows.size(); i++) {

        string title = "Unknown";

        for (const Movie& m : movies)
            if (m.getCode() == shows[i].getMovieCode())
                title = m.getTitle();

        cout << YELLOW << i + 1 << ". " << RESET
             << title << " | "
             << shows[i].getDate() << " | "
             << shows[i].getTime() << " | Hall "
             << shows[i].getHallNo() << endl;
    }

    int choice;
    cout << YELLOW << "Select showtime: " << RESET;

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input.\n" << RESET;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice < 1 || choice > (int)shows.size()) {
        cout << RED << "Invalid selection.\n" << RESET;
        return;
    }

    Showtime selectedShow = shows[choice - 1];

    SeatMap map = FileManager::loadOrCreateSeatMap(
        selectedShow.getHallNo(),
        selectedShow.getDate(),
        selectedShow.getTime()
    );

    cout << "\n" << CYAN << "Current Seat Map:\n" << RESET;
    map.display();

    cout << YELLOW << "Enter seats (A1 A2 or A1,B5): " << RESET;

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
            cout << RED << "Duplicate seat: " << seat << "\n" << RESET;
            return;
        }

        used.insert(seat);

        try {
            int r = seat[0] - 'A';
            int c = stoi(seat.substr(1)) - 1;

            if (!map.isSeatAvailable(r, c)) {
                cout << RED << "Seat not available: " << seat << "\n" << RESET;
                return;
            }

            selectedSeats.push_back({r, c});

        } catch (...) {
            cout << RED << "Invalid seat format: " << seat << "\n" << RESET;
            return;
        }
    }

    if (selectedSeats.empty()) {
        cout << RED << "No seats selected.\n" << RESET;
        return;
    }

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

            int ch;
            cout << YELLOW
                 << "Apply group discount?\n1. Yes\n2. No\nChoice: "
                 << RESET;

            cin >> ch;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (ch == 1) {
                finalPrice -= basePrice * p / 100;
                discountTk = basePrice - finalPrice;
                discountLabel = "Group Discount";
            }
        }
    }

    // ===== BOOK SEATS =====
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

    // ===== PRINT =====
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

    // ===== SAVE (FIXED PATH INSIDE SERVICE) =====
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

    // ===== SNACK =====
    int snackChoice;

    cout << "\n" << CYAN << "Would you like to order snacks?\n" << RESET;
    cout << YELLOW << "1. Yes\n2. No\nChoice: " << RESET;

    while (true) {

        if (!(cin >> snackChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Enter 1 or 2: " << RESET;
            continue;
        }

        if (snackChoice == 1) {
            SnackShop::start();
            break;
        }
        else if (snackChoice == 2) {
            cout << CYAN << "Skipping snacks.\n" << RESET;
            break;
        }
        else {
            cout << YELLOW << "Enter 1 or 2: " << RESET;
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
