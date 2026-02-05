#ifndef CINEMASYSTEM_H
#define CINEMASYSTEM_H

#include "User.h"

class CinemaSystem {
public:

    void run();

private:
    
    void showWelcome();

    void showMainMenu();

    void login();

    void signup();

    void guestLogin();

    bool isValidBangladeshPhone(const string& phone);

    void addMovie();

    void viewMovies();

    void addShowtime();

    void viewShowtimes();

    void createSeatMap();

    void viewSeatMap();

    void bookSeat();
    
    void recommendSeat();

};

#endif