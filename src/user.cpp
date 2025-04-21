#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/user.h"
#include "../include/library.h"
#include "user.h"
using namespace std;

User::User() : id(0), name("") {}
User::User(int id, const string &name) : id(id), name(name) {}

int User::getId() const
{
    return id;
}

string User::getName() const
{
    return name;
}

const vector<BorrowedBook> &User::getBorrowedBooks() const
{
    return borrowedBooks;
}

void User::setName(const string &newName)
{
    name = newName;
}
bool User::borrowBook(std::shared_ptr<Book> newBook, const std::string &dueDate)
{
    if (!newBook)
    {
        return false;
    }

    auto it = std::find_if(borrowedBooks.begin(), borrowedBooks.end(),
                           [newBook](const BorrowedBook &book)
                           { return book.book->getId() == newBook->getId(); });

    // Check if book is already borrowed
    if (it != borrowedBooks.end())
    {
        return false;
    }

    borrowedBooks.emplace_back(newBook, dueDate);
    return true;
}

bool User::returnBook(Book &newBook)
{
    auto it = std::find_if(borrowedBooks.begin(), borrowedBooks.end(),
                           [newBook](const BorrowedBook &book)
                           { return book.book->getId() == newBook.getId(); });

    if (it != borrowedBooks.end())
    {
        borrowedBooks.erase(it);
        return true;
    }
    return true;
}

void User::returnAllBook()
{
    for (const auto &book : borrowedBooks)
    {
        book.book->returnBook();
    }
}

void User::displayUserDetails()
{
    cout << "\nUser Id: " << id << ", Name: " << name;
}

// void User::displayBorrowedBooks() {
// 	if (borrowedBooks.empty()) {
// 		// std::cout << "\n No books borrowed by " << name << " \n";
// 		return;
// 	}

// 	cout << "\n Borrowed Books:\n";

// 	for (const auto& book : borrowedBooks) {
// 		cout << "BookID: " << book.book->getId() << ", DueDate: " << book.dueDate << "\n";
// 	}

// }

void User::save(std::ofstream &out) const
{
    out << id << '\n'
        << name << '\n'
        << borrowedBooks.size() << '\n';
    for (const auto &borrowedBook : borrowedBooks)
    {
        out << borrowedBook.book->getId() << '\n';
        out << borrowedBook.dueDate << '\n';
    }
}

User User::load(std::ifstream &in)
{
    int id;
    std::string name;
    size_t borrowedBooksCount;
    in >> id;
    in.ignore();
    std::getline(in, name);
    in >> borrowedBooksCount;
    in.ignore();
    User user(id, name);
    for (size_t i = 0; i < borrowedBooksCount; ++i)
    {
        int bookId;
        std::string dueDate;
        in >> bookId;
        in.ignore();
        std::getline(in, dueDate);
        auto book = Library::searchBook(bookId);
        if (book)
        {
            user.borrowedBooks.emplace_back(book, dueDate);
        }
        else
        {
            std::cerr << "Error: Book with ID " << bookId << " not found in the library.\n";
        }
    }

    return user;
}