
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include "User.h"
#include "Movie.h"
#include "Showtime.h"
#include "SeatMap.h"

class FileManager {
public:

    static void saveUser(const User& user);

    static vector<User> loadUsers();

    static int getNextMovieCode();

    static bool movieCodeExists(int code);

    static void saveMovie(const Movie& movie);

    static vector<Movie> loadMovies();

    static void saveShowtime(const Showtime& showtime);

    static vector<Showtime> loadShowtimes();

    static string getSeatMapFilename(int hallNo, const string& date, const string& time);

    static SeatMap loadOrCreateSeatMap(int hallNo, const string& date, const string& time);

    static void saveSeatMap(const string& filename, const SeatMap& map);  

    static void deleteSeatMapFile(int hallNo, const string& date, const string& time);

    /*Global Discount*/

    static void saveGlobalDiscount(int percent, const string& message);

    static bool loadGlobalDiscount(int& percent, string& message);

    /* Save Ticket */
    
    static void saveTicketToFile(const string& record);

    // Ticket cancellation support
    static vector<string> loadAllTickets();
    static void overwriteAllTickets(const vector<string>& records);

   // Refund logging
   static void logRefund(const string& record);

   
};

#endif
