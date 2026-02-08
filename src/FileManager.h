
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include "User.h"
#include "Movie.h"
#include "Showtime.h"
#include "SeatMap.h"

/*
 * FileManager
 * -----------
 * Utility class responsible for persistent storage handling.
 * Provides static methods to save and load system data
 * using text-based file storage.
 */
class FileManager {
public:
    /*
     * Saves a user's credentials and role to storage.
     * Data is appended to avoid overwriting existing users.
     */

    static void saveUser(const User& user);

    /*
     * Loads all registered users from storage.
     * Each entry is reconstructed into a User object.
     */

    static vector<User> loadUsers();

    /*
     * Saves movie details such as title, genre, duration,
     * and language into persistent storage.
     */

    static int getNextMovieCode();
    static bool movieCodeExists(int code);

    static void saveMovie(const Movie& movie);

    /*
     * Loads all movies from storage.
     * Used for browsing and showtime creation.
     */
    
    static vector<Movie> loadMovies();

    /*
     * Saves showtime information linked to a movie.
     * Each showtime stores date, time, and hall number.
     */

    static void saveShowtime(const Showtime& showtime);

    /*
     * Loads all showtimes from storage.
     * Enables users and admins to view schedules.
     */

    static vector<Showtime> loadShowtimes();

   
    /*
     * Saves the seat map for a specific show.
     * Seat layout is persisted separately per show ID.
     */

    static string getSeatMapFilename(int hallNo, const string& date, const string& time);

    static SeatMap loadOrCreateSeatMap(int hallNo, const string& date, const string& time);

    static void saveSeatMap(const string& filename, const SeatMap& map);  

    static void deleteSeatMapFile(int hallNo, const string& date, const string& time);

    /*Global Discount*/

    static void saveGlobalDiscount(int percent, const string& message);

    static bool loadGlobalDiscount(int& percent, string& message);

    /* Save Ticket */
    
    static void saveTicketToFile(const string& record);
   
};

#endif
