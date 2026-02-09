#ifndef MENU_SERVICE_H
#define MENU_SERVICE_H

#include "User.h"

class MenuService {
public:
    // Main menus
    void showUserMenu(const User& user);

private:
    // Sub-menus
    void browseMoviesMenu(const User& user);
    void selectMovieForDetails(const User& user);
};

#endif
