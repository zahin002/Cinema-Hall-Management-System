#ifndef CINEMASYSTEM_H
#define CINEMASYSTEM_H

#include "User.h"

/*
 * CinemaSystem
 * -------------
 * Main controller class of the CINE++ Cinema Hall Management System.
 * This class manages program flow, authentication, role-based access,
 * and coordinates all major system features.
 *
 * Timeline follows the original SPL-1 development order strictly.
 * Weeks are NOT merged or re-mapped.
 */
class CinemaSystem {
public:
    /*
     * Week 1:
     * Starts the system execution.
     * Controls welcome screen display, main menu navigation,
     * and routes users to login or signup workflows.
     */
    void run();

private:
    /*
     * Week 1:
     * Displays the initial welcome banner.
     * Purely responsible for user-facing introduction.
     */
    void showWelcome();

    /*
     * Week 1:
     * Displays the main menu before authentication.
     * Keeps entry actions simple and isolated.
     */
    void showMainMenu();

    /*
     * Week 1:
     * Handles user authentication.
     * Compares encrypted credentials and routes users
     * based on their role (admin or user).
     */
    void login();

    /*
     * Week 1:
     * Handles user registration.
     * Encrypts password before storing user data.
     */
    void signup();

    /*
     * Week 2:
     * Displays admin-only operations.
     * Provides access to system modification features.
     */
    void adminMenu(const User& admin);

    /*
     * Week 2:
     * Displays user-only operations.
     * Restricts access to browsing and booking features.
     */
    void userMenu(const User& user);

    /*
     * Week 3:
     * Allows administrators to add movies to the system.
     * Movie information becomes the foundation for scheduling.
     */
    void addMovie();

    /*
     * Week 3:
     * Displays all available movies.
     * Used by both admins and users for selection.
     */
    void viewMovies();

    /*
     * Week 5:
     * Adds showtimes for existing movies.
     * Introduces scheduling using date, time, and hall number.
     */
    void addShowtime();

    /*
     * Week 5:
     * Displays all stored showtimes.
     * Helps users and admins view scheduling details.
     */
    void viewShowtimes();

    /*
     * Week 6:
     * Creates a seat map for a specific show.
     * Initializes seating layout before booking begins.
     */
    void createSeatMap();

    /*
     * Week 6:
     * Displays the seat map of a selected show.
     * Visually distinguishes available and booked seats.
     */
    void viewSeatMap();

    /*
     * Week 7:
     * Handles seat booking for users.
     * Includes input normalization, duplicate prevention,
     * boundary checks, availability validation,
     * and atomic seat booking.
     */
    void bookSeat();
    
    /*
     * Week 8:
     * Recommends the best available seats automatically.
     * Uses custom seat selection logic for group booking.
     */
    void recommendSeat();

};

#endif
