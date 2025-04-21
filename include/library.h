#pragma once

#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <memory>

#include "book.h"
#include "user.h"

class Library
{
private:
	static std::vector<std::shared_ptr<Book>> books; // vector of shared pointers to Book objects
	static std::vector<std::shared_ptr<User>> users; // vector of shared pointers to User objects
	// vector<shared_ptr<Book>> books;
	// vector<shared_ptr<User>> users;

public:
	void addBook(const Book &book);
	void removeBook(int bookId);
	const vector<shared_ptr<Book>> &getBooks() const;
	const vector<shared_ptr<User>> &getUsers() const;

	// shared_ptr<Book> searchBook(int bookId) const;
	static std::shared_ptr<Book> searchBook(int bookId);

	void addUser(const User &user);
	bool renameUser(int userId, const string &newName);
	bool removeUser(int userId);
	shared_ptr<User> searchUser(int userId) const;
	void displayBorrowedBook(int userId) const;

	bool borrowBook(int userId, int bookId, string &dueDate);
	bool returnBook(int userId, int bookId) const;

	void displayAllBooks() const;
	void displayAllUsers() const;
};

#endif