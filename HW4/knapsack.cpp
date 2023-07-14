#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <queue>
#include <time.h>
/*
HW4 21900628

For chrono library need to specify gcc version using "-std=c++11"
Refernce for quick sort: https://www.geeksforgeeks.org/cpp-program-for-quicksort/ 
Get helps from node implementation from chat GPT in branch and bound

*/

using namespace std;

typedef struct{
    int value;
    int weight;
} item;

item* randomly_generate(int item_number){
    item* items;
    items = new item[item_number];

    srand(100);

    for(int i = 0; i < item_number ; i++){ //iteration
        items[i].value = rand() % 500 + 1;
        items[i].weight = rand() % 100 + 1;
    }
    
    #ifdef _DEBUG
        for(int i = 0; i < item_number ; i++){ //iteration
            cout << items[i].value << " / " << items[i].weight << endl;
        }
    #endif
    
    return items;
}

void swap(item* a, item* b)
{
    item temp;
    temp = *a;
    a->value = b->value;
    a->weight = b->weight;
    b->weight = temp.weight;
    b->value = temp.value;
}


//quick sort as weight / pivot
int partition(item* arr, int low, int high)
{
    float pivot = (float)arr[high].value/arr[high].weight; 
    int i = (low - 1); 

    for (int j = low; j <= high - 1; j++) {
        if ( (float)arr[j].value/arr[j].weight > pivot) {
            i++; 
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(item* arr, int low, int high)
{
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }

}

void quick_sort(item* arr, int item_number){
    
    quickSort(arr, 0, item_number - 1);
}

class brute_force{
    private:
        int item_number;
        item *items;
        int max_capacity;
    public:
        brute_force(int item_number);
        ~brute_force();
        void print();
        int run(int capacity, int current_index);
        int run_first();
};

brute_force::brute_force(int item_number){
    this->item_number = item_number;
    items = randomly_generate(item_number);
    max_capacity = 25 * item_number;
}

brute_force::~brute_force(){
    delete items;
}

int brute_force::run(int capacity, int current_index){
    if (current_index ==  item_number || capacity == 0) {
        return 0;
    }
    if (items[current_index].weight > capacity) {
        return run(capacity, current_index + 1);
    }

    // Recursive case:
    int includeItem = items[current_index].value + run(capacity - items[current_index].weight, current_index + 1);    
    int excludeItem = run(capacity, current_index + 1);
    return max(includeItem, excludeItem);
}

int brute_force::run_first(){
    
    return run(max_capacity,0);
}

void brute_force::print(){
    cout << "==============================================" << endl;
    for(int i = 0; i < item_number ; i++){ //iteration
        cout << (float)items[i].value/items[i].weight << " / " << items[i].value << " / " << items[i].weight << endl;
    }
}

class greedy{
    private:
        int item_number;
        item *items;
        int max_capacity;
    public:
        greedy(int item_number, item* input_items);
        ~greedy();
        void print();
        int run();

};

greedy::greedy(int item_number, item* input_items){
    this->item_number = item_number;
    max_capacity = 25 * item_number;
    items = new item[item_number];
    copy(input_items, input_items+item_number,items);
    quick_sort(items, item_number);
}

greedy::~greedy(){
    // cout << "destructor called " << endl;
    delete[] items;
}

int greedy::run(){
    
    int totalVal = 0;
    int cur_weight = 0;

    for (int i=0 ; i < item_number ; i++) {
        // Check if adding the current item exceeds capacity
    if (cur_weight + items[i].weight <= max_capacity) {
        cur_weight += items[i].weight;
        totalVal += items[i].value;
     } else {
        // If not, add a fraction of the current item to fill up the capacity
        int remainingCapacity = max_capacity - cur_weight;
        double fraction = (float)(remainingCapacity) / items[i].weight;
        totalVal += (int)(items[i].value * fraction);
        break;
        }
    }

    return totalVal;

}

void greedy::print(){
    cout << "==============================================" << endl;
    for(int i = 0; i < item_number ; i++){ //iteration
        cout << (float)items[i].value/items[i].weight << " / " << items[i].value << " / " << items[i].weight << endl;
    }
}


class dynamic_programming{
    private:
        int item_number;
        item *items;
        int max_capacity;
    public:
        dynamic_programming(int item_number, item* input_item);
        ~dynamic_programming();
        void print();
        int run();
};

dynamic_programming::dynamic_programming(int item_number, item* input_item){
    max_capacity = 25 * item_number;
    this->item_number = item_number;
    items = input_item;
}

dynamic_programming::~dynamic_programming(){
    
    // cout << "deleted" << endl;
    
}

int dynamic_programming::run(){
    int **dp;
    int max_number;
    int partial_item ; 
    int epoch;

    //preprocessing for item numbers
    if(item_number == 10000){
        partial_item = 1000;
        epoch = 10 ; 
    } else {
        partial_item = item_number; 
        epoch = 1 ;
    }

    dp = new int*[partial_item+1];
    for(int i = 0 ; i <= partial_item ; i++){
        dp[i] = new int[max_capacity + 1];
    }
    
    for(int i = 0 ; i <= max_capacity ; i++){
        dp[0][i] = 0;
    }
    for(int i = 0 ; i <= partial_item ; i++){
        dp[i][0] = 0;
    }

    for (int rotation = 0 ; rotation < epoch ; rotation++ ){ // Assume 
        for (int i = 1; i <= partial_item ; i++) {
            for (int j = 1; j <= max_capacity ; j++) {
                if (items[partial_item * rotation + i - 1].weight <= j) {
                    dp[i][j] = max(items[partial_item * rotation + i - 1].value + dp[i - 1][j - items[partial_item * rotation + i - 1].weight], dp[i - 1][j]);
                }
                else {
                    dp[i][j] = dp[i - 1][j];
                }
            }
        }
        for(int i = 0 ; i <= max_capacity ; i++){
            dp[0][i] = dp[partial_item][i];
        }
    }
    max_number = dp[partial_item][max_capacity];

    for (int i = 0 ; i <= partial_item ; i++) {
        delete[] dp[i];
    }
    delete[] dp;

    return max_number;
}

void dynamic_programming::print(){
    cout << "==============================================" << endl;
    for(int i = 0; i < item_number ; i++){ //iteration
        cout << (float)items[i].value/items[i].weight << " / " << items[i].value << " / " << items[i].weight << endl;
    }
}

//Node stuct for branch and bound
typedef struct{
    int level;
    int profit;
    int bound; 
    float weight;
} node;

bool operator<(const node& a, const node& b) {
    return a.bound < b.bound;
}

class BAB{
    private:
        int item_number;
        item *items;
        int max_capacity;
        int max_profit;
    public:
        BAB(int item_number, item* input_items);
        ~BAB();
        void print();
        int run();
        int getBound(node insert);

};

BAB::BAB(int item_number, item* input_items){
    this->item_number = item_number;
    max_capacity = 25 * item_number;
    items = new item[item_number];
    copy(input_items, input_items+item_number,items);
    quick_sort(items, item_number);
    max_profit = 0;
}

BAB::~BAB(){
    delete[] items;
    // cout << "destructor called" << endl;
}

void BAB::print(){
    cout << "==============================================" << endl;
    for(int i = 0; i < item_number ; i++){ //iteration
        cout << (float)items[i].value/items[i].weight << " / " << items[i].value << " / " << items[i].weight << endl;
    }
}

int BAB::getBound(node insert){

    if(insert.weight >= max_capacity){
        return 0;
    }

    int bound = insert.profit;

    int total_weight = insert.weight;
    int temp_level = insert.level + 1;

    while((temp_level < item_number) && (total_weight + items[temp_level].weight <= max_capacity))
    {
        total_weight = total_weight + items[temp_level].weight;
        bound = bound + items[temp_level].value;
        temp_level++;
    }
    
    if(bound < 0){
        cout << " bound error" ;
        exit(1);
    }

    if (temp_level < item_number){ //partial addition for remaining weight
        bound = bound + ((max_capacity - total_weight) * items[temp_level].value/items[temp_level].weight);
         
    }

    return bound;
}

int BAB::run(){
    
   priority_queue<node> pq;
    node u, v;


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
        if (v.level == item_number - 1) {
            continue;
        }
        u.level = v.level + 1;
        u.weight = v.weight + items[u.level].weight;
        u.profit = v.profit + items[u.level].value;


        if (u.weight <= max_capacity && u.profit > max_profit) {
            max_profit = u.profit;
        }

        u.bound = getBound(u);

        if (u.bound > max_profit) {
            pq.push(u);
        }

        u.weight = v.weight;
        u.profit = v.profit;
        u.bound = getBound(u);

        if (u.bound > max_profit) {
            pq.push(u);
        }
        
    }

    return max_profit;
}



int main(int argc, char* argv[]){

    
    #ifdef _TEST
        int others[] = {30}; 
        int brute_force_sequence[] = {5};   
    #else
        int others[] = {10, 100, 1000, 10000}; 
        int brute_force_sequence[] = {11, 21, 31};   
    #endif
    
    item* random_numbers[sizeof(others)/sizeof(int)];

    for(int i=0 ; i<sizeof(others)/sizeof(int); i++){
        random_numbers[i] = randomly_generate(others[i]);
    }


    float brute_time[3];
    int brute_max[3];
    int max;

    //brute for execution
        
    for(int i = 0; i < sizeof(brute_force_sequence) / sizeof(int) ; i++){
        
        brute_force *brute_force_manner = new brute_force(brute_force_sequence[i]);

        auto start = chrono::high_resolution_clock::now();
        
        // brute_force_manner.print();
        max = brute_force_manner->run_first();

        auto end = chrono::high_resolution_clock::now();

        auto duration_ms = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        
        #ifdef _DEBUG 
            cout << "Code execution time: " << (double)duration_ms/1000000 << " ms" << endl;
            cout << max << endl;
        #endif

        brute_time[i] = (float)duration_ms/1000000;
        brute_max[i] = max;

        delete brute_force_manner;
    }

    float greedy_time[4];
    int greedy_max[4];

    // greedy manner fractional knapsack
    for(int i = 0; i < sizeof(others) / sizeof(int) ; i++){


        greedy *greedy_manner = new greedy(others[i], random_numbers[i]);
        auto start = chrono::high_resolution_clock::now();
        max = greedy_manner->run();
        
        auto end = chrono::high_resolution_clock::now();

        auto duration_ms = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        
        #ifdef _DEBUG 
            cout << "Code execution time: " << (double)duration_ms/1000000 << " ms" << endl;
            cout << max << endl;
        #endif

        greedy_time[i] = (float)duration_ms/1000000;
        greedy_max[i] = max;
        delete greedy_manner;
    }

    float dp_time[4];
    int dp_max[4];

    for(int i = 0; i < sizeof(others) / sizeof(int) ; i++){

        dynamic_programming* dp = new dynamic_programming(others[i], random_numbers[i]);

        auto start = chrono::high_resolution_clock::now();
        
        max = dp->run();
        
        auto end = chrono::high_resolution_clock::now();

        auto duration_ms = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        
        #ifdef _DEBUG 
            cout << "Code execution time: " << (double)duration_ms/1000000 << " ms" << endl;
            cout << max << endl;
        #endif

        dp_time[i] = (float)duration_ms/1000000;
        dp_max[i] = max;
        delete dp;
        
    }

    float BAB_time[4];
    int BAB_max[4];

    for(int i = 0; i < sizeof(others) / sizeof(int) ; i++){

        BAB *bab = new BAB(others[i], random_numbers[i]);

        auto start = chrono::high_resolution_clock::now();
        
        max = bab->run();
        // bab->print();

        auto end = chrono::high_resolution_clock::now();

        auto duration_ms = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        
        #ifdef _DEBUG 
            cout << "Code execution time: " << (double)duration_ms/1000000 << " ms" << endl;
            cout << max << endl;
        #endif

        BAB_time[i] = (float)duration_ms/1000000;
        BAB_max[i] = max;
        delete bab;
        
    }


    //result execution
    cout << "Number of items | Processing time in milliseconds / Maximum benefit value" << endl;
    for(int i=0 ; i < sizeof(brute_force_sequence)/sizeof(int) ; i++){
        cout << "\t" << brute_force_sequence[i] << "\t|\t\t\t" << brute_time[i] << " / " << brute_max[i] << endl;
    }
    cout << "==================================================================================" << endl;

    
    cout << "Number of items | Processing time in milliseconds / Maximum benefit value" << endl;
    cout << "                |       Greedy      |         D. P.       |       B. & B.       " << endl;
    for(int i=0 ; i < sizeof(others)/sizeof(int) ; i++){
        cout << "\t" << others[i] << "\t|   " << greedy_time[i] << " / " << greedy_max[i]
        << "   |   " << dp_time[i] << " / " << dp_max[i] 
        << "   |   " << BAB_time[i] << " / " << BAB_max[i] << endl;
    }
    cout << "==================================================================================" << endl;


    return 0;
}