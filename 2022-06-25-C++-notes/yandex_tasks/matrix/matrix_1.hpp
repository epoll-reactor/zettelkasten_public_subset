// Реализуйте шаблонный класс Matrix<T> для хранения прямоугольной матрицы.
// Тип элементов матрицы должен быть шаблонным параметром. Размеры матрицы
// шаблонными параметрами не являются.

// Класс должен содержать:

// 1. Конструктор от вектора векторов (std::vector<std::vector<T>>), передаваемого по
// константной ссылке (списка строк матрицы). Гарантируется, что строки матрицы не
// пусты и все имеют одинаковый размер.
//
// 2. Оператор std::ostream& operator<<(std::ostream& out, const Matrix<T>& m) для
// матрицы, выводящий элементы матрицы в поток вывода out, передаваемый в оператор.
// При этом элементы внутри одной строки должны быть разделены знаками табуляции, а
// строки — переносами строк. После каждой строки не должно быть символа табуляции и
// в конце не должно быть переноса строки.
//
// 3. Константную функцию-член size без аргументов, возвращающую пару std::pair<size_t,
// size_t> вида (число строк, число столбцов).
//
// В качестве ответа к задаче отправьте только файл с классом и необходимыми заголовками
// #include для вашей задачи.

// Можете считать, что количество элементов в матрице не превосходит 10^6.

#include <iostream>
#include <vector>
template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> matrix;

public:
    Matrix(const std::vector<std::vector<T>>& input) {
        for (size_t i = 0; i < input.size(); ++i) {
            std::vector<T> inner = input[i];
            matrix.push_back(inner);
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
            if (i < m.matrix.size()-1) {
                out << "\n";
            }
        }
        return out;
    }
};
