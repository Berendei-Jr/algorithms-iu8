#include <iostream>

struct matrix {
    long long data[2][2]{1,1,1,0};
    matrix operator*(const matrix& m) {
        matrix new_matrix {0, 0, 0, 0};
        for (int x = 0; x < 2; ++x) {
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    new_matrix.data[x][i] += data[x][j] * m.data[j][i];
                }
            }
        }
        return new_matrix;
    }
};

template<class T>
T pow(T b, int p) {
    if (p == 1) {
        return b;
    } else if (p == 2) {
        return b * b;
    } else {
        T new_b = b * b;
        if (p % 2) {
            return b * pow(new_b, (p - 1)/2);
        } else {
            return pow(new_b, p/2);
        }
    }
}

int main() {
    matrix Q;
    int n; std::cin >> n;
    if (n < 0)
        return 0;
    if (n == 0) {
        std::cout << 0 << std::endl;
        return 0;
    } else if (n == 1) {
        std::cout << 1 << std::endl;
        return 0;
    }
    std::cout << pow<matrix>(Q, n - 1).data[0][0] << std::endl;
    return 0;
}
