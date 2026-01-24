#include "Showtime.h"

/*
 * Showtime
 * --------
 * Represents a scheduled screening of a movie.
 * Links a movie title with a specific date, time,
 * and hall number.
 *
 * Timeline:
 * - Week 5: Introduction of showtime scheduling
 */

/*
 * Week 5:
 * Default constructor.
 * Allows creation of empty Showtime objects when
 * loading data from storage or using containers.
 */
Showtime::Showtime() {}

/*
 * Week 5:
 * Parameterized constructor.
 * Initializes all attributes required to uniquely
 * identify a movie screening.
 */
Showtime::Showtime(string movieTitle, string date, string time, int hallNo) {
    this->movieTitle = movieTitle;
    this->date = date;
    this->time = time;
    this->hallNo = hallNo;
}

/*
 * Week 5:
 * Returns the title of the movie being shown.
 * Used to associate showtimes with movies.
 */
string Showtime::getMovieTitle() const {
    return movieTitle;
}

/*
 * Week 5:
 * Returns the date of the show.
 * Stored as a string for simple display and input.
 */
string Showtime::getDate() const {
    return date;
}

/*
 * Week 5:
 * Returns the time of the show.
 * Used for schedule display.
 */
string Showtime::getTime() const {
    return time;
}

/*
 * Week 5:
 * Returns the hall number where the show is scheduled.
 * Helps differentiate multiple screenings.
 */
int Showtime::getHallNo() const {
    return hallNo;
}
