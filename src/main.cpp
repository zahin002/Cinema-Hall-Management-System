#include "CinemaSystem.h"

/*
 * Week 1:
 * Program entry point.
 * Creates an instance of CinemaSystem and
 * transfers control to the main application flow.
 */
int main() {
    /*
     * Week 1:
     * CinemaSystem object acts as the central controller
     * for the entire application.
     */
    CinemaSystem system;

    /*
     * Week 1:
     * Starts the execution of the system.
     * Handles welcome screen, menus, and user interaction.
     */
    system.run();

    /*
     * Week 1:
     * Returns 0 to indicate successful program termination.
     */
    return 0;
}


/* Compile:  
             g++ main.cpp CinemaSystem.cpp AdminService.cpp UserService.cpp User.cpp FileManager.cpp Movie.cpp Showtime.cpp SeatMap.cpp -o cine

            ./cine
*/
