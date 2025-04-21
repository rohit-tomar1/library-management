#pragma once

#include <fstream>
#include <string>

class Book
{
private:
    int id;
    std::string title;
    std::string author;
    int totalCopy;
    int availableCopy; // represents the number of copies available to borrow

public:
    Book(int id, const std::string &title, const std::string &author, int totalCopy, int availableCopy);
    int getId() const;
    int getAvailableCopy() const;
    bool borrowBook();
    bool returnBook();
    std::string display() const;
    void save(std::ofstream &out) const; // Save book data to file
    static Book load(std::ifstream &in); // Load book data from file
};