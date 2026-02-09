#ifndef CINEMASYSTEM_H
#define CINEMASYSTEM_H

#include "User.h"

#include <string>
using std::string;

class CinemaSystem {
public:

    void run();

private:
    
    void showWelcome();

    void showMainMenu();

    void login(User& user);

    void signup(User& user);

    void guestLogin(User& user);

    int getNextGuestNumber();

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