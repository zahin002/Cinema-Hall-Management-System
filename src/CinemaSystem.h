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

    bool login(User& user);

    bool signup(User& user);

    bool guestLogin(User& user);

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