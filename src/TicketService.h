#ifndef TICKET_SERVICE_H
#define TICKET_SERVICE_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class TicketService {
public:
    // ===== Ticket Creation =====
    static string generateTicketId();

    static void printTicket(
        const string& ticketId,
        const string& customerName,
        const string& movieTitle,
        const string& showDate,
        const string& showTime,
        int hallNo,
        const vector<pair<int,int>>& seats,
        double basePrice,
        double discountTk,
        const string& discountLabel,
        double finalPrice
    );

    static void saveTicket(
        const string& ticketId,
        const string& customerName,
        const string& movieTitle,
        const string& showDate,
        const string& showTime,
        int hallNo,
        const vector<pair<int,int>>& seats,
        double finalPrice
    );

    // ===== Cancellation & Refund =====
    static void cancelTicket(const string& ticketId);

private:
    // Refund calculation based on time before show
    static int calculateRefundPercent(
        const string& showDate,
        const string& showTime
    );

    // Cancelled ticket printout
    static void printCancelledTicket(
        const string& ticketId,
        const string& customerName,
        const string& movieTitle,
        const string& showDate,
        const string& showTime,
        int hallNo,
        const vector<pair<int,int>>& seats,
        double paidAmount,
        int refundPercent,
        double refundAmount
    );
};

#endif
