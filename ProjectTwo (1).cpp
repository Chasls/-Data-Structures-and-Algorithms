#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Stores one course and any prerequisites tied to it.
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Trim spaces and extra line characters from a string.
string trim(const string& value) {
    size_t start = value.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }

    size_t end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

// Make course lookups easier by treating user input the same no matter the case.
string toUpperCase(string value) {
    for (char& letter : value) {
        letter = static_cast<char>(toupper(static_cast<unsigned char>(letter)));
    }
    return value;
}

// Split one CSV line into separate fields.
vector<string> splitCSVLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream ss(line);

    while (getline(ss, field, ',')) {
        fields.push_back(trim(field));
    }

    return fields;
}

// Load the course file into a hash table.
bool loadCourses(const string& fileName, unordered_map<string, Course>& courses) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file " << fileName << "." << endl;
        return false;
    }

    unordered_map<string, Course> tempCourses;
    string line;

    while (getline(inputFile, line)) {
        if (trim(line).empty()) {
            continue;
        }

        vector<string> fields = splitCSVLine(line);

        if (fields.size() < 2 || fields.at(0).empty() || fields.at(1).empty()) {
            cout << "Error: Invalid course line found: " << line << endl;
            return false;
        }

        Course course;
        course.courseNumber = toUpperCase(fields.at(0));
        course.courseTitle = fields.at(1);

        for (size_t i = 2; i < fields.size(); ++i) {
            string prerequisite = toUpperCase(fields.at(i));
            if (!prerequisite.empty()) {
                course.prerequisites.push_back(prerequisite);
            }
        }

        tempCourses[course.courseNumber] = course;
    }

    // Make sure every prerequisite listed actually exists in the file.
    for (const auto& pair : tempCourses) {
        const Course& course = pair.second;
        for (const string& prerequisite : course.prerequisites) {
            if (tempCourses.find(prerequisite) == tempCourses.end()) {
                cout << "Error: " << course.courseNumber << " lists " << prerequisite
                     << " as a prerequisite, but that course was not found." << endl;
                return false;
            }
        }
    }

    courses = tempCourses;
    cout << "Course data loaded successfully." << endl;
    return true;
}

// Print all courses in alphanumeric order.
void printCourseList(const unordered_map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "Please load the course data first." << endl;
        return;
    }

    vector<string> courseNumbers;

    for (const auto& pair : courses) {
        courseNumbers.push_back(pair.first);
    }

    sort(courseNumbers.begin(), courseNumbers.end());

    cout << "Here is a sample schedule:" << endl;
    for (const string& courseNumber : courseNumbers) {
        const Course& course = courses.at(courseNumber);
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }
}

// Print one course and its prerequisites.
void printCourseInformation(const unordered_map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "Please load the course data first." << endl;
        return;
    }

    string courseNumber;
    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    courseNumber = toUpperCase(trim(courseNumber));

    auto courseSearch = courses.find(courseNumber);
    if (courseSearch == courses.end()) {
        cout << "Course not found." << endl;
        return;
    }

    const Course& course = courseSearch->second;
    cout << course.courseNumber << ", " << course.courseTitle << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites: ";
    for (size_t i = 0; i < course.prerequisites.size(); ++i) {
        const string& prerequisiteNumber = course.prerequisites.at(i);
        const Course& prerequisiteCourse = courses.at(prerequisiteNumber);

        cout << prerequisiteCourse.courseNumber << " - " << prerequisiteCourse.courseTitle;

        if (i < course.prerequisites.size() - 1) {
            cout << "; ";
        }
    }
    cout << endl;
}

void printMenu() {
    cout << endl;
    cout << "Welcome to the course planner." << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

int main() {
    unordered_map<string, Course> courses;
    int choice = 0;

    while (choice != 9) {
        printMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "That is not a valid option." << endl;
            continue;
        }

        cin.ignore(10000, '\n');

        switch (choice) {
            case 1: {
                string fileName;
                cout << "Enter the course data file name: ";
                getline(cin, fileName);
                loadCourses(trim(fileName), courses);
                break;
            }
            case 2:
                printCourseList(courses);
                break;
            case 3:
                printCourseInformation(courses);
                break;
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << choice << " is not a valid option." << endl;
                break;
        }
    }

    return 0;
}
