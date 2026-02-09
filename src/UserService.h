#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "User.h"

class UserService {
public:
    void userMenu(const User& user);
    void viewMovies();
    void filterMovies();
    void searchMovieByName();
    void viewShowtimes();
    void bookSeat(const User& user);
    void recommendSeat();
    static void cancelTicket();
private:
    
};

#endif
