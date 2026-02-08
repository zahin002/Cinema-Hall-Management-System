#ifndef TICKET_SERVICE_H
#define TICKET_SERVICE_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class TicketService {
public:

    static std::string generateTicketId();
    
    // Prints colored ticket to terminal
    
    static void printTicket(
        const string& ticketId,
        const string& customerName,
        const string& movieTitle,
        const string& showDate,
        const string& showTime,
        int hallNo,
        const vector<pair<int,int>>& selectedSeats,
        double basePrice,
        double discountTk,
        const string& discountLabel,
        double finalPrice
    );

    
    // Saves ticket to file
    static void saveTicket(
        const string& ticketId,
        const string& customerName,
        const string& movieTitle,
        const string& showDate,
        const string& showTime,
        int hallNo,
        const vector<pair<int,int>>& selectedSeats,
        double finalPrice
    );
};

#endif
