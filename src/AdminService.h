#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include "User.h"
#include <string>

using namespace std;

class AdminService {
public:
    // Main admin menu
    void adminMenu(const User& admin);

    // Date validation for showtime (DD-MM-YYYY, today → next 30 days)
    bool isValidFutureDate(const string& dateStr);

private:
    // Movie management
    void addMovie();
    void viewMovies();
    void deleteMovie();

    // Showtime management
    void addShowtime();
    void viewShowtimes();

    // Seat map management
    void createSeatMap();
    void viewSeatMap();
};

#endif
