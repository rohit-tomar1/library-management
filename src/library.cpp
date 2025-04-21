#include "../include/library.h"
#include <iostream>
#include <algorithm>

std::vector<std::shared_ptr<Book>> Library::books;
std::vector<std::shared_ptr<User>> Library::users;

void Library::addBook(const Book &book)
{
    books.push_back(make_shared<Book>(book));
}

void Library::removeBook(int bookId)
{
    books.erase(remove_if(books.begin(), books.end(),
                          [bookId](const shared_ptr<Book> &book)
                          {
                              return book->getId() == bookId;
                          }),
                books.end());
}

const vector<shared_ptr<Book>> &Library::getBooks() const
{
    return books;
}

const vector<shared_ptr<User>> &Library::getUsers() const
{
    return users;
}

shared_ptr<Book> Library::searchBook(int bookId)
{
    for (const auto &book : books)
    {
        if (book->getId() == bookId)
        {
            return book;
        }
    }
    return nullptr;
}

void Library::addUser(const User &user)
{
    users.push_back(make_shared<User>(user));
}

bool Library::renameUser(int userId, const string &newName)
{
    auto user = searchUser(userId);
    if (!user)
    {
        return false;
    }
    user->setName(newName);
    return true;
}

bool Library::removeUser(int userId)
{
    auto user = searchUser(userId);
    if (!user)
    {
        return false;
    }
    // return all book before user deletion.
    user->returnAllBook();

    users.erase(remove_if(users.begin(), users.end(),
                          [userId](const shared_ptr<User> &user)
                          {
                              return user->getId() == userId;
                          }),
                users.end());
    return true;
}

shared_ptr<User> Library::searchUser(int userId) const
{
    for (const auto &user : users)
    {
        if (user->getId() == userId)
        {
            return user;
        }
    }
    return nullptr;
}

void Library::displayBorrowedBook(int userId) const
{
    auto user = searchUser(userId);
    if (user)
    {
        if (user->getBorrowedBooks().empty())
        {
            return;
        }

        // cout << "\nBorrowed Books:\n";

        for (const auto &borrowedBook : user->getBorrowedBooks())
        {
            auto book = searchBook(borrowedBook.book->getId());
            if (book)
            {
                cout << "\n    Book ID: " << book->getId() << ", DueDate: " << borrowedBook.dueDate << "\n";
            }
        }
    }
    else
    {
        cout << "\nUserId not exist.";
    }
}

bool Library::borrowBook(int userId, int bookId, string &dueDate)
{
    auto user = searchUser(userId);
    if (!user)
    {
        cout << "\nUser Id not exist.";
        return false;
    }
    auto book = searchBook(bookId);
    if (!book)
    {
        cout << "\nBook Id not exist.";
        return false;
    }
    if (book->getAvailableCopy() <= 0 || !book->borrowBook())
    {
        cout << "\nBook out of stock.";
        return false;
    }
    user->borrowBook(book, dueDate);
    return true;
}

bool Library::returnBook(int userId, int bookId) const
{
    auto user = searchUser(userId);
    if (!user)
    {
        cout << "User Id not exist.";
        return false;
    }
    auto book = searchBook(bookId);
    if (!book)
    {
        cout << "Book Id not exist.";
        return false;
    }
    book->returnBook();
    user->returnBook(*book);
    return true;
}

void Library::displayAllBooks() const
{
    for (const auto &book : books)
    {
        cout << book->display();
    }
}

void Library::displayAllUsers() const
{
    for (const auto &user : users)
    {
        user->displayUserDetails();
        // user->displayBorrowedBooks();
        Library::displayBorrowedBook(user->getId());
    }
}
