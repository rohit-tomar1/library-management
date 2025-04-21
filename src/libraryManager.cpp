#include "../include/libraryManager.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <filesystem>
#include <fstream>

Library LibraryManager::library;

LibraryManager::~LibraryManager()
{
    saveData();
}

void LibraryManager::start()
{
    loadData(); // Load data when the system starts
    int choice;
    do
    {
        std::cout << "\nLibrary Management System:\n"
                  << "1. Add Book\n"
                  << "2. Remove Book\n"
                  << "3. Search Book\n"
                  << "4. Add User\n"
                  << "5. Remove User\n"
                  << "6. Edit User Details\n"
                  << "7. Borrow Book\n"
                  << "8. Return Book\n"
                  << "9. Display All Books Borrowed By User\n"
                  << "10. Display All Books\n"
                  << "11. Display All Users\n"
                  << "12. Display All Transactions\n"
                  << "13. Exit\n"
                  //   << "14. Load\n"
                  //   << "15. Save\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            // "1. Add Book"
            int id;
            std::string title, author;
            int totalCopy;
            std::cout << "Enter Book ID: ";
            std::cin >> id;
            std::cin.ignore();
            std::cout << "Enter Book Title: ";
            std::getline(std::cin, title);
            std::cout << "Enter Book Author: ";
            std::getline(std::cin, author);
            std::cout << "Enter Total Copy: ";
            std::cin >> totalCopy;
            std::cin.ignore();

            if (id > 0 && !title.empty() && !author.empty())
            {
                auto book = LibraryManager::library.searchBook(id);
                if (book)
                {
                    std::cout << "\nBook Id Already Exist.\n";
                    break;
                }

                library.addBook(Book(id, title, author, totalCopy, totalCopy));
                logTransaction(Transaction(static_cast<int>(transactions.size()) + 1, 0, id, "Book Added"));
                std::cout << "\nBook added successfully.\n";
            }
            else
            {
                std::cout << "\nInvalid input. Please try again.\n";
            }
            break;
        }
        case 2:
        {
            // 2. Remove Book
            int bookId;
            std::cout << "Enter Book ID: ";
            std::cin >> bookId;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nInvalid input. Please enter a valid Book ID.\n";
                break;
            }

            library.removeBook(bookId);
            logTransaction(Transaction(static_cast<int>(transactions.size()) + 1, 0, bookId, "Book Removed"));
            std::cout << "\nBook removed successfully.\n";
            break;
        }
        case 3:
        {
            // 3. Search Book
            int bookId;
            std::cout << "Enter Book ID: ";
            std::cin >> bookId;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nInvalid input. Please enter a valid Book ID.\n";
                break;
            }

            auto bo = library.searchBook(bookId);
            if (bo)
            {
                cout << bo->display();
            }
            else
            {
                std::cout << "\nBook not found.\n";
            }
            break;
        }
        case 4:
        {
            // 4. Add User
            int userId;
            std::string name;
            std::cout << "Enter User ID: ";
            std::cin >> userId;
            std::cin.ignore();
            std::cout << "Enter User Name: ";
            std::getline(std::cin, name);

            if (userId > 0 && !name.empty())
            {
                auto book = library.searchUser(userId);
                if (book)
                {
                    std::cout << "\nUser Id Already Exist.\n";
                    break;
                }
                library.addUser(User(userId, name));
                logTransaction(Transaction(static_cast<int>(transactions.size()) + 1, userId, 0, "User Added"));
                std::cout << "\nUser added successfully.\n";
            }
            else
            {
                std::cout << "\nInvalid input. Please try again.\n";
            }
            break;
        }
        case 5:
        {
            // 5. Remove User
            int userId;
            std::cout << "Enter User ID: ";
            std::cin >> userId;

            if (!library.removeUser(userId))
            {
                cout << "User Id not exist.";
                break;
            }
            logTransaction(Transaction(static_cast<int>(transactions.size()) + 1, userId, 0, "User Removed"));
            std::cout << "\nUser removed successfully.\n";
            break;
        }
        case 6:
        {
            // 6. Edit User Details
            int userId = 0;
            std::string name;
            std::cout << "Enter User ID: ";
            std::cin >> userId;
            std::cin.ignore();
            std::cout << "Enter User New Name: ";
            std::getline(std::cin, name);

            if (!library.renameUser(userId, name))
            {
                cout << "\nUser Id not exist.";
                break;
            }
            logTransaction(Transaction(static_cast<int>(transactions.size()) + 1, userId, 0, "User Updated"));
            std::cout << "\nUser Details Updated successfully.\n";
            break;
        }
        case 7:
        {
            // 7. Borrow Book
            int userId, bookId;
            std::cout << "Enter User ID: ";
            std::cin >> userId;
            std::cout << "Enter Book ID: ";
            std::cin >> bookId;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nInvalid input. Please enter valid IDs.\n";
                break;
            }
            std::time_t t = std::time(nullptr);
            std::tm currentDate;
            currentDate = *std::localtime(&t);

            // Calculate the future date
            std::tm dueDateTm = calculateFutureDate(currentDate, 14);

            // Format the future date as a string
            std::stringstream ss;
            ss << std::put_time(&dueDateTm, "%Y-%m-%d");
            std::string dueDate = ss.str();
            if (library.borrowBook(userId, bookId, dueDate))
            {
                logTransaction(Transaction(static_cast<int>(transactions.size()) + 1, userId, bookId, "Book Borrowed"));
                std::cout << "\nBook borrowed successfully. Due date: " << dueDate << "\n";
            }
            else
            {
                std::cout << "\nFailed to borrow book.\n";
            }
            break;
        }
        case 8:
        {
            // 8. Return Book
            int userId, bookId;
            std::cout << "Enter User ID: ";
            std::cin >> userId;
            std::cout << "Enter Book ID: ";
            std::cin >> bookId;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nInvalid input. Please enter valid IDs.\n";
                break;
            }

            if (library.returnBook(userId, bookId))
            {
                logTransaction(Transaction(static_cast<int>(transactions.size()) + 1, userId, bookId, "Book Returned"));
                std::cout << "\nBook returned successfully.\n";
            }
            break;
        }
        case 9:
        {
            // 9. Display All Books Borrowed By User
            int userId;
            std::cout << "Enter User ID: ";
            std::cin >> userId;
            std::cin.ignore();
            std::cout << "\nBorrowed By User:\n";
            library.displayBorrowedBook(userId);
            break;
        }
        case 10:
        {
            // 10. Display All Books
            std::cout << "\nAll Books:\n";
            library.displayAllBooks();
            break;
        }
        case 11:
        {
            // 11. Display All Users
            std::cout << "\nAll Users:\n";
            library.displayAllUsers();
            break;
        }
        case 12:
        {
            // 12. Display All Transactions
            std::cout << "\nAll Transaction:\n";
            displayTransactions();
            break;
        }
        case 13:
            std::cout << "Exiting Library Management System. Goodbye!\n";
            saveData();
            exit(0);
            break;
        // case 14:
        //     std::cout << "Load Data!\n";
        //     loadData();
        //     break;
        // case 15:
        //     std::cout << "Save Data\n";
        //     saveData();
        //     break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 13);
}

void LibraryManager::logTransaction(const Transaction &transaction)
{
    cout << "\nTransaction logged";
    transactions.push_back(transaction);
}

void LibraryManager::displayTransactions() const
{
    if (transactions.empty())
    {
        std::cout << "\nNo Transaction Recorded!\n";
        return;
    }

    std::cout << "\nAll Transaction:\n";
    for (const auto &transaction : transactions)
    {
        transaction.displayDetails();
    }
}

// int main()
// {
//     std::cout << "Hello World!\n";
//     LibraryManager obj = LibraryManager();
//     obj.start();
// }

// Calculate the future date, this give the due date of the book
std::tm LibraryManager::calculateFutureDate(const std::tm &currentDate, int daysToAdd)
{
    std::tm futureDate = currentDate;
    futureDate.tm_mday += daysToAdd;
    std::mktime(&futureDate);
    return futureDate;
}

void LibraryManager::saveData() const
{
    const std::string directory = "./data";
    const std::string filePath = directory + "/library_data.txt";
    std::filesystem::create_directories(directory);

    // Open the file for writing
    std::ofstream outFile(filePath);
    if (!outFile)
    {
        cout << "Error in file opening: " << std::endl;
        std::cerr << "Error opening file for writing.\n";
        return;
    }

    // Save books
    const auto &books = library.getBooks();
    outFile << books.size() << '\n';
    for (const auto &book : books)
    {
        book->save(outFile);
    }

    // Save users
    const auto &users = library.getUsers();
    outFile << users.size() << '\n';
    for (const auto &user : users)
    {
        user->save(outFile);
    }

    // Save transactions
    outFile << transactions.size() << '\n';
    for (const auto &transaction : transactions)
    {
        outFile << transaction.getTransactionId() << '\n'
                << transaction.getUserId() << '\n'
                << transaction.getBookId() << '\n'
                << transaction.getType() << '\n'
                << transaction.getTimestamp() << '\n';
    }
}

void LibraryManager::loadData()
{
    const std::string filePath = "./data/library_data.txt";
    std::ifstream inFile(filePath);
    if (!inFile)
    {
        std::cerr << "Error opening file for reading.\n";
        return;
    }

    // Check if the file is empty
    if (std::filesystem::file_size(filePath) == 0)
    {
        return;
    }

    // Load books
    size_t bookCount;
    inFile >> bookCount;
    inFile.ignore();
    for (size_t i = 0; i < bookCount; ++i)
    {
        Book book = Book::load(inFile);
        auto b = library.searchBook(book.getId());
        if (b != nullptr)
        {
            library.removeBook(book.getId());
        }
        library.addBook(book);
    }

    // Load users
    size_t userCount;
    inFile >> userCount;
    inFile.ignore();
    for (size_t i = 0; i < userCount; ++i)
    {
        User user = User::load(inFile);
        auto u = library.searchUser(user.getId());
        if (u != nullptr)
        {
            library.removeUser(user.getId());
        }
        library.addUser(user);
    }

    // Load transactions
    transactions = {};
    size_t transactionCount;
    inFile >> transactionCount;
    inFile.ignore();
    for (size_t i = 0; i < transactionCount; ++i)
    {
        int id, userId, bookId;
        std::string action, date;
        inFile >> id >> userId >> bookId;
        inFile.ignore();
        std::getline(inFile, action);
        std::getline(inFile, date);
        transactions.emplace_back(id, userId, bookId, action, date);
    }
}