#include "FileManager.h"
#include "SeatMap.h"
#include <fstream>
#include<iostream>
#include <vector>
#include <string>

using namespace std;

/* ================= USERS ================= */

void FileManager::saveUser(const User& user) {
    ofstream file("../data/users.txt", ios::app);
    file << user.getEmail() << " "
         << user.getPassword() << " "
         << user.getRole() << endl;
}

vector<User> FileManager::loadUsers() {
    vector<User> users;
    ifstream file("../data/users.txt");

    string email, password, role;
    while (file >> email >> password >> role) {
        users.push_back(User(email, password, role));
    }
    return users;
}

/* ================= MOVIES ================= */

void FileManager::saveMovie(const Movie& movie) {
    ofstream file("../data/movies.txt", ios::app);
    file << movie.getCode() << "|"
         << movie.getTitle() << "|"
         << movie.getGenre() << "|"
         << movie.getDuration() << "|"
         << movie.getLanguage() << endl;
}

int FileManager::getNextMovieCode() {
    vector<Movie> movies = loadMovies();
    int maxCode = 100;

    for (const Movie& m : movies)
        maxCode = max(maxCode, m.getCode());

    return maxCode + 1;
}

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
    return movies;
}

bool FileManager::movieCodeExists(int code) {
    for (const Movie& m : loadMovies())
        if (m.getCode() == code)
            return true;
    return false;
}

/* ================= SHOWTIMES ================= */

void FileManager::saveShowtime(const Showtime& s) {
    ofstream file("../data/showtimes.txt", ios::app);
    file << s.getMovieCode() << "|"
         << s.getDate() << "|"
         << s.getTime() << "|"
         << s.getHallNo() << endl;
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

        shows.emplace_back(movieCode, date, time, hallNo);
    }
    return shows;
}

/* ================= SEAT MAP ================= */

string FileManager::getSeatMapFilename(
        int hallNo,
        const string& date,
        const string& time) {

    string t = time;
    for (char& c : t)
        if (c == ' ' || c == ':' || c == '-')
            c = '_';

    return "../data/seats/hall_" +
           to_string(hallNo) + "_" +
           date + "_" + t + ".txt";
}

SeatMap FileManager::loadOrCreateSeatMap(
        int hallNo,
        const string& date,
        const string& time) {

    string filename = getSeatMapFilename(hallNo, date, time);
    ifstream file(filename);

    // Create new map if not exists
    if (!file.is_open()) {
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

void FileManager::saveSeatMap(
        const string& filename,
        const SeatMap& map) {

    ofstream file(filename);
    file << map.getRows() << " " << map.getCols() << endl;

    for (auto& row : map.getSeats()) {
        for (char seat : row)
            file << seat << " ";
        file << endl;
    }
}

void FileManager::deleteSeatMapFile(
    int hallNo,
    const string& date,
    const string& time
) {
    string filename = getSeatMapFilename(hallNo, date, time);
    remove(filename.c_str());
}

void FileManager::saveGlobalDiscount(int percent, const string& message) {
    ofstream file("../data/global_discount.txt");
    file << percent << endl;
    file << message << endl;
}

bool FileManager::loadGlobalDiscount(int& percent, string& message) {
    ifstream file("../data/global_discount.txt");
    if (!file.is_open()) return false;

    file >> percent;
    file.ignore();
    getline(file, message);
    return true;
}

/* Save Ticket */

void FileManager::saveTicketToFile(const string& record) {
    ofstream out("tickets.txt", ios::app);
    if (!out) {
        cout << "Error saving ticket.\n";
        return;
    }
    out << record << endl;
    out.close();
}

vector<string> FileManager::loadAllTickets() {
    vector<string> records;
    ifstream in("../data/tickets.txt");
    string line;

    while (getline(in, line)) {
        records.push_back(line);
    }

    in.close();
    return records;
}

void FileManager::overwriteAllTickets(const vector<string>& records) {
    // IMPORTANT: no ios::app here
    ofstream out("../data/tickets.txt");

    for (const string& r : records) {
        out << r << endl;
    }

    out.close();
}

void FileManager::logRefund(const string& record) {
    ofstream out("../data/refunds.txt", ios::app);
    out << record << endl;
    out.close();
}






