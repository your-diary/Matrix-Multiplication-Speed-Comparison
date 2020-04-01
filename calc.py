import numpy as np
import time
import sys

if (len(sys.argv) != 2):
    print("Pass `size` as an argument.", file = sys.stderr);
    sys.exit(1);
size = int(sys.argv[1]);

A = np.ndarray((size, size));
B = np.ndarray((size, size));

for i in range(size):
    for j in range(size):
        A[i][j] = i * 3.14 + j
        B[i][j] = i * 3.14 - j

start = time.time()
C = np.dot(A, B);
print("{:.3e}".format(time.time() - start), file = sys.stderr);

for i in range(size):
    for j in range(size):
        print("{:.3e}".format(C[i][j]));

