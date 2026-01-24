#ifndef SHOWTIME_H
#define SHOWTIME_H

#include <string>
using namespace std;

/*
 * Showtime
 * --------
 * Data model representing a single movie screening.
 * Connects a movie with a specific date, time, and hall.
 *
 * Timeline:
 * - Week 5: Showtime scheduling feature
 */
class Showtime {
private:
    /*
     * Week 5:
     * Title of the movie being screened.
     * Stored as string to directly link with Movie records.
     */
    string movieTitle;

    /*
     * Week 5:
     * Date of the show in YYYY-MM-DD format.
     * Stored as string for simple input and display.
     */
    string date;   // e.g. 2026-02-10

    /*
     * Week 5:
     * Time of the show in HH:MM format.
     * Used for schedule display and selection.
     */
    string time;   // e.g. 18:30

    /*
     * Week 5:
     * Hall number where the show is scheduled.
     * Differentiates multiple screenings of the same movie.
     */
    int hallNo;

public:
    /*
     * Week 5:
     * Default constructor.
     * Required for object creation during file loading.
     */
    Showtime();

    /*
     * Week 5:
     * Parameterized constructor.
     * Initializes all showtime attributes at once.
     */
    Showtime(string movieTitle, string date, string time, int hallNo);

    /*
     * Week 5:
     * Returns the movie title associated with this showtime.
     */
    string getMovieTitle() const;

    /*
     * Week 5:
     * Returns the show date.
     */
    string getDate() const;

    /*
     * Week 5:
     * Returns the show time.
     */
    string getTime() const;

    /*
     * Week 5:
     * Returns the hall number for this show.
     */
    int getHallNo() const;
};

#endif
