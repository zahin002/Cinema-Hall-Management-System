#include "FileManager.h"
#include "SeatMap.h"
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/* ================= USERS ================= */

/*
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

/* ================= MOVIES ================= */

/* Saves movie information into persistent storage. */

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

/* ================= SHOWTIMES ================= */

/*
 * Saves showtime information for a movie.
 * Each showtime links a movie with a date, time and hall number.
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


/* ========== SEAT MAP ========== */

/*
 * Saves the seat map of a specific show.
 * Each show has a dedicated file identified by show ID.
 */

string FileManager::getSeatMapFilename(int hallNo, const string& date, const string& time) {
    string t = time;
    for (char& c : t) {
        if (c == ' ' || c == ':' || c == '-') c = '_';
    }

    return "../data/seats/hall_" + to_string(hallNo) + "_" + date + "_" + t + ".txt";
}

    SeatMap FileManager::loadOrCreateSeatMap(int hallNo, const string& date, const string& time) {
        string filename = getSeatMapFilename(hallNo, date, time);

    ifstream file(filename);
    if (!file.is_open()) {
        // Seat map doesn't exist yet → create new from hall layout
        SeatMap map = SeatMap::createForHall(hallNo);
        saveSeatMap(filename, map);
        return map;
    }

    int rows, cols;
    file >> rows >> cols;

    SeatMap map(rows, cols);
    vector<vector<char>> seats(rows, vector<char>(cols));

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            file >> seats[i][j];

    map.setSeats(seats);
    return map;
}

//Old One//

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

//New One//

void FileManager::saveSeatMap(const string& filename, const SeatMap& map) {
    ofstream file(filename);

    file << map.getRows() << " " << map.getCols() << endl;

    for (auto& row : map.getSeats()) {
        for (char seat : row)
            file << seat << " ";
        file << endl;
    }
}


/*
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

void FileManager::saveTicket(
    const string& userKey,
    int showId,
    int seatCount,
    int totalPrice,
    const vector<pair<int,int>>& seats
) {
    ofstream file("../data/tickets.txt", ios::app);

    file << userKey << "|"
         << showId << "|"
         << seatCount << "|"
         << totalPrice << "|";

    for (size_t i = 0; i < seats.size(); i++) {
        char row = 'A' + seats[i].first;
        int col = seats[i].second + 1;
        file << row << col;
        if (i != seats.size() - 1) file << ",";
    }

    file << endl;
    file.close();
}


