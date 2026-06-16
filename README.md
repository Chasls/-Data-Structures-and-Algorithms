# CS 300 ABCU Advising Program

## Overview

This repository contains my CS 300 portfolio work for the ABCU Course Advising Program. The goal of this project was to design and build a program that helps academic advisors load course information from a file, view all courses in alphanumeric order, and search for a specific course to see its title and prerequisites.

Throughout the course, I worked with multiple data structures including vectors, hash tables, and binary search trees. After comparing the strengths and weaknesses of each one, I chose a hash table for the final version because it provides fast course lookups by course number.

## Project Features

The final program allows the user to:

* Load course data from a CSV file
* Store course information in a hash table
* Print a full list of courses in alphanumeric order
* Search for a specific course by course number
* Display the selected course title and prerequisites
* Validate that listed prerequisites exist in the course file

## Data Structure Used

The final project uses a hash table through C++ `unordered_map`.

I chose this structure because the program needs to quickly find courses by course number. A hash table provides average constant-time lookup, which makes it a good fit for searching course records and retrieving prerequisite information.

Even though a hash table does not automatically keep courses sorted, the program handles that by copying the course numbers into a vector and sorting them before printing the full course list.

## What I Learned

This project helped me better understand how data structures affect program performance and design. A vector is simple and easy to use, but searching through it can take longer as the data grows. A binary search tree is useful for sorted output, but it can become inefficient if the tree is not balanced. A hash table works well when fast lookup is the main goal.

I also gained more practice with:

* File input in C++
* CSV parsing
* Data validation
* Sorting output
* Using structs to organize related data
* Comparing runtime complexity
* Writing cleaner and more reusable code

## Files Included

This repository may include:

* Final ABCU advising program source code
* Project pseudocode
* Runtime analysis
* Reflections from earlier milestones
* Supporting course planning documents

## How to Run

1. Open the project in a C++ development environment.
2. Make sure the course data CSV file is available.
3. Compile and run the C++ program.
4. Use the menu options to load the course data, print the course list, or search for a course.

## Author

Chas Stevens
CS 300: Data Structures and Algorithms
