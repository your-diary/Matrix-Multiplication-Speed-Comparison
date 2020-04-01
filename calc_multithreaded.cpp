using namespace std;
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

void f(const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, unsigned row_start, unsigned row_end) {
    const unsigned j_max = B[0].size();
    const unsigned k_max = B.size();
    for (int i = row_start; i < row_end; ++i) {
        for (int j = 0; j < j_max; ++j) {
            for (int k = 0; k < k_max; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char **argv) {

    if (argc != 3) {
        cerr << "Pass `size` and `num_thread` as arguments.\n";
        return 1;
    }
    const unsigned size = atoi(argv[1]);
    const unsigned num_thread = atoi(argv[2]);

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

    vector<thread> thread_array;
    for (int i = 0; i < num_thread; ++i) {
        const unsigned row_start = i * (size / num_thread);
        const unsigned row_end = (i == num_thread - 1 ? size : (i + 1) * (size / num_thread));
        thread_array.push_back(thread(
                                       f,
                                       cref(A),
                                       cref(B),
                                       ref(C),
                                       row_start,
                                       row_end
                                     ));
    }
    for (int i = 0; i < num_thread; ++i) {
        thread_array[i].join();
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

