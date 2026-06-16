//============================================================================
// Name        : HashTable.cpp
// Author      : Charles Stevens
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 4-2 Hash Table
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>
#include <vector>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
    size_t Size();
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // Create the hash table buckets using the default table size.
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // Store the requested table size and create that many buckets.
    this->tableSize = size;
    nodes.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // Each bucket stores the first node directly in the vector. Any collisions
    // are stored as linked nodes, so those need to be deleted manually.
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        Node* current = nodes.at(i).next;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }

        nodes.at(i).next = nullptr;
    }

    nodes.clear();
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // The modulo operation keeps the key inside the table boundaries.
    return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // Create the hash key from the bid ID.
    unsigned int key = hash(atoi(bid.bidId.c_str()));

    // If this bucket is empty, store the bid directly in the vector bucket.
    if (nodes.at(key).key == UINT_MAX) {
        nodes.at(key) = Node(bid, key);
        return;
    }

    // If the bucket is already being used, walk the linked list and add the
    // new bid to the end. This handles collisions using chaining.
    Node* current = &nodes.at(key);
    while (current->next != nullptr) {
        current = current->next;
    }

    current->next = new Node(bid, key);
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // Visit every bucket in the table and print each bid stored there.
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        Node* current = &nodes.at(i);

        while (current != nullptr && current->key != UINT_MAX) {
            cout << "Key " << current->key << ": " << current->bid.bidId << " | "
                 << current->bid.title << " | " << current->bid.amount << " | "
                 << current->bid.fund << endl;

            current = current->next;
        }
    }
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // Find the bucket where this bid should be stored.
    unsigned int key = hash(atoi(bidId.c_str()));
    Node* current = &nodes.at(key);
    Node* previous = nullptr;

    // Search the bucket chain for the matching bid ID.
    while (current != nullptr && current->key != UINT_MAX) {
        if (current->bid.bidId == bidId) {
            // If the bid is in the first bucket node, either promote the next
            // chained node or reset the bucket to an empty state.
            if (previous == nullptr) {
                if (current->next != nullptr) {
                    Node* temp = current->next;
                    current->bid = temp->bid;
                    current->key = temp->key;
                    current->next = temp->next;
                    delete temp;
                } else {
                    current->bid = Bid();
                    current->key = UINT_MAX;
                    current->next = nullptr;
                }
            } else {
                previous->next = current->next;
                delete current;
            }
            return;
        }

        previous = current;
        current = current->next;
    }
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // Find the bucket where this bid should be stored.
    unsigned int key = hash(atoi(bidId.c_str()));
    Node* current = &nodes.at(key);

    // Search the bucket and any chained nodes for the matching bid ID.
    while (current != nullptr && current->key != UINT_MAX) {
        if (current->bid.bidId == bidId) {
            return current->bid;
        }

        current = current->next;
    }

    return bid;
}

/**
 * Return the number of bids currently stored in the hash table.
 */
size_t HashTable::Size() {
    size_t count = 0;

    for (unsigned int i = 0; i < nodes.size(); ++i) {
        Node* current = &nodes.at(i);
        while (current != nullptr && current->key != UINT_MAX) {
            ++count;
            current = current->next;
        }
    }

    return count;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
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
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();
    
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
