// Ten program używa C++17.
#include "dane.h"
#include "kontener.h"

#include <iostream>
#include <iomanip> // std::setw
#include <fstream>
//#include <vector> // Czasowo, dla testowania

std::ostream &operator<<(std::ostream &stream, const osoba &o) {
    return stream << "Osoba("
        << o.rok_studiow() << ", "
        << o.index() << ", "
        << o.imie() << ", "
        << o.nazwisko() << ", "
        << o.email() << ")\n";
}

template <typename T>
std::ostream &operator<<(std::ostream &stream, const list<T> &list) {
    typename list<T>::node_ *runner = list.head_;
    while (runner) {
        stream << runner->value << ", ";
        runner = runner->next;
    }
    return stream << std::endl;
}

void create_files() {
    std::ofstream osoby("osoby.txt");
    osoby << "1;000002;Mateusz;Siborski;WUI.mateusz.siborski@wui.pl;\n";
    osoby << "2;000004;Jan;Pawlak;WUI.jan.pawlak@wui.pl;\n";
    osoby << "3;000006;Pawel;X;WUI.pawel.x@wui.pl;\n";

    std::ofstream oceny("oceny.txt");
    oceny << "4 5 3\n";
    oceny << "4 4 3\n";
    oceny << "3 2 2\n";
}

list<osoba> read_osoby() {
    std::ifstream osoby("osoby.txt");
    std::string line;
    list<osoba> lista_osob;

    auto get_field = [&] {
        size_t semicolon_idx = line.find_first_of(';');
        std::string field = line.substr(0, semicolon_idx);
        line = line.substr(semicolon_idx + 1);
        return field;
    };

    while (std::getline(osoby, line)) {
        int rok = std::stoi(get_field());
        std::string index = get_field();
        std::string imie = get_field();
        std::string nazwisko = get_field();
        std::string mail = get_field();

        // Nie wolno robic tak:
        // osoba(get_field(), get_field(), get_field()),
        // bo ISO C++ nie daje gwarancji, ze funkcje (lambdy) beda
        // wywolane w tym porzadku, który oczekujemy.
        lista_osob.push(
            osoba(
                rok,
                std::move(index),
                std::move(imie),
                std::move(nazwisko),
                std::move(mail)
            )
        );
    }

    return lista_osob;
}

list<list<int>> read_oceny() {
    std::ifstream oceny("oceny.txt");
    std::string line;
    list<list<int>> listy_ocen;

    while (std::getline(oceny, line)) {
        list<int> lista_ocen;

        while (true) {
            size_t digit_idx = line.find_first_of("0123456789");
            if (digit_idx == std::string::npos || digit_idx >= line.size() - 1)
                break;

            line = line.substr(2);

            // To jest nie do końca.
            // Odejmowanie 48 wygląda dziwnie, ale to najprostszy sposób
            // skonwertować ASCII symbol to liczby :)
            lista_ocen.push(int(line.front() - 48));
        }

        listy_ocen.push(std::move(lista_ocen));
    }

    return listy_ocen;
}

float average_mark(list<int> marks) {
    int list_size = marks.size();
    float average;

    while (!marks.is_empty()) {
        average += marks.front();
        marks.pop_front();
    }

    return average / list_size;
}

void write_by_year(
    list<osoba>     &osoby,
    list<list<int>> &oceny
) {
    if (oceny.size() != osoby.size()) {
        throw std::runtime_error("Niezgodność ilości osób a ich ocen.");
    }

    std::ofstream rok_1_file("rok1.txt");
    std::ofstream rok_2_file("rok2.txt");
    std::ofstream rok_3_file("rok3.txt");

    list<osoba> osoby_1_rok;
    list<osoba> osoby_2_rok;
    list<osoba> osoby_3_rok;

    while (!osoby.is_empty() && !oceny.is_empty()) {
        osoba &o = osoby.front();
        float srednia = average_mark(oceny.front());
        o.set_srednia_ocen(srednia);

        osoby.pop_front();
        oceny.pop_front();

        switch (o.rok_studiow()) {
        case 1:
            osoby_1_rok.sorted_push(o);
        case 2:
            osoby_2_rok.sorted_push(o);
        case 3:
            osoby_3_rok.sorted_push(o);
        default:
            break;
        }
    }

    auto write = [](std::ofstream &file, list<osoba> list) {
        while (!list.is_empty()) {
            const osoba &o = list.front();
            file << o.imie() << " " << o.nazwisko() << std::endl;
            file << o.index() << std::endl;
            file << std::setw(2) << o.srednia_ocen() << std::endl;
            file << o.email() << std::endl;
            file << "\n";
            list.pop_front();
        }
    };

    write(rok_1_file, osoby_1_rok);
    write(rok_2_file, osoby_2_rok);
    write(rok_3_file, osoby_3_rok);
    // std::ofstream::close() się wywoła w destruktorze.
    // Dokladnie to piszę, bo to był "jeden z powodow"
    // nie zaliczyc mi egzamin w tym roku przez Pana Cybule.
}

int main() {
    create_files();
    auto osoby = read_osoby();
    auto oceny = read_oceny();

    //std::cout << osoby << std::endl;
    //std::cout << oceny << std::endl;

    write_by_year(osoby, oceny);
}
