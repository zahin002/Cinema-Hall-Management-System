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
     * Week 1:
     * Saves a user's credentials and role to storage.
     * Data is appended to avoid overwriting existing users.
     */
    static void saveUser(const User& user);

    /*
     * Week 1:
     * Loads all registered users from storage.
     * Each entry is reconstructed into a User object.
     */
    static vector<User> loadUsers();

    /*
     * Week 3:
     * Saves movie details such as title, genre, duration,
     * and language into persistent storage.
     */
    static void saveMovie(const Movie& movie);

    /*
     * Week 3:
     * Loads all movies from storage.
     * Used for browsing and showtime creation.
     */
    static vector<Movie> loadMovies();

    // Week 5
    /*
     * Week 5:
     * Saves showtime information linked to a movie.
     * Each showtime stores date, time, and hall number.
     */
    static void saveShowtime(const Showtime& showtime);

    /*
     * Week 5:
     * Loads all showtimes from storage.
     * Enables users and admins to view schedules.
     */
    static vector<Showtime> loadShowtimes();

    // Week 6
    /*
     * Week 6:
     * Saves the seat map for a specific show.
     * Seat layout is persisted separately per show ID.
     */
    static void saveSeatMap(int showId, const SeatMap& map);

    /*
     * Week 6:
     * Loads the seat map for a given show.
     * Reconstructs seat availability for booking operations.
     */
    static SeatMap loadSeatMap(int showId);
};

#endif
