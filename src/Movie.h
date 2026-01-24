#ifndef MOVIE_H
#define MOVIE_H

#include <string>
using namespace std;

/*
 * Movie
 * -----
 * Data model representing a movie in the CINE++ system.
 * Stores essential movie information used for browsing,
 * scheduling showtimes, and display.
 *
 * Timeline:
 * - Week 3: Introduction of movie management and data modeling
 */
class Movie {
private:
    /*
     * Week 3:
     * Title of the movie.
     * Acts as the primary identifier when associating
     * movies with showtimes.
     */
    string title;

    /*
     * Week 3:
     * Genre/category of the movie.
     * Used for descriptive and browsing purposes.
     */
    string genre;

    /*
     * Week 3:
     * Duration of the movie in minutes.
     * Provides runtime information for users.
     */
    int duration; // minutes

    /*
     * Week 3:
     * Language in which the movie is available.
     * Helps users choose movies based on preference.
     */
    string language;

public:
    /*
     * Week 3:
     * Default constructor.
     * Required for object creation during file loading
     * or container initialization.
     */
    Movie();

    /*
     * Week 3:
     * Parameterized constructor.
     * Initializes all movie attributes at once.
     */
    Movie(string title, string genre, int duration, string language);

    /*
     * Week 3:
     * Returns the movie title.
     */
    string getTitle() const;

    /*
     * Week 3:
     * Returns the movie genre.
     */
    string getGenre() const;

    /*
     * Week 3:
     * Returns the movie duration in minutes.
     */
    int getDuration() const;

    /*
     * Week 3:
     * Returns the movie language.
     */
    string getLanguage() const;
};

#endif
