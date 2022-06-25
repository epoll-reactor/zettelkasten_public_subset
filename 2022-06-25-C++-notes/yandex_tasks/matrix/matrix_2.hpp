// Добавьте в предыдущий класс следующие методы:

// 1. operator +=, принимающий константную ссылку на вторую матрицу того
// же типа и размера. Заметьте, что оператор должен вернуть ссылку на класс.
//
// 2. Константный operator +, принимающий константную ссылку на вторую матрицу
// того же типа и размера и возвращающий сумму матриц.
//
// 3. Шаблонный operator *=, принимающий константную ссылку на число (не
// обязательно типа T) и умножающий матрицу на этот скаляр. Заметьте,
// что оператор должен вернуть ссылку на класс.
//
// 4. Шаблонный константный operator *, принимающий константную ссылку на число
// (не обязательно типа T) и возвращающий матрицу, умноженную на этот скаляр.
//
// Убедитесь также, что в программе присутствуют операторы присваивания и конструктор
// копирования (при корректной реализации задачи хватит стандартных, которые
// автоматически генерирует компилятор), а также оператор вывода из предыдущей задачи.

#include <iostream>
#include <vector>
template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> matrix;

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
};
