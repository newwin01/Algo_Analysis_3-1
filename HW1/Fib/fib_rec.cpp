#include <iostream>

using namespace std;

long long Fib (long long n){
    if(n < 2) {
        return n;
    } 
    else {
        return Fib(n-1) + Fib(n-2);
    }
}

int main() {
    long long n;
    cout << "Enter number: ";
    cin >> n;

    clock_t start = 0, stop = 0;
	double duration = 0;
	start = clock();

    long long fib_value = Fib(n);

    stop = clock();
	duration = (double) (stop - start) / CLOCKS_PER_SEC;

    cout << "Time elapsed is " << duration << "seconds\n" << "answer: " << fib_value << endl;

    return 0;
}