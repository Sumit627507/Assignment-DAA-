```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

class Item {
public:
    string name;
    int weight;
    int utility;

    Item(string name, int weight, int utility) {
        this->name = name;
        this->weight = weight;
        this->utility = utility;
    }
};

void knapsack(vector<Item>& items, int capacity) {
    int n = items.size();

    // dp[i][w] stores maximum utility using
    // the first i items with capacity w.
    vector<vector<int>> dp(
        n + 1, vector<int>(capacity + 1, 0)
    );

    // Build the DP table.
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {

            if (items[i - 1].weight <= w) {
                int include = items[i - 1].utility
                    + dp[i - 1][w - items[i - 1].weight];

                int exclude = dp[i - 1][w];

                dp[i][w] = max(include, exclude);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Maximum achievable utility.
    int maxUtility = dp[n][capacity];

    // Trace back to identify selected items.
    vector<Item> selectedItems;
    int remainingCapacity = capacity;

    for (int i = n; i > 0; i--) {
        if (dp[i][remainingCapacity]
            != dp[i - 1][remainingCapacity]) {

            selectedItems.push_back(items[i - 1]);

            remainingCapacity -= items[i - 1].weight;
        }
    }

    reverse(selectedItems.begin(), selectedItems.end());

    // Display results.
    cout << "\n========== DISASTER RELIEF RESOURCE ALLOCATION ==========\n";
    cout << "Truck Capacity: " << capacity << " kg\n";

    cout << "\nAvailable Relief Items:\n";

    cout << left
         << setw(5) << "No."
         << setw(20) << "Item"
         << setw(12) << "Weight (kg)"
         << setw(12) << "Utility" << '\n';

    for (int i = 0; i < n; i++) {
        cout << left
             << setw(5) << i + 1
             << setw(20) << items[i].name
             << setw(12) << items[i].weight
             << setw(12) << items[i].utility << '\n';
    }

    cout << "\nSelected Relief Items:\n";

    cout << left
         << setw(5) << "No."
         << setw(20) << "Item"
         << setw(12) << "Weight (kg)"
         << setw(12) << "Utility" << '\n';

    int totalWeight = 0;
    int totalUtility = 0;

    if (selectedItems.empty()) {
        cout << "No items selected.\n";
    } else {
        for (int i = 0; i < (int)selectedItems.size(); i++) {
            Item item = selectedItems[i];

            cout << left
                 << setw(5) << i + 1
                 << setw(20) << item.name
                 << setw(12) << item.weight
                 << setw(12) << item.utility << '\n';

            totalWeight += item.weight;
            totalUtility += item.utility;
        }
    }

    cout << "\n==================== FINAL RESULT ====================\n";
    cout << "Maximum Utility       : " << maxUtility << '\n';
    cout << "Total Weight Carried  : " << totalWeight << " kg\n";
    cout << "Remaining Capacity    : "
         << capacity - totalWeight << " kg\n";

    cout << "\nTime Complexity       : O(N * W)\n";
    cout << "Space Complexity      : O(N * W)\n";
    cout << "======================================================\n";
}

int main() {
    int n, capacity;

    cout << "Enter the number of relief items: ";
    cin >> n;

    cout << "Enter the truck capacity (kg): ";
    cin >> capacity;

    if (n < 0 || capacity < 0) {
        cout << "Error: Number of items and capacity "
             << "cannot be negative.\n";
        return 0;
    }

    vector<Item> items;
    items.reserve(n);

    cout << "\nEnter details for each item:\n";

    for (int i = 0; i < n; i++) {
        string name;
        int weight, utility;

        cout << "\nItem " << i + 1 << '\n';

        cout << "Enter item name (one word): ";
        cin >> name;

        cout << "Enter weight (kg): ";
        cin >> weight;

        cout << "Enter utility value: ";
        cin >> utility;

        if (weight <= 0 || utility < 0) {
            cout << "Error: Weight must be positive and "
                 << "utility cannot be negative.\n";
            return 0;
        }

        items.emplace_back(name, weight, utility);
    }

    knapsack(items, capacity);

    return 0;
}
```
