//
// Created by sergo on 12/5/19.
//

#include <chrono>
#include <random>

#include "matrix.hpp"

using namespace std;

template<typename T>
Matrix<T>::Matrix(size_t n, size_t m): height(n), width(m) {
    size_t pointersSpaceSize = sizeof(T *) * n;
    size_t elementsRowSize = sizeof(T) * m;
    data = reinterpret_cast<T **>(new char[pointersSpaceSize + elementsRowSize * n]);
    for (size_t i = 0; i < n; ++i) {
        data[i] = reinterpret_cast<T *>(reinterpret_cast<char *>(data) +
                                        pointersSpaceSize + i * m * sizeof(T));
    }
}

template<typename T>
Matrix<T>::Matrix(size_t n): Matrix(n, n) {}

template<typename T>
T *Matrix<T>::operator[](size_t n) {
    return data[n];
}

template<typename T>
bool Matrix<T>::operator==(Matrix<T>& other) {
    if (height != other.height or width != other.width) {
        return false;
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }

    return true;
}

template<typename T>
Matrix<T>::Matrix(T** &&array, size_t height, size_t width) {
    data = array;
    array = nullptr;
    this->height = height;
    this->width = width;
}

template<typename T>
[[nodiscard]] size_t Matrix<T>::getWidth() const {
    return width;
}

template<typename T>
[[nodiscard]] size_t Matrix<T>::getHeight() const {
    return height;
}

template<typename T> Matrix<T>::~Matrix() {
    if (data) {
        delete[] data;
        data = nullptr;
    }
}

template <typename T>
std::ostream &operator<<(std::ostream &out, Matrix<T> &matrix) {
    for (int i = 0; i < matrix.getHeight(); ++i) {
        for (int j = 0; j < matrix.getWidth(); ++j) {
            out << matrix[i][j] << ' ';
        }
        out << endl;
    }
    return out;
}

template<typename T>
void randomizeMatrix(Matrix<T> &matrix) {
    int low = 0;
    int high = 1e6;
    auto now = chrono::system_clock::now();
    default_random_engine generator(now.time_since_epoch().count());
    uniform_int_distribution<T> distribution(low, high);
    int i, j;
#pragma omp parallel for default(none), private(i, j), shared(distribution, generator, matrix)
    for (i = 0; i < matrix.getHeight(); ++i) {
#pragma omp parallel for default(shared), private(j), shared(distribution, generator, matrix)
        for (j = 0; j < matrix.getWidth(); ++j) {
            matrix[i][j] = distribution(generator);
        }
    }
}

template class Matrix<int>;
template class Matrix<uint64_t>;

template std::ostream& operator<<<int>(std::ostream&, Matrix<int>&);
template std::ostream& operator<<<uint64_t>(std::ostream&, Matrix<uint64_t>&);

template void randomizeMatrix<int>(Matrix<int>&);
template void randomizeMatrix<uint64_t>(Matrix<uint64_t>&);
