#include <iostream>

long pow(long b, int p) {
    if (p == 0) {
        return 1;
    } else if (p == 1) {
        return b;
    } else if (p == 2) {
        return b * b;
    } else {
        long new_b = b * b;
        if (p % 2) {
            return b * pow(new_b, (p - 1)/2);
        } else {
            return pow(new_b, p/2);
        }
    }
}

int main(int argc, char* argv[]) {
    long base = std::stol(argv[1]);
    int power = std::stoi(argv[2]);

    std::cout << pow(base, power);

    return 0;
}
