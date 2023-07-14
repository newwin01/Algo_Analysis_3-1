#include <iostream>
#include <vector>
using namespace std;

class Item {
public:
    int weight;
    int value;
};

class DynamicProgramming {
private:
    int item_number;
    vector<Item> items;
    int max_capacity;
public:
    DynamicProgramming(int item_number);
    void print();
    int run();
};

DynamicProgramming::DynamicProgramming(int item_number) {
    max_capacity = 25 * item_number;
    this->item_number = item_number;
    items.resize(item_number);
    for (int i = 0; i < item_number; i++) {
        items[i].weight = rand() % (max_capacity / 2) + 1; // random weight between 1 and max_capacity/2
        items[i].value = rand() % (max_capacity / 2) + 1;  // random value between 1 and max_capacity/2
    }
}

int DynamicProgramming::run() {
    vector<vector<int>> dp(item_number + 1, vector<int>(max_capacity + 1, 0));

    // algorithm from ppt
    for (int i = 1; i <= item_number; i++) {
        for (int j = 1; j <= max_capacity; j++) {
            if (items[i - 1].weight <= j) {
                dp[i][j] = max(items[i - 1].value + dp[i - 1][j - items[i - 1].weight], dp[i - 1][j]);
            }
            else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    return dp[item_number][max_capacity];
}

void DynamicProgramming::print() {
    cout << "==============================================" << endl;
    for (int i = 0; i < item_number; i++) {
        cout << (float)items[i].value / items[i].weight << " / " << items[i].value << " / " << items[i].weight << endl;
    }
}

int main(){
    DynamicProgramming *dp = new DynamicProgramming(10000);
}