import numpy as np

# 生成80x60的矩阵
matrix1 = np.random.rand(80, 60)

# 生成60x80的矩阵
matrix2 = np.random.rand(60, 80)

# 计算矩阵乘积
result = np.dot(matrix1, matrix2)

print(result)