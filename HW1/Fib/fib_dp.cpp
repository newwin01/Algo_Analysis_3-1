#include <iostream>

using namespace std;

// Fib(n){
//     int fib[n+1];
// }

int main() {
    long long n;
    cout << "Enter number: ";
    cin >> n;

    clock_t start = 0, stop = 0;
	double duration = 0;
	start = clock();

    // long long fib_value = Fib(n);



    stop = clock();
	duration = (double) (stop - start) / CLOCKS_PER_SEC;

    cout << "Time elapsed is " << duration << "seconds\n" << "answer: "  << endl;

    return 0;
}