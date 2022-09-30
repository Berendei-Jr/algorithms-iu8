Q = ([[1, 1], [1, 0]])

def matrix_mul(a, b):
    newQ = ([[0,0], [0,0]])
    for x in range(2):
        for i in range(2):
            for j in range(2):
                    newQ[x][i] += a[x][j] * b[j][i]
    return newQ

def quickMatrixPow(b, p):
    if (p == 0):
        return [[1, 0], [0, 1]]
    elif (p == 1):
        return b
    elif (p == 2):
        return matrix_mul(b, b)
    else:
        new_b = matrix_mul(b, b)
        if (p % 2):
            return matrix_mul(b, quickMatrixPow(new_b, (p - 1)/2))
        else:
            return quickMatrixPow(new_b, p/2)

n = int(input())
if (n < 0):
    exit(0)
elif (n == 0):
    print(0)
    exit(0)
print(int(quickMatrixPow(Q, n-1)[0][0]))
