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
        cout << BOLD << m.getCode() << RESET
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

    
    ReviewService::showReviews(movieCode);

    if (user.getRole() == "user") {
        cout << "\n1. Give / Update Rating\n"
             << "2. Add / Update Review\n"
             << "3. Delete My Review\n"
             << "0. Back\n"
             << YELLOW << "Enter choice: " << RESET;

        int ch;
        cin >> ch;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (ch == 1)
            RatingService::giveOrUpdateRating(movieCode, user);
        else if (ch == 2)
            ReviewService::addOrUpdateReview(movieCode, user);
        else if (ch == 3)
            ReviewService::deleteOwnReview(movieCode, user);
    }
    else if (user.getRole() == "admin") {
        cout << "\n" << RED << "Admin Options\n" << RESET
             << "1. Delete a Review\n"
             << "0. Back\n"
             << YELLOW << "Enter choice: " << RESET;

        int ch;
        cin >> ch;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (ch == 1)
            ReviewService::deleteAnyReview(movieCode);
    }

    cout << BOLD << CYAN
         << "===================================\n"
         << RESET;
}

/* ================= TRENDING MOVIES ================= */

void UserService::showTrendingMovies() {

    ifstream file("../data/ratings.txt");
    if (!file.is_open()) {
        cout << YELLOW << "No ratings available yet.\n" << RESET;
        return;
    }

    map<int, pair<string, pair<double, int>>> ratingMap;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string codeStr, movieName, email, ratingStr, dateTime;

        getline(ss, codeStr, '|');
        getline(ss, movieName, '|');
        getline(ss, email, '|');
        getline(ss, ratingStr, '|');
        getline(ss, dateTime);

        int code = stoi(codeStr);
        int rating = stoi(ratingStr);

        ratingMap[code].first = movieName;
        ratingMap[code].second.first += rating;
        ratingMap[code].second.second += 1;
    }

    struct Trend {
        int code;
        string name;
        double avg;
        int count;
    };

    vector<Trend> trends;

    for (auto& it : ratingMap) {
        double avg = it.second.second.first / it.second.second.second;
        trends.push_back({
            it.first,
            it.second.first,
            avg,
            it.second.second.second
        });
    }

    if (trends.empty()) {
        cout << YELLOW << "No ratings available yet.\n" << RESET;
        return;
    }

    sort(trends.begin(), trends.end(),
         [](const Trend& a, const Trend& b) {
             return a.avg > b.avg;
         });

    cout << "\n" << BOLD << CYAN << "🔥 TRENDING MOVIES 🔥\n" << RESET;
    cout << BOLD << CYAN << "----------------------------------\n" << RESET;

    int limit = min(10, (int)trends.size());
    for (int i = 0; i < limit; i++) {
        cout << BOLD << (i + 1) << ". "
             << trends[i].code << " | "
             << trends[i].name << RESET
             << "  ⭐ " << fixed << setprecision(1)
             << trends[i].avg
             << " (" << trends[i].count << " ratings)\n";
    }

    cout << BOLD << CYAN << "----------------------------------\n" << RESET;
}

/* ================= GENRE RECOMMENDATION ================= */

void UserService::showGenreRecommendations(int currentMovieCode, const string& genre) {

    vector<Movie> movies = FileManager::loadMovies();
    vector<pair<Movie, double>> candidates;

    for (const Movie& m : movies) {
        if (m.getGenre() == genre && m.getCode() != currentMovieCode) {
            int count = 0;
            double avg = RatingService::getAverageRating(m.getCode(), count);
            candidates.push_back({ m, avg });
        }
    }

    if (candidates.empty()) {
        cout << YELLOW
             << "\n⚠ No similar movies found in this genre.\n"
             << RESET;
        return;
    }

    sort(candidates.begin(), candidates.end(),
        [](auto& a, auto& b) {
            return a.second > b.second;
        });

    cout << BOLD << CYAN
         << "\n🎬 You may also like (Same Genre)\n"
         << "---------------------------------\n"
         << RESET;

    int shown = 0;
    for (auto& item : candidates) {
        cout << "• "
             << item.first.getCode()
             << " | " << item.first.getTitle();

        if (item.second > 0)
            cout << " ⭐ " << fixed << setprecision(1) << item.second;

        cout << endl;
        if (++shown == 5) break;
    }
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
        cout << "\n[Movie] " << m.getCode() << "] "
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

    string customerName = user.getFullName();

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
