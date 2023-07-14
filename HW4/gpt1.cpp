#include <iostream>
#include <queue>
#include <vector>

using namespace std;



struct item {
    int weight;
    int value;
};

struct node {
    int level;
    int profit;
    int weight;
    int bound;
};

bool operator<(const node& a, const node& b) {
    return a.bound < b.bound;
}

class BranchAndBound {
   private:
    int item_number;
    int max_capacity;
    vector<item> items;

   public:
    BranchAndBound(int item_number);
    int run();
};

BranchAndBound::BranchAndBound(int item_number) {
    this->item_number = item_number;
    max_capacity = 25 * item_number;
    items.resize(item_number);
    srand(100);
    for (int i = 0; i < item_number; i++) {
        items[i].weight = rand() % 100 + 1;
        items[i].value = rand() % 500 + 1;
        // cout << items[i].weight << " " << items[i].value << endl;
    }
}

int getBound(node insert, const vector<item>& items, int max_capacity, int item_number) {
    if (insert.weight >= max_capacity) {
        return 0;
    }

    int bound = insert.profit;

    int total_weight = insert.weight;
    int temp_level = insert.level + 1;

    while ((temp_level < item_number) && (total_weight + items[temp_level].weight <= max_capacity)) {
        total_weight = total_weight + items[temp_level].weight;
        bound = bound + items[temp_level].value;
        temp_level++;
    }

    if (temp_level < item_number) {  //partial addition for remaining weight
        bound = bound + ((max_capacity - total_weight) * items[temp_level].value / items[temp_level].weight);
    }

    return bound;
}


int BranchAndBound::run() {
    priority_queue<node> pq;
    node u, v;
    vector<bool> taken(item_number);

    v.level = -1;
    v.profit = 0;
    v.weight = 0;
    v.bound = 0;
    pq.push(v);

    int max_profit = 0;
    while (!pq.empty()) {
        v = pq.top();
        pq.pop();
        if (v.level == -1) {
            u.level = 0;
        }
        if (v.level == max_capacity - 1) {
            continue;
        }
        u.level = v.level + 1;
        u.weight = v.weight + items[u.level].weight;
        u.profit = v.profit + items[u.level].value;
        taken[u.level] = true;

        if (u.weight <= max_capacity && u.profit > max_profit) {
            max_profit = u.profit;
        }

        u.bound = getBound(u, items, max_capacity, item_number);

        if (u.bound > max_profit) {
            pq.push(u);
        }

        taken[u.level] = false;

        u.weight = v.weight;
        u.profit = v.profit;
        u.bound = getBound(u, items, max_capacity, item_number);

        if (u.bound > max_profit) {
            pq.push(u);
        }
        cout << u.level << " " ; 
    }

    return max_profit;
}


int main() {
    int item_number = 10000;
    BranchAndBound bab(item_number);

    cout << "Max Profit: " << bab.run() << endl;

    return 0;
}
