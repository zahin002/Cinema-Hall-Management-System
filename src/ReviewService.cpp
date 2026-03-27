#include "ReviewService.h"
#include "FileManager.h"
#include "TerminalColors.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <limits>
#include <iomanip>

using namespace std;

/* ================= DISPLAY REVIEWS ================= */

void ReviewService::showReviews(int movieCode) {

    ifstream file("../data/reviews.txt");
    if (!file.is_open()) {
        cout << YELLOW << "No reviews yet.\n" << RESET;
        return;
    }

    struct Review {
        string fullName;
        string text;
        string dt;
    };

    vector<Review> reviews;
    string line;

    while (getline(file, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);
        string codeStr, movieName, fullName, text, dt;

        getline(ss, codeStr, '|');
        getline(ss, movieName, '|');
        getline(ss, fullName, '|');
        getline(ss, text, '|');
        getline(ss, dt);   // last field has no delimiter

        try {
            if (stoi(codeStr) == movieCode)
                reviews.push_back({ fullName, text, dt });
        }
        catch (...) {}
    }

    if (reviews.empty()) {
        cout << YELLOW << "No reviews yet.\n" << RESET;
        return;
    }

    cout << "\n" << BOLD << CYAN << "REVIEWS\n" << RESET;

    for (const auto& r : reviews) {

        cout << BOLD << GREEN
             << "==================================================\n"
             << RESET;

        cout << BOLD << BLUE
             << "User   : " << r.fullName
             << RESET << "\n";

        cout << "Date   : " << r.dt << "\n\n";

        cout << BOLD << YELLOW << "\"" << r.text << "\"" << RESET << "\n";

        cout << BOLD << GREEN
             << "==================================================\n"
             << RESET << "\n";
    }
}

/* ================= ADD / UPDATE REVIEW ================= */

void ReviewService::addOrUpdateReview(int movieCode, const User& user) {

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
    tm* local = localtime(&now);

    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%b %d %Y", local);
    string dt = dateStr;

    while (getline(in, line)) {

        stringstream ss(line);
        string codeStr, mName, fullName, text, oldDt;

        getline(ss, codeStr, '|');
        getline(ss, mName, '|');
        getline(ss, fullName, '|');
        getline(ss, text, '|');
        getline(ss, oldDt);

        try {
            if (stoi(codeStr) == movieCode &&
                fullName == user.getFullName()) {

                // Update existing review
                line = codeStr + "|" +
                       movieName + "|" +
                       user.getFullName() + "|" +
                       review + "|" +
                       dt;

                updated = true;
            }
        } catch (...) {}

        lines.push_back(line);
    }

    // If user has not reviewed before
    if (!updated) {

        lines.push_back(
            to_string(movieCode) + "|" +
            movieName + "|" +
            user.getFullName() + "|" +
            review + "|" +
            dt
        );
    }

    ofstream out("../data/reviews.txt");
    for (const auto& l : lines)
        out << l << "\n";

    cout << GREEN << "Review saved successfully.\n" << RESET;
}

/* ================= DELETE OWN REVIEW ================= */

void ReviewService::deleteOwnReview(int movieCode, const User& user) {

    ifstream in("../data/reviews.txt");
    if (!in.is_open()) {
        cout << YELLOW << "No reviews file found.\n" << RESET;
        return;
    }

    vector<string> lines;
    string line;
    bool deleted = false;

    while (getline(in, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);
        string codeStr, mName, fullName, text, dt;

        getline(ss, codeStr, '|');
        getline(ss, mName, '|');
        getline(ss, fullName, '|');
        getline(ss, text, '|');
        getline(ss, dt);

        try {
            if (stoi(codeStr) == movieCode &&
                fullName == user.getFullName()) {

                deleted = true;
                continue; // skip this line (delete)
            }
        }
        catch (...) {
            // skip corrupted lines safely
        }

        lines.push_back(line);
    }

    if (!deleted) {
        cout << YELLOW << "No review found to delete.\n" << RESET;
        return;
    }

    ofstream out("../data/reviews.txt");
    for (const auto& l : lines)
        out << l << "\n";

    cout << GREEN << "Review deleted successfully.\n" << RESET;
}

/* ================= DELETE ANY REVIEW (ADMIN) ================= */

void ReviewService::deleteAnyReview(int movieCode) {

    ifstream in("../data/reviews.txt");
    if (!in.is_open()) {
        cout << YELLOW << "No reviews file found.\n" << RESET;
        return;
    }

    vector<string> lines;
    vector<string> candidates;
    string line;

    while (getline(in, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);
        string codeStr, movieName, fullName, text, dt;

        getline(ss, codeStr, '|');
        getline(ss, movieName, '|');
        getline(ss, fullName, '|');
        getline(ss, text, '|');
        getline(ss, dt);

        try {
            if (stoi(codeStr) == movieCode)
                candidates.push_back(line);
        }
        catch (...) {
            continue; // skip corrupted line safely
        }

        lines.push_back(line);
    }

    if (candidates.empty()) {
        cout << YELLOW << "No reviews to delete.\n" << RESET;
        return;
    }

    cout << RED << "Select review to delete:\n" << RESET;

    for (size_t i = 0; i < candidates.size(); i++) {
        cout << i + 1 << ". " << candidates[i] << "\n";
    }

    int choice;
    cout << YELLOW << "Enter choice: " << RESET;
    cin >> choice;

    if (choice < 1 || choice > (int)candidates.size()) {
        cout << RED << "Invalid selection.\n" << RESET;
        return;
    }

    cout << YELLOW << "Confirm delete? (1=Yes, 2=No): " << RESET;
    int confirm;
    cin >> confirm;

    if (confirm != 1) {
        cout << YELLOW << "Deletion cancelled.\n" << RESET;
        return;
    }

    string target = candidates[choice - 1];

    ofstream out("../data/reviews.txt");

    for (const auto& l : lines) {
        if (l != target)
            out << l << "\n";
    }

    cout << GREEN << "Review deleted successfully.\n" << RESET;
}
