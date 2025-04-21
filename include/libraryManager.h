
#pragma once

#include "library.h"
#include "transaction.h"
#include "user.h"
#include <fstream>
#include <vector>

class LibraryManager
{
private:
    static Library library; // Library object
    // std::vector<User> users;
    vector<Transaction> transactions; // vector of Transaction objects

public:
    ~LibraryManager();
    void start();                                        // Start the library management system
    void logTransaction(const Transaction &transaction); // Log a transaction
    void displayTransactions() const;
    void saveData() const;
    void loadData();
    std::tm calculateFutureDate(const std::tm &currentDate, int daysToAdd); // Calculate the future date, this give the due date of the book
    static Library getLibrary() { return library; }
    size_t getTransactionCount() const { return transactions.size(); }
};
