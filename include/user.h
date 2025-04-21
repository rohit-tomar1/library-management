#pragma once

#ifndef USER_H
#define USER_H

#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include "book.h"

using namespace std;

struct BorrowedBook
{
	// Book* book;
	std::shared_ptr<Book> book; // shared pointer to Book object

	string dueDate; // Due date of the borrowed book

	// BorrowedBook(Book* book, const string& date) : book(book), dueDate(date) {}
	BorrowedBook(std::shared_ptr<Book> book, const std::string &date) : book(book), dueDate(date) {}
};

class User
{
private:
	int id;
	string name;
	vector<BorrowedBook> borrowedBooks;

public:
	User();
	User(int id, const string &name);

	int getId() const;
	string getName() const;
	const vector<BorrowedBook> &getBorrowedBooks() const;

	void setName(const string &newName);

	bool borrowBook(std::shared_ptr<Book> newBook, const std::string &dueDate);

	// bool borrowBook(Book *newBook, string &dueDate);

	bool returnBook(Book &book);
	void returnAllBook();
	void displayUserDetails();
	// void displayBorrowedBooks();
	void save(std::ofstream &out) const;
	static User load(std::ifstream &in);
};

#endif