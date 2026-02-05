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
    void viewMovies();
    void deleteMovie();

    // Showtime management
    void addShowtime();
    void viewShowtimes();
    void deleteShowtime();   // only once
};

#endif
