```cpp
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

class Room {
public:
    string name;
    int capacity;

    Room(string name, int capacity) {
        this->name = name;
        this->capacity = capacity;
    }
};

int main() {
    int studentCount;

    cout << "Enter number of students: ";
    cin >> studentCount;

    if (studentCount < 0) {
        cout << "Error: Number of students cannot be negative.\n";
        return 0;
    }

    map<string, set<string>> studentCourses;
    set<string> allCourses;

    // Read student course registrations.
    for (int i = 1; i <= studentCount; i++) {
        int count;

        cout << "Enter number of courses for student S"
             << i << ": ";
        cin >> count;

        if (count < 0) {
            cout << "Error: Course count cannot be negative.\n";
            return 0;
        }

        set<string> enrolled;

        cout << "Enter course names: ";

        for (int j = 0; j < count; j++) {
            string course;
            cin >> course;

            enrolled.insert(course);
            allCourses.insert(course);
        }

        studentCourses["S" + to_string(i)] = enrolled;
    }

    // Read room details.
    int roomCount;

    cout << "Enter number of rooms: ";
    cin >> roomCount;

    if (roomCount < 0) {
        cout << "Error: Number of rooms cannot be negative.\n";
        return 0;
    }

    vector<Room> rooms;

    for (int i = 1; i <= roomCount; i++) {
        string name;
        int capacity;

        cout << "Enter room name and capacity for room "
             << i << ": ";

        cin >> name >> capacity;

        if (capacity < 0) {
            cout << "Error: Room capacity cannot be negative.\n";
            return 0;
        }

        rooms.emplace_back(name, capacity);
    }

    if (allCourses.empty()) {
        cout << "No courses were entered.\n";
        return 0;
    }

    map<string, set<string>> graph;
    map<string, int> enrollmentCount;

    for (const string& course : allCourses) {
        graph[course] = set<string>();
        enrollmentCount[course] = 0;
    }

    // Build the conflict graph and count enrollments.
    for (const auto& student : studentCourses) {
        const set<string>& enrolled = student.second;

        for (const string& course : enrolled) {
            enrollmentCount[course]++;
        }

        vector<string> courseList(
            enrolled.begin(), enrolled.end()
        );

        for (int i = 0; i < (int)courseList.size(); i++) {
            for (int j = i + 1;
                 j < (int)courseList.size(); j++) {

                string c1 = courseList[i];
                string c2 = courseList[j];

                graph[c1].insert(c2);
                graph[c2].insert(c1);
            }
        }
    }

    // Welsh-Powell algorithm:
    // Sort courses by descending vertex degree.
    vector<string> order(
        allCourses.begin(), allCourses.end()
    );

    sort(order.begin(), order.end(),
         [&](const string& a, const string& b) {

        int degreeA = graph[a].size();
        int degreeB = graph[b].size();

        if (degreeA != degreeB) {
            return degreeA > degreeB;
        }

        return a < b;
    });

    // Assign the smallest available color to each course.
    map<string, int> color;

    for (const string& course : order) {
        set<int> usedColors;

        for (const string& neighbor : graph[course]) {
            if (color.find(neighbor) != color.end()) {
                usedColors.insert(color[neighbor]);
            }
        }

        int assignedColor = 1;

        while (usedColors.count(assignedColor)) {
            assignedColor++;
        }

        color[course] = assignedColor;
    }

    int slotCount = 0;

    for (const auto& entry : color) {
        slotCount = max(slotCount, entry.second);
    }

    // Sort rooms by ascending capacity.
    vector<Room> roomsByCapacity = rooms;

    sort(roomsByCapacity.begin(), roomsByCapacity.end(),
         [](const Room& a, const Room& b) {

        if (a.capacity != b.capacity) {
            return a.capacity < b.capacity;
        }

        return a.name < b.name;
    });

    map<string, string> assignedRoom;
    bool allocationFailed = false;

    // Allocate rooms independently for each time slot.
    for (int slot = 1; slot <= slotCount; slot++) {
        vector<string> exams;

        for (const string& course : allCourses) {
            if (color[course] == slot) {
                exams.push_back(course);
            }
        }

        // Schedule exams with more students first.
        sort(exams.begin(), exams.end(),
             [&](const string& a, const string& b) {

            if (enrollmentCount[a] != enrollmentCount[b]) {
                return enrollmentCount[a] > enrollmentCount[b];
            }

            return a < b;
        });

        set<string> occupiedRooms;

        for (const string& course : exams) {
            const Room* chosen = nullptr;

            for (const Room& room : roomsByCapacity) {
                if (occupiedRooms.count(room.name) == 0
                    && room.capacity >= enrollmentCount[course]) {

                    chosen = &room;
                    break;
                }
            }

            if (chosen == nullptr) {
                allocationFailed = true;
                assignedRoom[course] = "NO ROOM";
            } else {
                assignedRoom[course] = chosen->name;
                occupiedRooms.insert(chosen->name);
            }
        }
    }

    // Display the final timetable.
    cout << "\n========== UNIVERSITY EXAM TIMETABLE ==========\n";

    cout << "Conflict Graph: Courses sharing students "
         << "cannot share a time slot.\n";

    cout << "Coloring Algorithm: Welsh-Powell\n";
    cout << "Total Courses: " << allCourses.size() << '\n';
    cout << "Total Exam Slots Used: " << slotCount << '\n';

    cout << "\nCourse Conflicts:\n";

    for (const string& course : allCourses) {
        cout << course << " -> {";

        bool first = true;

        for (const string& neighbor : graph[course]) {
            if (!first) {
                cout << ", ";
            }

            cout << neighbor;
            first = false;
        }

        cout << "}\n";
    }

    cout << "\nFinal Timetable:\n";

    cout << left
         << setw(10) << "Course"
         << setw(12) << "Time Slot"
         << setw(10) << "Students"
         << setw(12) << "Room" << '\n';

    for (const string& course : allCourses) {
        cout << left
             << setw(10) << course
             << setw(12) << ("Slot " + to_string(color[course]))
             << setw(10) << enrollmentCount[course]
             << setw(12) << assignedRoom[course]
             << '\n';
    }

    cout << "\nRoom Allocation Policy: Smallest available room "
         << "that fits the exam.\n";

    if (allocationFailed) {
        cout << "Warning: One or more exams could not be "
             << "assigned a suitable room.\n";
    } else {
        cout << "Room Allocation Status: All exams "
             << "assigned successfully.\n";
    }

    cout << "================================================\n";

    return 0;
}
```
