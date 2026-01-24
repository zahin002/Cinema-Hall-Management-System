#include "Movie.h"

/*
 * Movie
 * -----
 * Represents a single movie entity in the CINE++ system.
 * Stores basic metadata required for browsing and scheduling.
 *
 * Timeline:
 * - Week 3: Movie management and data modeling
 */

/*
 * Week 3:
 * Default constructor.
 * Allows creation of an empty Movie object when required
 * by containers or file-loading mechanisms.
 */
Movie::Movie() {}

/*
 * Week 3:
 * Parameterized constructor.
 * Initializes all core attributes of a movie at creation time.
 * This constructor is primarily used when adding movies
 * or reconstructing them from persistent storage.
 */
Movie::Movie(string title, string genre, int duration, string language) {
    this->title = title;
    this->genre = genre;
    this->duration = duration;
    this->language = language;
}

/*
 * Week 3:
 * Returns the title of the movie.
 * Used for display, selection, and showtime association.
 */
string Movie::getTitle() const {
    return title;
}

/*
 * Week 3:
 * Returns the genre of the movie.
 * Helps users identify movie category during browsing.
 */
string Movie::getGenre() const {
    return genre;
}

/*
 * Week 3:
 * Returns the duration of the movie in minutes.
 * Used for informational display and scheduling context.
 */
int Movie::getDuration() const {
    return duration;
}

/*
 * Week 3:
 * Returns the language of the movie.
 * Allows users to filter or choose movies based on language.
 */
string Movie::getLanguage() const {
    return language;
}
