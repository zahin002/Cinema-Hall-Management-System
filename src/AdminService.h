#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include "User.h"
#include <string>

class AdminService {
public:
    void adminMenu(const User& admin);

private:
    // Movie management
    void addMovie();
    void deleteMovie();

    // Showtime management
    void addShowtime();
    void viewShowtimes();
    void deleteShowtime();

    // Seat map viewing 
    void viewSeatMap();

    // Date validation helper
    bool isValidFutureDate(const std::string& dateStr);
};

#endif
