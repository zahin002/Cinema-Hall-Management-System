#ifndef SEATMAP_H
#define SEATMAP_H

#include <vector>
using namespace std;

/*
 * SeatMap
 * -------
 * Represents the seating layout for a specific show.
 * Manages seat availability, booking operations,
 * and intelligent seat recommendation logic. */
class SeatMap {
private:
 
    int rows;
    int cols;

    /*
     * 2D grid representing seat status.
     * 'O' = available, 'X' = booked
     */

    vector<vector<char>> seats;

public:
    /*
     * Constructor to create a seat map with given dimensions.
     * Default values allow object creation before initialization.
     */

    SeatMap(int rows = 0, int cols = 0);

    /*
     * Initializes all seats as available.
     * Called during construction and seat map creation.
     */

    void initialize();

    /*
     * Displays the seat layout with row and column labels.*/

    void display() const;

    /*
     * Attempts to book a seat at the specified position.
     * Returns false if the seat is invalid or already booked.
     */

    bool bookSeat(int row, int col);

    /*  Returns the total number of rows. */

    int getRows() const;

    /* Returns the total number of columns. */

    int getCols() const;

    /*
     * Returns the complete seat grid.
     * Used for saving seat map data to storage.
     */

    vector<vector<char>> getSeats() const;

    /*
     * Replaces the current seat grid.
     * Used when loading seat map data from storage.
     */

    void setSeats(const vector<vector<char>>& seats);

    /*
     * Checks whether a specific seat is available.
     * Includes bounds validation.
     */

    bool isSeatAvailable(int row, int col) const;

    // Best Seat Recommendation

    /*
     * Recommends the best single available seat.
     * Preference is given to seats closest to the center.
     */

    pair<int,int> recommendBestSeat() const;

    /*
     * Recommends a contiguous block of seats for group booking.
     * Attempts to place the group near the center if possible.
     */
    
    vector<pair<int,int>> recommendBestSeats_Custom(int count) const;

};

#endif
