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
        string email;
        string text;
        string dt;
    };

    vector<Review> reviews;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string codeStr, movieName, email, text, dt;

        getline(ss, codeStr, '|');
        getline(ss, movieName, '|');
        getline(ss, email, '|');
        getline(ss, text, '|');
        getline(ss, dt);

        try {
            if (stoi(codeStr) == movieCode)
                reviews.push_back({ email, text, dt });
        } catch (...) {
            continue;
        }
    }

    if (reviews.empty()) {
        cout << YELLOW << "No reviews yet.\n" << RESET;
        return;
    }

    // Load users once
    vector<User> users = FileManager::loadUsers();

    sort(reviews.begin(), reviews.end(),
         [](const Review& a, const Review& b) {
             return a.dt > b.dt;
         });

    cout << "\n" << BOLD << CYAN << "REVIEWS\n" << RESET;

    for (const auto& r : reviews) {

        string displayName = r.email; // fallback

        for (const User& u : users) {
            if (u.getEmail() == r.email) {
                displayName = u.getFullName();
                break;
            }
        }

        cout << "[" << r.dt << "] "
             << BOLD << GREEN << displayName << RESET << "\n"
             << r.text << "\n\n";
    }
}

string ReviewService::getStarVisual(int rating) {

    string stars;

    for (int i = 1; i <= 5; i++) {
        if (i <= rating)
            stars += "★ ";
        else
            stars += "☆ ";
    }

    return stars;
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
        string codeStr, mName, email, text, oldDt;

        getline(ss, codeStr, '|');
        getline(ss, mName, '|');
        getline(ss, email, '|');
        getline(ss, text, '|');
        getline(ss, oldDt);

        try {
            if (stoi(codeStr) == movieCode && email == user.getEmail()) {
                line = codeStr + "|" + movieName + "|" +
                       user.getEmail() + "|" + review + "|" + dt;
                updated = true;
            }
        } catch (...) {}

        lines.push_back(line);
    }

    if (!updated) {
        lines.push_back(
            to_string(movieCode) + "|" +
            movieName + "|" +
            user.getEmail() + "|" +
            review + "|" + dt
        );
    }

    ofstream out("../data/reviews.txt");
    for (const auto& l : lines)
        out << l << "\n";

    cout << GREEN << "Review saved.\n" << RESET;
}

/* ================= DELETE OWN REVIEW ================= */

void ReviewService::deleteOwnReview(int movieCode, const User& user) {

    ifstream in("../data/reviews.txt");
    vector<string> lines;
    string line;
    bool deleted = false;

    while (getline(in, line)) {
        stringstream ss(line);
        string codeStr, name, email, text, dt;

        getline(ss, codeStr, '|');
        getline(ss, name, '|');
        getline(ss, email, '|');
        getline(ss, text, '|');
        getline(ss, dt);

        if (stoi(codeStr) == movieCode && email == user.getEmail()) {
            deleted = true;
            continue;
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

    cout << GREEN << "Review deleted.\n" << RESET;
}

/* ================= DELETE ANY REVIEW (ADMIN) ================= */

void ReviewService::deleteAnyReview(int movieCode) {

    ifstream in("../data/reviews.txt");
    vector<string> lines, candidates;
    string line;

    while (getline(in, line)) {
        stringstream ss(line);
        string codeStr, name, email, text, dt;

        getline(ss, codeStr, '|');
        getline(ss, name, '|');
        getline(ss, email, '|');
        getline(ss, text, '|');
        getline(ss, dt);

        if (stoi(codeStr) == movieCode)
            candidates.push_back(line);

        lines.push_back(line);
    }

    if (candidates.empty()) {
        cout << YELLOW << "No reviews to delete.\n" << RESET;
        return;
    }

    cout << RED << "Select review to delete:\n" << RESET;
    for (size_t i = 0; i < candidates.size(); i++)
        cout << i + 1 << ". " << candidates[i] << "\n";

    int choice;
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
    for (const auto& l : lines)
        if (l != target)
            out << l << "\n";

    cout << GREEN << "Review deleted.\n" << RESET;
}
