//============================================================================
// Name        : snhu-cs-260-linked-list.cpp
// Author      : Glenn Bacon
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : 7-1 Final Lists and Searching
//============================================================================

#include <time.h>

#include <algorithm>
#include <iostream>
#include <string>

#include "CSVparser.hpp"

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(std::string str, char ch);

// define a structure to hold bid information
struct Bid {
  std::string bidId;  // unique identifier
  std::string title;
  std::string fund;
  double amount;
  Bid() { amount = 0.0; }
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {
 private:
  // 2
  struct Node {
    Bid bid;
    Node* next;

    // default constructor
    Node() { next = nullptr; }

    // initialize a node with a bid
    Node(Bid aBid) {
      bid = aBid;
      next = nullptr;
    }
  };

  Node* head;
  Node* tail;
  int size = 0;

 public:
  LinkedList();
  virtual ~LinkedList();
  void Append(Bid bid);
  void Prepend(Bid bid);
  void PrintList();
  void Remove(std::string bidId);
  Bid Search(std::string bidId);
  int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
  // (2): Initialize housekeeping variables
  head = tail = nullptr;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
  // (3): Implement append logic
  Node* node = new Node(bid);

  if (head == nullptr) {
    head = tail = node;
  } else {
    if (tail != nullptr) {
      tail->next = node;
    }
  }

  // new node is always the tail
  tail = node;

  // Increment linked list size
  size++;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
  // (4): Implement prepend logic
  Node* node = new Node(bid);

  if (head != nullptr) {
    node->next = head;
  }

  // New node is always tail
  head = node;

  // Increment linked list size
  size++;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
  // (5): Implement print logic
  Node* current = head;

  // Loop over each node looking for a match
  while (current != nullptr) {
    std::cout << current->bid.bidId << ": " << current->bid.title << " | "
              << current->bid.amount << " | " << current->bid.fund << std::endl;
    current = current->next;
  }
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(std::string bidId) {
  Node* current = head;

  // (6): Implement remove logic
  // Only one node
  if (size == 1) {
    if (current->bid.bidId.compare(bidId) == 0) {
      current->next = head = tail = nullptr;
      size = 0;
    }
  } else {
    // Loop over each node looking for a match
    while (current->next != nullptr && size > 1) {
      if (current->next->bid.bidId.compare(bidId) == 0) {
        // Node is tail, update tail
        if (current->next->next == nullptr) {
          current->next = tail = nullptr;
        } else {
          // make current node point beyond the next one (to be removed)
          current->next = current->next->next;
        }

        // Decrement linked list size
        size--;

        return;
      }
      current = current->next;
    }
  }
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(std::string bidId) {
  // (7): Implement search logic
  Node* current = head;

  Bid emptyBid;

  // Loop over each node looking for a match
  while (current != nullptr) {
    if (current->bid.bidId.compare(bidId) == 0) {
      return current->bid;
    }
    current = current->next;
  }

  return emptyBid;
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() { return size; }

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
  std::cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << std::endl;
  return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
  Bid bid;
  std::string strAmount;

  std::cout << "Enter Id: ";
  std::cin.ignore();
  getline(std::cin, bid.bidId);

  std::cout << "Enter title: ";
  std::getline(std::cin, bid.title);

  std::cout << "Enter fund: ";
  std::getline(std::cin, bid.title);

  std::cout << "Enter amount: ";
  std::getline(std::cin, strAmount);
  bid.amount = strToDouble(strAmount, '$');

  return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(std::string csvPath, LinkedList* list) {
  std::cout << "Loading CSV file " << csvPath << std::endl;

  // initialize the CSV Parser
  csv::Parser file = csv::Parser(csvPath);

  try {
    // loop to read rows of a CSV file
    for (int i = 0; i < int(file.rowCount()); i++) {
      // initialize a bid using data from current row (i)
      Bid bid;
      bid.bidId = file[i][1];
      bid.title = file[i][0];
      bid.fund = file[i][8];
      bid.amount = strToDouble(file[i][4], '$');

      // cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " <<
      // bid.amount << endl;

      // add this bid to the end
      list->Append(bid);
    }
  } catch (csv::Error& e) {
    std::cerr << e.what() << std::endl;
  }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(std::string str, char ch) {
  str.erase(remove(str.begin(), str.end(), ch), str.end());
  return atof(str.c_str());
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {
  // process command line arguments
  std::string csvPath, bidKey;
  switch (argc) {
    case 2:
      csvPath = argv[1];
      bidKey = "98109";
      break;
    case 3:
      csvPath = argv[1];
      bidKey = argv[2];
      break;
    default:
      csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
      bidKey = "98109";
  }

  clock_t ticks;

  LinkedList bidList;

  Bid bid;

  int choice = 0;
  while (choice != 9) {
    std::cout << "Menu:" << std::endl;
    std::cout << "  1. Enter a Bid" << std::endl;
    std::cout << "  2. Load Bids" << std::endl;
    std::cout << "  3. Display All Bids" << std::endl;
    std::cout << "  4. Find Bid" << std::endl;
    std::cout << "  5. Remove Bid" << std::endl;
    std::cout << "  6. Prepend a Bid" << std::endl;
    std::cout << "  9. Exit" << std::endl;
    std::cout << "Enter choice: ";
    std::cin >> choice;

    switch (choice) {
      case 1:
        bid = getBid();
        bidList.Append(bid);
        displayBid(bid);

        break;

      case 2:
        ticks = clock();

        loadBids(csvPath, &bidList);

        std::cout << bidList.Size() << " bids read" << std::endl;

        ticks =
            clock() - ticks;  // current clock ticks minus starting clock ticks
        std::cout << "time: " << ticks << " milliseconds" << std::endl;
        std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds"
                  << std::endl;

        break;

      case 3:
        bidList.PrintList();
        std::cout << bidList.Size() << " bids read" << std::endl;
        break;

      case 4:
        std::cout << "Enter Bid Id: ";
        std::cin >> bidKey;
        ticks = clock();

        bid = bidList.Search(bidKey);

        ticks =
            clock() - ticks;  // current clock ticks minus starting clock ticks

        if (!bid.bidId.empty()) {
          std::cout << "Bid Id " << bidKey << " found." << std::endl;
          displayBid(bid);
        } else {
          std::cout << "Bid Id " << bidKey << " not found." << std::endl;
        }

        std::cout << "time: " << ticks << " clock ticks" << std::endl;
        std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds"
                  << std::endl;

        break;

      case 5:
        std::cout << "Enter Bid Id: ";
        std::cin >> bidKey;
        bidList.Remove(bidKey);

        break;

      case 6:
        bid = getBid();
        bidList.Prepend(bid);
        displayBid(bid);

        break;
    }
  }

  std::cout << "Good bye." << std::endl;

  return 0;
}
