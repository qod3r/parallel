import numpy as np

v1 = np.fromfunction(lambda x: 1.0 / (1.0 + x), (100000000,), dtype=np.float64)
v2 = np.fromfunction(lambda x: x / (1.0 + x), (100000000,), dtype=np.float64)

print(np.dot(v1, v2))