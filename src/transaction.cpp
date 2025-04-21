#include "../include/transaction.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

std::string getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm ptm;
    localtime_r(&tt, &ptm);

    std::stringstream ss;
    ss << std::put_time(&ptm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

Transaction::Transaction(int transactionId, int userId, int bookId, const std::string &type)
    : transactionId(transactionId), userId(userId), bookId(bookId), type(type)
{
    this->timestamp = getCurrentTime();
}

Transaction::Transaction(int transactionId, int userId, int bookId, const std::string &type, const std::string &timestamp)
    : transactionId(transactionId), userId(userId), bookId(bookId), type(type), timestamp(timestamp) {}

int Transaction::getTransactionId() const { return transactionId; }

int Transaction::getUserId() const { return userId; }

int Transaction::getBookId() const { return bookId; }

std::string Transaction::getType() const { return type; }

std::string Transaction::getTimestamp() const
{
    // std::ostringstream oss;
    // std::tm tm = *std::localtime_s(&timestamp);
    // oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    // return oss.str();
    return timestamp;
}

void Transaction::displayDetails() const
{
    std::cout << "Transaction ID: " << transactionId
              << ", User ID: " << userId
              << ", Book ID: " << bookId
              << ", Type: " << type
              << ", Timestamp: " << getTimestamp() << std::endl;
}