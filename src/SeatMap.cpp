#include "SeatMap.h"
#include <iostream>

/*
 * SeatMap
 * -------
 * Manages seat layout, booking state, and recommendation logic
 * for a specific show.
 */

/*
 * Constructs a seat map with given dimensions.
 * Immediately initializes all seats as available.
 */

SeatMap::SeatMap(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    initialize();
}

/*
 * Creates a predefined seat layout based on hall number.
 * Hall structures are fixed and reused for all showtimes.
 */
SeatMap SeatMap::createForHall(int hallNo) {
    if (hallNo == 1) {
        // Largest hall
        return SeatMap(10, 12);
    }
    else if (hallNo == 2) {
        // Medium hall
        return SeatMap(8, 10);
    }
    else if (hallNo == 3) {
        // Smallest hall
        return SeatMap(6, 8);
    }
    else {
        // Safety fallback
        return SeatMap(0, 0);
    }
}


/*
 * Initializes the seat grid.
 * All seats are marked 'O' to indicate availability.
 */

void SeatMap::initialize() {
    seats.assign(rows, vector<char>(cols, 'O'));
}

/*
 * Displays the seat layout in a user-friendly format.
 * Rows are labeled alphabetically (A, B, C, ...)
 * Columns are numbered starting from 1.
 */

void SeatMap::display() const {
    cout << "\nSeat Layout (O = Available, X = Booked)\n\n   ";

    // Print column numbers with spacing alignment
    for (int c = 1; c <= cols; c++) {
        if (c < 10) cout << c << "  ";
        else cout << c << " ";
    }
    cout << endl;

    for (int i = 0; i < rows; i++) {
        char rowChar = 'A' + i;
        cout << rowChar << "  ";

        for (int j = 0; j < cols; j++) {
            cout << seats[i][j] << "  ";
        }
        cout << endl;
    }
}

/*
 * Attempts to book a seat at the given row and column.
 * Performs bounds checking and prevents double booking.
 */

bool SeatMap::bookSeat(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return false;

    if (seats[row][col] == 'X')
        return false;

    seats[row][col] = 'X';
    return true;
}

/* Getter for total number of rows. */

int SeatMap::getRows() const { return rows; }

/* Getter for total number of columns. */

int SeatMap::getCols() const { return cols; }

/*
 * Returns the entire seat grid.
 * Used during file persistence.
 */

vector<vector<char>> SeatMap::getSeats() const { return seats; }

/*
 * Replaces the current seat grid.
 * Used when loading seat data from storage.
 */

void SeatMap::setSeats(const vector<vector<char>>& s) {
    seats = s;
}


/*
 * Checks whether a specific seat is available.
 * Includes bounds checking to prevent invalid access.
 */
bool SeatMap::isSeatAvailable(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return false;

    return seats[row][col] == 'O';
}

/*
 * Recommends a single best available seat.
 * Preference is given to seats closest to the center of the seating layout.
 */

pair<int,int> SeatMap::recommendBestSeat() const {
    int centerRow = rows / 2;
    int centerCol = cols / 2;

    /*
     * The search expands outward from the center,
     * alternating above and below the center row,
     * and left and right of the center column.
     */

    for (int offset = 0; offset < rows; offset++) {
        int r1 = centerRow - offset;
        int r2 = centerRow + offset;

        for (int r : {r1, r2}) {
            if (r < 0 || r >= rows) continue;

            for (int cOffset = 0; cOffset < cols; cOffset++) {
                int c1 = centerCol - cOffset;
                int c2 = centerCol + cOffset;

                for (int c : {c1, c2}) {
                    if (c < 0 || c >= cols) continue;
                    if (seats[r][c] == 'O')
                        return {r, c};
                }
            }
        }
    }

    // No available seat found
    return {-1, -1};
}

/*
 * Recommends a contiguous block of seats for group booking.
 * Attempts to find seats closest to the center first.
 */

vector<pair<int,int>> SeatMap::recommendBestSeats_Custom(int count) const {
    vector<pair<int,int>> result;

    int midRow = rows / 2;
    int midCol = cols / 2;

    /*
     * Row traversal strategy:
     * Starts from the middle row and expands outward,
     * alternating between rows behind and in front.
     */

    for (int offset = 0; offset < rows; offset++) {
        int rowCandidates[2] = {
            midRow + offset,
            midRow - offset
        };

        for (int r : rowCandidates) {
            if (r < 0 || r >= rows) continue;

            /*
             * CASE 1:
             * Search for consecutive seats to the right of center.
             */

            for (int start = midCol; start <= cols - count; start++) {
                bool ok = true;
                for (int k = 0; k < count; k++) {
                    if (seats[r][start + k] != 'O') {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    for (int k = 0; k < count; k++)
                        result.push_back({r, start + k});
                    return result;
                }
            }

            /*
             * CASE 2:
             * Search for consecutive seats to the left of center.
             */
            
            for (int start = midCol - count; start >= 0; start--) {
                bool ok = true;
                for (int k = 0; k < count; k++) {
                    if (seats[r][start + k] != 'O') {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    for (int k = 0; k < count; k++)
                        result.push_back({r, start + k});
                    return result;
                }
            }
        }
    }

    // No suitable group seating found
    return {};
}