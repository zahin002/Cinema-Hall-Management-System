#include "UserService.h"
#include "FileManager.h"
#include "SeatMap.h"
#include "PricingEngine.h"
#include "TerminalColors.h"
#include "SnackShop.h"
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
        cout << RED << "❌ Invalid movie code.\n" << RESET;
        return;
    }

    cout << "\n";
    cout << BOLD << CYAN << "========== MOVIE DETAILS ==========\n" << RESET;
    cout << BOLD << YELLOW << selected.getTitle() << RESET << "\n";
    cout << BOLD << CYAN << "===================================\n" << RESET;

    cout << "Duration : " << selected.getDuration() << " minutes\n";
    cout << "Language : " << selected.getLanguage() << "\n";
    cout << "Genre    : " << selected.getGenre() << "\n";

    // ⭐ RATINGS
    int ratingCount = 0;
    double avg = getAverageRating(movieCode, ratingCount);

    cout << "\n";
    if (ratingCount > 0) {
        cout << BOLD << GREEN
             << "⭐ Average Rating: "
             << fixed << setprecision(1) << avg
             << " / 5 (" << ratingCount << " ratings)\n"
             << RESET;
    } else {
        cout << YELLOW << "⭐ No ratings yet\n" << RESET;
    }

    // 🎬 SHOWTIMES
    bool hasShow = false;
    cout << "\n" << BOLD << CYAN << "Showtimes:\n" << RESET;

    for (const Showtime& s : shows) {
        if (s.getMovieCode() == movieCode) {
            hasShow = true;
            cout << " • " << s.getDate()
                 << " | " << s.getTime()
                 << " | Hall " << s.getHallNo() << "\n";
        }
    }

    if (!hasShow) {
        cout << YELLOW << "⚠ Currently not in showtime\n" << RESET;
    }

    if (user.getRole() != "guest") {
        cout << "\n1. Give / Update Rating\n2. Back\nEnter choice: ";
        int ch;
        cin >> ch;
        if (ch == 1)
            giveOrUpdateRating(movieCode, user);
    }

    cout << BOLD << CYAN << "===================================\n" << RESET;
}



/* ================= TRENDING MOVIES ================= */


void UserService::showTrendingMovies() {

    ifstream file("../data/ratings.txt");
    if (!file.is_open()) {
        cout << YELLOW << "No ratings available yet.\n" << RESET;
        return;
    }

    // movieCode -> { movieName, totalRating, count }
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
    file.close();

    // Convert to sortable vector
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

    // Sort by average rating (descending)
    sort(trends.begin(), trends.end(),
         [](const Trend& a, const Trend& b) {
             return a.avg > b.avg;
         });

    cout << "\n";
    cout << BOLD << CYAN << "🔥 TRENDING MOVIES 🔥\n" << RESET;
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
            double avg = getAverageRating(m.getCode(), count);

            // Even if no rating, include with 0
            candidates.push_back({ m, avg });
        }
    }

    if (candidates.empty()) {
        cout << YELLOW
             << "\n⚠ No similar movies found in this genre.\n"
             << RESET;
        return;
    }

    // Sort by rating (descending)
    sort(candidates.begin(), candidates.end(),
        [](auto& a, auto& b) {
            return a.second > b.second;
        }
    );

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


/* ================= AVERAGE RATING ================= */


double UserService::getAverageRating(int movieCode, int& count) {
    ifstream file("../data/ratings.txt");
    count = 0;
    double total = 0;

    if (!file.is_open())
        return 0;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string codeStr, movieName, email, ratingStr, dt;

        getline(ss, codeStr, '|');
        getline(ss, movieName, '|');
        getline(ss, email, '|');
        getline(ss, ratingStr, '|');
        getline(ss, dt);

        if (stoi(codeStr) == movieCode) {
            total += stoi(ratingStr);
            count++;
        }
    }
    file.close();

    return (count == 0) ? 0 : total / count;
}


/* ================= GIVE / UPDATE RATING  ================= */


void UserService::giveOrUpdateRating(int movieCode, const User& user) {

    int rating;

    cout << "Enter rating (1–5): ";

    // strict integer input check

    if (!(cin >> rating)) {
        cin.clear();        // clear error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input. Rating must be an integer.\n" << RESET;
        return;
    }

    if (rating < 1 || rating > 5) {
        cout << RED << "Rating must be between 1 and 5.\n" << RESET;
        return;
    }

    vector<string> lines;
    ifstream in("../data/ratings.txt");
    string line;
    bool updated = false;

    while (getline(in, line)) {
        stringstream ss(line);
        string codeStr, movieName, email, rateStr, dt;

        getline(ss, codeStr, '|');
        getline(ss, movieName, '|');
        getline(ss, email, '|');
        getline(ss, rateStr, '|');
        getline(ss, dt);

        if (stoi(codeStr) == movieCode && email == user.getEmail()) {
            time_t now = time(nullptr);
            string t = ctime(&now);
            t.pop_back();

            line = codeStr + "|" + movieName + "|" +
                   email + "|" + to_string(rating) + "|" + t;

            updated = true;
        }

        lines.push_back(line);
    }
    in.close();

    if (!updated) {
        vector<Movie> movies = FileManager::loadMovies();
        string movieName;

        for (const Movie& m : movies)
            if (m.getCode() == movieCode)
                movieName = m.getTitle();

        time_t now = time(nullptr);
        string t = ctime(&now);
        t.pop_back();

        lines.push_back(
            to_string(movieCode) + "|" +
            movieName + "|" +
            user.getEmail() + "|" +
            to_string(rating) + "|" + t
        );
    }

    ofstream out("../data/ratings.txt");
    for (const string& l : lines)
        out << l << "\n";
    out.close();

    cout << GREEN << "✔ Rating saved successfully.\n" << RESET;
}


/* ================= DISPLAY REVIEWS ================= */

void UserService::showReviews(int movieCode) {

    ifstream file("../data/reviews.txt");
    if (!file.is_open()) {
        cout << YELLOW << "📝 No reviews yet.\n" << RESET;
        return;
    }

    struct Review {
        string name;
        string text;
        string datetime;
    };

    vector<Review> reviews;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string codeStr, movieName, reviewer, reviewText, dt;

        getline(ss, codeStr, '|');
        getline(ss, movieName, '|');
        getline(ss, reviewer, '|');
        getline(ss, reviewText, '|');
        getline(ss, dt);

        if (stoi(codeStr) == movieCode) {
            reviews.push_back({reviewer, reviewText, dt});
        }
    }
    file.close();

    if (reviews.empty()) {
        cout << YELLOW << "📝 No reviews yet.\n" << RESET;
        return;
    }

    // newest → oldest (string datetime works since format is consistent)

    sort(reviews.begin(), reviews.end(),
         [](const Review& a, const Review& b) {
             return a.datetime > b.datetime;
         });

    cout << "\n" << BOLD << CYAN << "📝 REVIEWS\n" << RESET;

    for (auto& r : reviews) {
        cout << "[" << r.datetime << "] "
             << BOLD << r.name << RESET << ":\n";
        cout << r.text << "\n\n";
    }
}


/* ================= ADD / UPDATE REVIEWS ================= */


void UserService::addOrUpdateReview(int movieCode, const User& user) {

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Write your review:\n> ";
    string review;
    getline(cin, review);

    if (review.empty()) {
        cout << RED << "Review cannot be empty.\n" << RESET;
        return;
    }

    vector<string> lines;
    ifstream in("../data/reviews.txt");
    string line;
    bool updated = false;

    vector<Movie> movies = FileManager::loadMovies();
    string movieName;

    for (const Movie& m : movies)
        if (m.getCode() == movieCode)
            movieName = m.getTitle();

    time_t now = time(nullptr);
    string dt = ctime(&now);
    dt.pop_back();

    while (getline(in, line)) {
        stringstream ss(line);
        string codeStr, mName, reviewer, text, oldDt;

        getline(ss, codeStr, '|');
        getline(ss, mName, '|');
        getline(ss, reviewer, '|');
        getline(ss, text, '|');
        getline(ss, oldDt);

        if (stoi(codeStr) == movieCode && reviewer == user.getFullName()) {
            line = codeStr + "|" + movieName + "|" +
                   user.getFullName() + "|" + review + "|" + dt;
            updated = true;
        }

        lines.push_back(line);
    }
    in.close();

    if (!updated) {
        lines.push_back(
            to_string(movieCode) + "|" +
            movieName + "|" +
            user.getFullName() + "|" +
            review + "|" + dt
        );
    }

    ofstream out("../data/reviews.txt");
    for (auto& l : lines)
        out << l << "\n";
    out.close();

    cout << GREEN << "✔ Review saved successfully.\n" << RESET;
}


/* ================= FILTER MOVIES ================= */

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

/* ================= SEARCH BY KEYWORD ================= */

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
