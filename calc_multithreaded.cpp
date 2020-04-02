#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

#define F_MODE 4

#if F_MODE == 1

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

#elif F_MODE == 2

//This version just changed the order of `j` and `k` which leads to a smaller number of cache-misses.
void f(const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, unsigned row_start, unsigned row_end) {
    const unsigned j_max = B[0].size();
    const unsigned k_max = B.size();
    for (int i = row_start; i < row_end; ++i) {
        for (int k = 0; k < k_max; ++k) {
            for (int j = 0; j < j_max; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

#elif F_MODE == 3

//Same as above, plus partial unrolling.
void f(const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, unsigned row_start, unsigned row_end) {

    static const unsigned num_unroll = 5;

    const unsigned j_max = B[0].size();
    const unsigned k_max_for_unrolled_loop = B.size() / num_unroll * num_unroll;
    const unsigned k_max = B.size();

    for (int i = row_start; i < row_end; ++i) {
        for (int k = 0; k < k_max_for_unrolled_loop; k += num_unroll) {
            for (int j = 0; j < j_max; ++j) {
                C[i][j] += A[i][k] * B[k][j];
                C[i][j] += A[i][k+1] * B[k+1][j];
                C[i][j] += A[i][k+2] * B[k+2][j];
                C[i][j] += A[i][k+3] * B[k+3][j];
                C[i][j] += A[i][k+4] * B[k+4][j];
            }
        }
        for (int k = k_max_for_unrolled_loop; k < k_max; ++k) {
            for (int j = 0; j < j_max; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

}

#elif F_MODE == 4

//Same as above, plus introduced `sum` to reduce the call of `C[i][j]`.
void f(const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, unsigned row_start, unsigned row_end) {

    static const unsigned num_unroll = 5;

    const unsigned j_max = B[0].size();
    const unsigned k_max_for_unrolled_loop = B.size() / num_unroll * num_unroll;
    const unsigned k_max = B.size();

    for (int i = row_start; i < row_end; ++i) {
        for (int k = 0; k < k_max_for_unrolled_loop; k += num_unroll) {
            for (int j = 0; j < j_max; ++j) {
                double sum;
                sum = A[i][k] * B[k][j];
                sum += A[i][k+1] * B[k+1][j];
                sum += A[i][k+2] * B[k+2][j];
                sum += A[i][k+3] * B[k+3][j];
                sum += A[i][k+4] * B[k+4][j];
                C[i][j] += sum;
            }
        }
        for (int k = k_max_for_unrolled_loop; k < k_max; ++k) {
            for (int j = 0; j < j_max; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

}

#elif F_MODE == 5

//Same as above, plus introduced `p` to the call of the operator function [].
//Note `p` is a pointer to the internal raw array, so that it should be faster than an iterator.
void f(const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, unsigned row_start, unsigned row_end) {

    static const unsigned num_unroll = 5;

    const unsigned j_max = B[0].size();
    const unsigned k_max_for_unrolled_loop = B.size() / num_unroll * num_unroll;
    const unsigned k_max = B.size();

    for (int i = row_start; i < row_end; ++i) {
        for (int k = 0; k < k_max_for_unrolled_loop; k += num_unroll) {
            for (int j = 0; j < j_max; ++j) {
                const double *p = A[i].data() + k;
                double sum;
                sum = *p++ * B[k][j];
                sum += *p++ * B[k+1][j];
                sum += *p++ * B[k+2][j];
                sum += *p++ * B[k+3][j];
                sum += *p++ * B[k+4][j];
                C[i][j] += sum;
            }
        }
        for (int k = k_max_for_unrolled_loop; k < k_max; ++k) {
            const double a = A[i][k];
            for (int j = 0; j < j_max; ++j) {
                C[i][j] += a * B[k][j];
            }
        }
    }

}

#elif F_MODE == 6

//Same as above without the introduction of `p`, plus using pointers intead of references.
//Pass arguments as pointers instead of `std::cref()` or `std::ref()`.
//Seems a little little little bit faster than the original.
void f(const vector<vector<double>> *A, const vector<vector<double>> *B, vector<vector<double>> *C, unsigned row_start, unsigned row_end) {

    static const unsigned num_unroll = 5;

    const unsigned j_max = B -> operator[](0).size();
    const unsigned k_max_for_unrolled_loop = B -> size() / num_unroll * num_unroll;
    const unsigned k_max = B -> size();

    for (int i = row_start; i < row_end; ++i) {
        for (int k = 0; k < k_max_for_unrolled_loop; k += num_unroll) {
            for (int j = 0; j < j_max; ++j) {
                double sum;
                sum = A -> operator[](i)[k] * B -> operator[](k)[j];
                sum += A -> operator[](i)[k+1] * B -> operator[](k+1)[j];
                sum += A -> operator[](i)[k+2] * B -> operator[](k+2)[j];
                sum += A -> operator[](i)[k+3] * B -> operator[](k+3)[j];
                sum += A -> operator[](i)[k+4] * B -> operator[](k+4)[j];
                C -> operator[](i)[j] += sum;
            }
        }
        for (int k = k_max_for_unrolled_loop; k < k_max; ++k) {
            for (int j = 0; j < j_max; ++j) {
                C -> operator[](i)[j] += A -> operator[](i)[k] * B -> operator[](k)[j];
            }
        }
    }

}

#elif F_MODE == 7

//Uses `column_*` instead of `row_*`, which means the input should be devided not by row but by column.
void f(const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, unsigned column_start, unsigned column_end) {

    static const unsigned num_unroll = 5;

    const unsigned k_max_for_unrolled_loop = B.size() / num_unroll * num_unroll;
    const unsigned k_max = B.size();
    const unsigned i_max = C.size();

    for (int i = 0; i < i_max; ++i) {
        for (int k = 0; k < k_max_for_unrolled_loop; k += num_unroll) {
            for (int j = column_start; j < column_end; ++j) {
                double sum;
                sum = A[i][k] * B[k][j];
                sum += A[i][k+1] * B[k+1][j];
                sum += A[i][k+2] * B[k+2][j];
                sum += A[i][k+3] * B[k+3][j];
                sum += A[i][k+4] * B[k+4][j];
                C[i][j] += sum;
            }
        }
        for (int k = k_max_for_unrolled_loop; k < k_max; ++k) {
            for (int j = column_start; j < column_end; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

}

#elif F_MODE == 8

//transpose version
//This first transpose `B` to reduce cache-misses.
//Perhaps transposing `B` before the call of this function is a better idea.
void f(const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, unsigned row_start, unsigned row_end) {
    vector<vector<double>> B_transposed(B[0].size(), vector<double>(B.size()));
    for (int i = 0; i< B_transposed.size(); ++i) {
        for (int j = 0; j < B_transposed[0].size(); ++j) {
            B_transposed[i][j] = B[j][i];
        }
    }
    const unsigned j_max = B[0].size();
    const unsigned k_max = B.size();
    for (int i = row_start; i < row_end; ++i) {
        for (int j = 0; j < j_max; ++j) {
            for (int k = 0; k < k_max; ++k) {
                C[i][j] += A[i][k] * B_transposed[j][k];
            }
        }
    }
}

#endif

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
//                                        &A,
//                                        &B,
//                                        &C,
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

