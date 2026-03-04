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
    if (!file.is_open()) {
        count = 0;
        return 0.0;
    }

    string line;
    double total = 0.0;
    count = 0;

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
            int code = stoi(codeStr);
            int rating = stoi(ratingStr);

            if (code == movieCode) {
                total += rating;
                count++;
            }
        }
        catch (...) {
            // skip malformed line safely
            continue;
        }
    }

    if (count == 0)
        return 0.0;

    return total / count;
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
        stringstream ss(line);
        string codeStr, name, email, rateStr, dt;

        getline(ss, codeStr, '|');
        getline(ss, name, '|');
        getline(ss, email, '|');
        getline(ss, rateStr, '|');
        getline(ss, dt);

        if (stoi(codeStr) == movieCode && email == user.getEmail()) {
            time_t now = time(nullptr);
            string t = ctime(&now);
            t.pop_back();

            line = codeStr + "|" + name + "|" +
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

    cout << GREEN << "Rating saved.\n" << RESET;
}
