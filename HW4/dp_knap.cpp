#include <iostream>
#include <vector>
#include <algorithm>

// Structure to represent an item
struct Item {
    int weight;
    int value;
};

// Function to solve Knapsack problem using a brute force approach
int knapsackBruteForce(std::vector<Item>& items, int capacity, int currentIndex) {
    // Base case: if there are no more items or the capacity is 0
    if (currentIndex == items.size() || capacity == 0) {
        return 0;
    }

    // If the current item's weight exceeds the capacity, skip it
    if (items[currentIndex].weight > capacity) {
        return knapsackBruteForce(items, capacity, currentIndex + 1);
    }

    // Recursive case:
    // Try including the current item and excluding the current item,
    // and return the maximum value obtained from the two options
    int includeItem = items[currentIndex].value + knapsackBruteForce(items, capacity - items[currentIndex].weight, currentIndex + 1);
    int excludeItem = knapsackBruteForce(items, capacity, currentIndex + 1);
    return std::max(includeItem, excludeItem);
}

int main() {
    // Example usage
    std::vector<Item> items = {
        {10, 60},
        {20, 100},
        {30, 120}
    };

    int capacity = 50;

    int maxValue = knapsackBruteForce(items, capacity, 0);
    std::cout << "Maximum value achievable: " << maxValue << std::endl;

    return 0;
}
