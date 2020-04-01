# Introduction

Using nested `std::vector` to represent matrices [is a bad idea](https://scicomp.stackexchange.com/questions/3159/is-it-a-good-idea-to-use-vectorvectordouble-to-form-a-matrix-class-for-high/3162). In this project, we investigate how it is bad by comparing the speed of matrix multiplication implemented via

1. nested `std::vector` in `C++`

2. `numpy` in `Python 3`

# Usage

## Preparations

```bash
$ git clone https://github.com/your-diary/Matrix-Multiplication-Speed-Comparison #downloads this project
Cloning into 'Matrix-Multiplication-Speed-Comparison'...
remote: Enumerating objects: 11, done.
remote: Counting objects: 100% (11/11), done.
remote: Compressing objects: 100% (8/8), done.
remote: Total 11 (delta 3), reused 11 (delta 3), pack-reused 0
Receiving objects: 100% (11/11), done.
Resolving deltas: 100% (3/3), done.

$ cd Matrix-Multiplication-Speed-Comparison/ #moves to the project directory
```

## Usage

```bash
$ bash comparison.sh N
```

By this, two `N`x`N` matrices are multiplied. Only square matrices are currently supported.

At most four threads are used by default. You can just change the line `num_thread=4` in `comparison.sh` to use another value.

## Example

**Command:**

```bash
$ for size in 100 200 300 400; do bash comparison.sh ${size}; done
```

**Output:**

```
matrix_size: 100x100
--------------- Time in seconds ---------------
                          calc.out: 4.702e-03
calc_multithreaded.out (1 threads): 2.036e-03
calc_multithreaded.out (2 threads): 8.144e-04
calc_multithreaded.out (3 threads): 4.705e-04
calc_multithreaded.out (4 threads): 4.040e-04
                           calc.py: 1.862e-04
-----------------------------------------------

matrix_size: 200x200
--------------- Time in seconds ---------------
                          calc.out: 8.513e-03
calc_multithreaded.out (1 threads): 8.591e-03
calc_multithreaded.out (2 threads): 4.636e-03
calc_multithreaded.out (3 threads): 3.174e-03
calc_multithreaded.out (4 threads): 2.427e-03
                           calc.py: 4.220e-04
-----------------------------------------------

matrix_size: 300x300
--------------- Time in seconds ---------------
                          calc.out: 2.922e-02
calc_multithreaded.out (1 threads): 2.918e-02
calc_multithreaded.out (2 threads): 1.517e-02
calc_multithreaded.out (3 threads): 1.046e-02
calc_multithreaded.out (4 threads): 7.964e-03
                           calc.py: 7.839e-04
-----------------------------------------------

matrix_size: 400x400
--------------- Time in seconds ---------------
                          calc.out: 6.986e-02
calc_multithreaded.out (1 threads): 6.974e-02
calc_multithreaded.out (2 threads): 3.639e-02
calc_multithreaded.out (3 threads): 2.499e-02
calc_multithreaded.out (4 threads): 1.919e-02
                           calc.py: 1.486e-03
-----------------------------------------------
```

# Requirements

This project is cross-platform but tested only under Arch Linux.

- GNU Make

- g++

- Python 3

- Bash

<!-- vim: set spell: -->

