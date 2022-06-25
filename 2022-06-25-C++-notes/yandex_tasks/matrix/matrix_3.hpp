// В класс Matrix внесите следующие изменения:

// 1. Реализуйте функцию-член transpose, транспонирующую матрицу и возвращающую
// ссылку на класс (функция должна модифицировать текущий экземпляр матрицы).
//
// 2. Реализуйте константную функцию-член transposed, возвращающую новую
// транспонированную матрицу.
//
// Убедитесь, что в классе присутствует оператор вывода из первой задачи про матрицу.

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
    const std::pair<size_t, size_t> size() const {
        return std::make_pair(matrix.size(), matrix[0].size());
    }
    friend std::ostream& operator<<(std::ostream& out, const Matrix<T>& m) {
        for (size_t i = 0; i < m.matrix.size(); ++i) {
            out << m.matrix[i][0];
            for (size_t j = 1; j < m.matrix[i].size(); ++j) {
                out << "\t" << m.matrix[i][j];
            }
            if (i < m.matrix.size()-1) {
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
    Matrix const operator* (const T &scalar) const {
        Matrix result(this->matrix);
        for (size_t i = 0; i < result.matrix.size(); ++i) {
            for (size_t j = 0; j < result.matrix[i].size(); ++j) {
                result.matrix[i][j] *= scalar;
            }
        }
        return result;
    }
    Matrix const operator*= (const T &scalar) {
        *this = *this * scalar;
        return *this;
    }
    const Matrix transposed() const {
        Matrix result(this->matrix);
        result.transpose();
        return result;
    }
    const Matrix transpose() const{
        std::vector<std::vector<T>> outer(matrix[0].size());
        for (size_t i = 0; i < matrix[0].size(); i++) {
            for (size_t j = 0; j < matrix.size(); j++)  {
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
