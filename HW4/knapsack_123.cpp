#include <iostream>
#include <vector>
#include <random>
#include <chrono>

/*

Refernce for quick sort: https://www.geeksforgeeks.org/cpp-program-for-quicksort/ 
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
    float pivot = arr[high].value/arr[high].weight; 
    int i = (low- 1); 
    for (int j = low; j <= high - 1; j++) {
        if ((float)arr[j].value/arr[j].weight > pivot) {
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
    int n = item_number - 1;
    quickSort(arr, 0, n);
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
    items = input_items;
    quick_sort(items, this->item_number);
}

greedy::~greedy(){
    cout << "destructor called " << endl;
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
    
    cout << "deleted" << endl;
    
}

int dynamic_programming::run(){
    
    int **dp = new int*[item_number+1];
    for(int i = 0 ; i <= item_number ; i++){
        dp[i] = new int[max_capacity + 1];
    }

    for(int i = 0 ; i <= max_capacity ; i++){
        dp[0][i] = 0;
    }
    for(int i = 0 ; i <= item_number ; i++){
        dp[i][0] = 0;
    }


    if (item_number == 10000){
        
    }
    //algorithm from ppt 
    for (int i = 1; i <= item_number ; i++) {
        for (int j = 1; j <= max_capacity ; j++) {
            if (items[i - 1].weight <= j) {
                dp[i][j] = max(items[i - 1].value + dp[i - 1][j - items[i - 1].weight], dp[i - 1][j]);
            }
            else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    int max = dp[item_number][max_capacity];
    for (int i = 0; i <= item_number; i++) {
        delete[] dp[i];
    }
    delete[] dp;

    return max;
}

void dynamic_programming::print(){
    cout << "==============================================" << endl;
    for(int i = 0; i < item_number ; i++){ //iteration
        cout << (float)items[i].value/items[i].weight << " / " << items[i].value << " / " << items[i].weight << endl;
    }
}



int main(int argc, char* argv[]){

    int brute_force_sequence[] = {11, 21, 31};   

    //brute for execution
    
    for(int i = 0; i < sizeof(brute_force_sequence) / sizeof(int) ; i++){

        auto start = chrono::high_resolution_clock::now();
        
        
        brute_force brute_force_manner(brute_force_sequence[i]);
        // brute_force_manner.print();
        cout << brute_force_manner.run_first() << endl;

        auto end = chrono::high_resolution_clock::now();

        auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Code execution time: " << duration_ms << " ms" << endl;
    }

    int others[] = {10, 100, 1000}; 
    item* random_numbers[4];

    for(int i=0;i<4;i++){
        random_numbers[i] = randomly_generate(others[i]);
    }
    
    //greedy manner fractional knapsack
    for(int i = 0; i < sizeof(others) / sizeof(int) ; i++){

        auto start = chrono::high_resolution_clock::now();

        greedy* greedy_manner = new greedy(others[i], random_numbers[i]);
        // greedy_manner.print(); 
        cout << greedy_manner->run() << endl;

        auto end = chrono::high_resolution_clock::now();

        auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Greedy manner code execution time: " << others[i] << duration_ms << " ms" << endl;
        delete greedy_manner;
    }

    //dynamic programming
    for(int i = 0; i < sizeof(others) / sizeof(int) ; i++){

        auto start = chrono::high_resolution_clock::now();

        dynamic_programming* dp = new dynamic_programming(others[i], random_numbers[i]);
        // dynamic_programming_manner.print();
        cout << dp->run() << endl;

        auto end = chrono::high_resolution_clock::now();

        auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Dynamic manner code execution time: " << others[i] << duration_ms << " ms" << endl;
        delete dp;
        // sleep(3);
    }


    return 0;
}