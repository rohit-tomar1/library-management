# Library Management

**Library Management** is a simple yet powerful Library Management System built in C++. It allows users to manage a catalog of books, perform borrow/return operations, and operate in different modes: Console, Server, or Client.

---

##  1. Project Overview

Library Management provides functionality to:

-  Manage books and users  
-  Handle transactions such as borrowing and returning books  
-  Support different operation modes:
  - **Console Application** (local usage)
  - **Server** (host a library server)
  - **Client** (connect remotely to a library server)

---

##  2. Dependencies

Ensure the following are installed on your system:

- `C++17` or later  
- `g++` (GNU Compiler Collection)  
- `make` (GNU Make utility)

---

##  3. Installation

To build the project, open a terminal in the project root and run:

```bash
make
```

This compiles the source files and generates the executable in the `./bin/` directory.

---

##  4. Running the Application

Once built, you can run the program with:

```bash
./bin/LincolnLibrary
```

---

##  5. Usage Instructions

After launching the application, you'll be prompted to select a mode of operation:

```
Welcome to Library Management!
Please select a mode:
1. Run Console Application
2. Run as Server
3. Run as Client
Enter your choice:
```

- **Option 1**: Launches the interactive console-based library system for local use.
- **Option 2**: Starts the application in server mode, allowing clients to connect remotely.
- **Option 3**: Starts the application in client mode to connect to a remote library server.

---