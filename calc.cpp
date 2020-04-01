using namespace std;
#include <iostream>
#include <vector>
#include <chrono>

int main(int argc, char **argv) {

    if (argc != 2) {
        cerr << "Pass `size` as an argument.\n";
        return 1;
    }
    const unsigned size = atoi(argv[1]);

    vector<vector<double>> A(size, vector<double>(size));
    vector<vector<double>> B(size, vector<double>(size));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            A[i][j] = i * 3.14 + j;
            B[i][j] = i * 3.14 - j;
        }
    }

    auto start = chrono::system_clock::now();

    vector<vector<double>> C(size, vector<double>(size, /* initial_value = */ 0));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    cerr << scientific;
    cerr.precision(3);
    cerr << chrono::duration<double>(chrono::system_clock::now() - start).count() << "\n";

    cout << scientific;
    cout.precision(3);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << C[i][j] << "\n";
        }
    }

}

