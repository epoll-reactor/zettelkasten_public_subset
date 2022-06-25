// Миша увлекается машинным обучением. Он обучил новейшую нейросетевую модель,
// распознающую лица на фотографии. Модель представляет из себя большой массив
// чисел, равномерно распределенных в диапазоне типа int (это веса отдельных
// нейронов в сети). Мише необходимо уметь сохранять модель на диск и считывать
// с диска. Он даже написал очень простой класс для хранения модели: https://clck.ru/CYyWw.
// Однако простейший тест — заполнить большой массив случайными числами,
// записать его, потом считать и сравнить с тем, что было, — выполняется больше секунды,
// да и файл получается достаточно большим. Поможете Мише ускорить запись и загрузку,
// а заодно и уменьшить размер файла?

// Перепишите функции save и load, чтобы ускорить процесс и по возможности
// уменьшить объём данных. Вы можете написать эти функции как хотите, но требуется,
// чтобы функция load восстанавливала исходные данные по результату, записанному save.
// В частности, load должна очищать данные модели, если они до этого как-то уже были
// заполнены. Мы будем тестировать Вашу программу примерно так: https://clck.ru/CYyUq.

// В проверяющую систему надо сдать весь переписанный класс Model целиком.
#include <iostream>
#include <vector>

class Model {
public:
    std::vector<int> data;

    void save(std::ostream& out) const;
    void load(std::istream& in);
};

void Model::save(std::ostream& out) const {
    auto size = data.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof size);
    out.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof data[0]);
}

void Model::load(std::istream& in) {
    size_t sz = 0;
    in.read(reinterpret_cast<char*>(&sz), sizeof sz);
    data.resize(sz);
    in.read(reinterpret_cast<char*>(data.data()), sz * sizeof data[0]);
}
