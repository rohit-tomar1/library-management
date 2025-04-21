#pragma once
#include <string>
#include <ctime>

class Transaction
{
private:
    int transactionId;
    int userId;
    int bookId;
    std::string type;
    std::string timestamp;

public:
    Transaction(int transactionId, int userId, int bookId, const std::string &type);
    Transaction(int transactionId, int userId, int bookId, const std::string &type, const std::string &timestamp);
    int getTransactionId() const;
    int getUserId() const;
    int getBookId() const;
    std::string getType() const;
    std::string getTimestamp() const;
    void displayDetails() const;
};