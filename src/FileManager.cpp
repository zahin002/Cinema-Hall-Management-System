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
 */

void FileManager::saveMovie(const Movie& movie) {
    ofstream file("../data/movies.txt", ios::app);
    file << movie.getCode() << "|"
         << movie.getTitle() << "|"
         << movie.getGenre() << "|"
         << movie.getDuration() << "|"
         << movie.getLanguage() << endl;
    file.close();
}

//Auto-generated movie code//

int FileManager::getNextMovieCode() {
    vector<Movie> movies = loadMovies();

    int maxCode = 100;
    for (const Movie& m : movies) {
        if (m.getCode() > maxCode)
            maxCode = m.getCode();
    }
    return maxCode + 1;
}

/*
 * Week 3:
 * Loads all movies from file.
 * Parsing logic carefully mixes getline and formatted input to correctly read both strings and integers.
 */

vector<Movie> FileManager::loadMovies() {
    vector<Movie> movies;
    ifstream file("../data/movies.txt");

    int code, duration;
    string title, genre, language;
    char sep;

    while (file >> code >> sep) {
        getline(file, title, '|');
        getline(file, genre, '|');
        file >> duration >> sep;
        getline(file, language);

        movies.push_back(Movie(code, title, genre, duration, language));
    }
    file.close();
    return movies;
}

//Prevents duplicates during manual code//

bool FileManager::movieCodeExists(int code) {
    vector<Movie> movies = loadMovies();
    for (const Movie& m : movies) {
        if (m.getCode() == code)
            return true;
    }
    return false;
}

/* ================= SHOWTIMES (WEEK 5) ================= */

/*
 * Week 5:
 * Saves showtime information for a movie.
 * Each showtime links a movie with a date, time, and hall number.
 */
void FileManager::saveShowtime(const Showtime& s) {
    ofstream file("../data/showtimes.txt", ios::app);
    file << s.getMovieCode() << "|"
         << s.getDate() << "|"
         << s.getTime() << "|"
         << s.getHallNo() << endl;
    file.close();
}

vector<Showtime> FileManager::loadShowtimes() {
    vector<Showtime> shows;
    ifstream file("../data/showtimes.txt");

    int movieCode, hallNo;
    string date, time;
    char sep;

    while (file >> movieCode >> sep) {
        getline(file, date, '|');
        getline(file, time, '|');
        file >> hallNo;
        file.ignore();

        shows.push_back(
            Showtime(movieCode, date, time, hallNo)
        );
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