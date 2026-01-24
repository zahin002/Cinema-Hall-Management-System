#ifndef SEATMAP_H
#define SEATMAP_H

#include <vector>
using namespace std;

/*
 * SeatMap
 * -------
 * Represents the seating layout for a specific show.
 * Manages seat availability, booking operations,
 * and intelligent seat recommendation logic.
 *
 * Timeline:
 * - Week 6: Seat map structure, display, and booking
 * - Week 8: Seat availability and recommendation logic
 */
class SeatMap {
private:
    /*
     * Week 6:
     * Total number of seat rows in the hall.
     */
    int rows;

    /*
     * Week 6:
     * Total number of seat columns in the hall.
     */
    int cols;

    /*
     * Week 6:
     * 2D grid representing seat status.
     * 'O' = available, 'X' = booked
     */
    vector<vector<char>> seats;

public:
    /*
     * Week 6:
     * Constructor to create a seat map with given dimensions.
     * Default values allow object creation before initialization.
     */
    SeatMap(int rows = 0, int cols = 0);

    /*
     * Week 6:
     * Initializes all seats as available.
     * Called during construction and seat map creation.
     */
    void initialize();

    /*
     * Week 6:
     * Displays the seat layout with row and column labels.
     */
    void display() const;

    /*
     * Week 6:
     * Attempts to book a seat at the specified position.
     * Returns false if the seat is invalid or already booked.
     */
    bool bookSeat(int row, int col);

    /*
     * Week 6:
     * Returns the total number of rows.
     */
    int getRows() const;

    /*
     * Week 6:
     * Returns the total number of columns.
     */
    int getCols() const;

    /*
     * Week 6:
     * Returns the complete seat grid.
     * Used for saving seat map data to storage.
     */
    vector<vector<char>> getSeats() const;

    /*
     * Week 6:
     * Replaces the current seat grid.
     * Used when loading seat map data from storage.
     */
    void setSeats(const vector<vector<char>>& seats);

    /*
     * Week 8:
     * Checks whether a specific seat is available.
     * Includes bounds validation.
     */
    bool isSeatAvailable(int row, int col) const;

    // Best Seat Recommendation

    /*
     * Week 8:
     * Recommends the best single available seat.
     * Preference is given to seats closest to the center.
     */
    pair<int,int> recommendBestSeat() const;

    /*
     * Week 8:
     * Recommends a contiguous block of seats for group booking.
     * Attempts to place the group near the center if possible.
     */
    vector<pair<int,int>> recommendBestSeats_Custom(int count) const;

};

#endif
