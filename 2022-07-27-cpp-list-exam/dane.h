#include <iostream>
#include <string>

class osoba {
public:
    osoba(
        int rok_studiow,
        std::string index,
        std::string imie,
        std::string nazwisko,
        std::string email
    );

    int rok_studiow() const;
    const std::string &index() const;
    const std::string &imie() const;
    const std::string &nazwisko() const;
    const std::string &email() const;
    float srednia_ocen() const;

    void set_srednia_ocen(float);

    // Cos za duzo ich wyszlo...
    bool operator==(const osoba &rhs) const;
    bool operator!=(const osoba &rhs) const;
    bool operator<(const osoba &rhs) const;
    bool operator>(const osoba &rhs) const;
    bool operator>=(const osoba &rhs) const;
    bool operator<=(const osoba &rhs) const;

private:
    void verify_email() const;

    int rok_studiow_;
    std::string index_;
    std::string imie_;
    std::string nazwisko_;
    std::string email_;
    float srednia_ocen_;
};
