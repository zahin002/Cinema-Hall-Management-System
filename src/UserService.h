#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "User.h"

class UserService {
public:
    void userMenu(const User& user);

private:
    void viewMovies();
    void viewShowtimes();
    void bookSeat();
    void recommendSeat();
};

#endif
