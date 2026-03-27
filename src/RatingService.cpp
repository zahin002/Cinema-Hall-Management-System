#include "RatingService.h"
#include "FileManager.h"
#include "Movie.h"
#include "TerminalColors.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <ctime>
#include <limits>

using namespace std;

/* ================= AVERAGE RATING ================= */

double RatingService::getAverageRating(int movieCode, int& count) {

    ifstream file("../data/ratings.txt");
    count = 0;
    double total = 0;

    if (!file.is_open())
        return 0;

    string line;

    while (getline(file, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);
        string codeStr, movieName, email, ratingStr, dt;

        getline(ss, codeStr, '|');
        getline(ss, movieName, '|');
        getline(ss, email, '|');
        getline(ss, ratingStr, '|');
        getline(ss, dt);

        try {
            // validate before converting
            if (codeStr.empty() || ratingStr.empty())
                continue;

            int code = stoi(codeStr);
            int rating = stoi(ratingStr);

            if (code == movieCode) {
                total += rating;
                count++;
            }
        }
        catch (...) {
            // skip invalid/corrupted lines safely
            continue;
        }
    }

    return (count == 0) ? 0 : total / count;
}

/* ================= GIVE / UPDATE RATING ================= */

void RatingService::giveOrUpdateRating(int movieCode, const User& user) {

    int rating;
    cout << "Enter rating (1-5): ";

    if (!(cin >> rating) || rating < 1 || rating > 5) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid rating.\n" << RESET;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<string> lines;
    ifstream in("../data/ratings.txt");
    string line;
    bool updated = false;

    while (getline(in, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);
        string codeStr, name, email, rateStr, dt;

        getline(ss, codeStr, '|');
        getline(ss, name, '|');
        getline(ss, email, '|');
        getline(ss, rateStr, '|');
        getline(ss, dt);

        try {
            // validate before stoi
            if (codeStr.empty() || rateStr.empty())
                throw invalid_argument("empty");

            int code = stoi(codeStr);

            if (code == movieCode && email == user.getEmail()) {

                time_t now = time(nullptr);
                string t = ctime(&now);
                t.pop_back(); // remove newline

                line = to_string(movieCode) + "|" + name + "|" +
                       email + "|" + to_string(rating) + "|" + t;

                updated = true;
            }
        }
        catch (...) {
            // skip corrupted lines safely
            continue;
        }

        lines.push_back(line);
    }

    in.close();

    // ===== ADD NEW RATING IF NOT UPDATED =====
    if (!updated) {

        vector<Movie> movies = FileManager::loadMovies();
        string movieName = "Unknown";

        for (const Movie& m : movies) {
            if (m.getCode() == movieCode) {
                movieName = m.getTitle();
                break;
            }
        }

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

    cout << GREEN << "Rating saved successfully.\n" << RESET;
}
