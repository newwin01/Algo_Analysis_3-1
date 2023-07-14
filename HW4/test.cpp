#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

struct Item {
    int weight;
    int value;
};

class dynamic_programming {
private:
    int item_number;
    vector<Item> items;
    int max_capacity;
    unordered_map<int, unordered_map<int, int>> memo;

public:
    dynamic_programming(int item_number);
    void print();
    int knapsack(int i, int capacity);
    int run();
};

dynamic_programming::dynamic_programming(int item_number) {
    max_capacity = 25 * item_number;
    this->item_number = item_number;
    items.resize(item_number);
    for (int i = 0; i < item_number; i++) {
        items[i].weight = rand() % 100 + 1; // Random weight between 1 and 100
        items[i].value = rand() % 100 + 1; // Random value between 1 and 100
    }
}

int dynamic_programming::knapsack(int i, int capacity) {
    if (i == item_number || capacity == 0) {
        return 0;
    }
    if (memo[i][capacity]) {
        return memo[i][capacity];
    }
    int res;
    if (items[i].weight > capacity) {
        res = knapsack(i + 1, capacity);
    } else {
        res = max(knapsack(i + 1, capacity), items[i].value + knapsack(i + 1, capacity - items[i].weight));
    }
    memo[i][capacity] = res;
    return res;
}

int dynamic_programming::run() {
    return knapsack(0, max_capacity);
}

void dynamic_programming::print() {
    cout << "==============================================" << endl;
    for (int i = 0; i < item_number; i++) {
        cout << "Item " << i + 1 << ": " << (float)items[i].value / items[i].weight << " / " << items[i].value << " / " << items[i].weight << endl;
    }
}

int main(){

    dynamic_programming *dp = new dynamic_programming(10000);
    cout << dp->run() << endl;

    return 0;
}