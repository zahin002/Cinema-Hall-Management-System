#include "FileManager.h"
#include "SeatMap.h"
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/*
 * FileManager
 * -----------
 * Responsible for file-based persistence in the CINE++ system.
 * All read/write operations for users, movies, showtimes,
 * and seat maps are centralized here.
 */

/* ================= USERS (WEEK 1) ================= */

/*
 * Week 1:
 * Saves a user's credentials and role into persistent storage.
 * Data is appended so that existing users are not overwritten.
 */
void FileManager::saveUser(const User& user) {
    ofstream file("../data/users.txt", ios::app);
    file << user.getEmail() << " "
         << user.getPassword() << " "
         << user.getRole() << endl;
    file.close();
}

/*
 * Week 1:
 * Loads all registered users from file.
 * Each line is parsed into email, encrypted password, and role.
 */
vector<User> FileManager::loadUsers() {
    vector<User> users;
    ifstream file("../data/users.txt");

    string email, password, role;
    while (file >> email >> password >> role) {
        users.push_back(User(email, password, role));
    }
    file.close();
    return users;
}

/* ================= MOVIES (WEEK 3) ================= */

/*
 * Week 3:
 * Saves movie information into persistent storage.
 * Uses '|' as a delimiter to support movie titles with spaces.
 */
void FileManager::saveMovie(const Movie& movie) {
    ofstream file("../data/movies.txt", ios::app);
    file << movie.getTitle() << "|"
         << movie.getGenre() << "|"
         << movie.getDuration() << "|"
         << movie.getLanguage() << endl;
    file.close();
}

/*
 * Week 3:
 * Loads all movies from file.
 * Parsing logic carefully mixes getline and formatted input
 * to correctly read both strings and integers.
 */
vector<Movie> FileManager::loadMovies() {
    vector<Movie> movies;
    ifstream file("../data/movies.txt");

    string title, genre, language;
    int duration;
    char sep;

    while (getline(file, title, '|')) {
        getline(file, genre, '|');
        file >> duration >> sep;   // sep consumes the '|' delimiter
        getline(file, language);
        movies.push_back(Movie(title, genre, duration, language));
    }
    file.close();
    return movies;
}

/* ================= SHOWTIMES (WEEK 5) ================= */

/*
 * Week 5:
 * Saves showtime information for a movie.
 * Each showtime links a movie with a date, time, and hall number.
 */
void FileManager::saveShowtime(const Showtime& s) {
    ofstream file("../data/showtimes.txt", ios::app);
    file << s.getMovieTitle() << "|"
         << s.getDate() << "|"
         << s.getTime() << "|"
         << s.getHallNo() << endl;
    file.close();
}

/*
 * Week 5:
 * Loads all stored showtimes.
 * Delimiter-based parsing allows flexible string values.
 */
vector<Showtime> FileManager::loadShowtimes() {
    vector<Showtime> shows;
    ifstream file("../data/showtimes.txt");

    string movieTitle, date, time;
    int hallNo;
    char sep;

    while (getline(file, movieTitle, '|')) {
        getline(file, date, '|');
        getline(file, time, '|');
        file >> hallNo;
        file.ignore();   // clears newline before next getline
        shows.push_back(Showtime(movieTitle, date, time, hallNo));
    }
    file.close();
    return shows;
}

/* ========== SEAT MAP (WEEK 6) ========== */

/*
 * Week 6:
 * Saves the seat map of a specific show.
 * Each show has a dedicated file identified by show ID.
 */
void FileManager::saveSeatMap(int showId, const SeatMap& map) {
    string filename = "../data/seats/show_" + to_string(showId) + ".txt";
    ofstream file(filename);

    // First line stores the seat grid dimensions
    file << map.getRows() << " " << map.getCols() << endl;

    // Remaining lines store seat availability row by row
    for (auto row : map.getSeats()) {
        for (char seat : row) {
            file << seat << " ";
        }
        file << endl;
    }
    file.close();
}

/*
 * Week 6:
 * Loads the seat map for a given show.
 * Reconstructs the seating grid exactly as it was saved.
 */
SeatMap FileManager::loadSeatMap(int showId) {
    string filename = "../data/seats/show_" + to_string(showId) + ".txt";
    ifstream file(filename);

    int rows, cols;
    file >> rows >> cols;

    SeatMap map(rows, cols);
    vector<vector<char>> seats(rows, vector<char>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            file >> seats[i][j];
        }
    }

    map.setSeats(seats);
    file.close();
    return map;
}
