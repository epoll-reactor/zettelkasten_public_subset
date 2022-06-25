// Внесите следующие изменения в предыдущую программу:

// 1. Добавьте перегруженную версию оператора operator *= (const Matrix& other),
// умножающую текущую матрицу на other. Заметьте, что оператор должен вернуть ссылку
// на текущую модифицированную матрицу.
//
// 2. Добавьте реализацию константного оператора operator * (const Matrix& other),
// возвращающую произведение матриц.
//
// Можете считать, что размеры матрицы не превосходят 100, а значения — целые числа,
// не превосходящие 100 по модулю.

// Проверьте согласованность размеров перемножаемых матриц с помощью стандартной
// конструкции assert (https://en.cppreference.com/w/cpp/error/assert).

#pragma once
#include <assert.h>
#include <iostream>
#include <vector>
template<typename T>
class Matrix {
private:
    mutable std::vector<std::vector<T>> matrix;

public:
    Matrix(const std::vector<std::vector<T>>& input) {
        matrix = input;
    }
    ~Matrix() {
        for (size_t i = 0; i < matrix.size(); ++i) {
            matrix[i].pop_back();
        }
    }
    const std::pair<size_t, size_t> size() const {
        return std::make_pair(matrix.size(), matrix[0].size());
    }
    friend std::ostream& operator<<(std::ostream& out, const Matrix<T>& m) {
        for (size_t i = 0; i < m.matrix.size(); ++i) {
            out << m.matrix[i][0];
            for (size_t j = 1; j < m.matrix[i].size(); ++j) {
                out << "\t" << m.matrix[i][j];
            }
            if (i < m.matrix.size() - 1) {
                out << "\n";
            }
        }
        return out;
    }
    Matrix const operator+ (const Matrix& right) const {
        Matrix result(this->matrix);
        for (size_t i = 0; i < result.matrix.size(); ++i) {
            for (size_t j = 0; j < result.matrix[i].size(); ++j) {
                result.matrix[i][j] += right.matrix[i][j];
            }
        }
        return result;
    }
    Matrix const operator+= (const Matrix& right) {
        *this = *this + right;
        return *this;
    }
    Matrix const operator* (const T& scalar) const {
        Matrix result(this->matrix);
        for (size_t i = 0; i < result.matrix.size(); ++i) {
            for (size_t j = 0; j < result.matrix[i].size(); ++j) {
                result.matrix[i][j] *= scalar;
            }
        }
        return result;
    }
    Matrix const operator*= (const T& scalar) {
        *this = *this * scalar;
        return *this;
    }
    Matrix const operator* (const Matrix& other) const {
        assert(matrix.size() == other.matrix[0].size());
        Matrix result(this->matrix);
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < other.matrix[0].size(); ++j) {
                result.matrix[i].resize(other.matrix[0].size());
                result.matrix[i][j] = 0;
                for (size_t k = 0; k < matrix[0].size(); k++) {
                    result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
                }
            }
        }
        return result;
    }
    Matrix const operator*= (const Matrix& other) {
        *this = *this * other;
        return *this;
    }
    const Matrix transposed() const {
        Matrix result(this->matrix);
        result.transpose();
        return result;
    }
    const Matrix transpose() const {
        std::vector<std::vector<T>> outer(matrix[0].size());
        for (size_t i = 0; i < matrix[0].size(); i++) {
            for (size_t j = 0; j < matrix.size(); j++) {
                outer[i].push_back(matrix[j][i]);
            }
        }
        matrix.resize(outer.size());
        for (size_t i = 0; i < outer.size(); i++) {
            matrix[i].resize(outer[i].size());
            for (size_t j = 0; j < outer[i].size(); j++) {
                matrix[i][j] = outer[i][j];
            }
        }
        return *this;
    }
};
