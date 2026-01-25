#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include "User.h"

class AdminService {
public:
    void adminMenu(const User& admin);

private:
    void addMovie();
    void viewMovies();
    void addShowtime();
    void viewShowtimes();
    void createSeatMap();
    void viewSeatMap();
};

#endif