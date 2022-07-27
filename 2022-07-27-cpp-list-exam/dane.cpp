#include "dane.h"

osoba::osoba(
    int rok_studiow,
    std::string index,
    std::string imie,
    std::string nazwisko,
    std::string email
) : rok_studiow_(rok_studiow)
  , index_(std::move(index))
  , imie_(std::move(imie))
  , nazwisko_(std::move(nazwisko))
  , email_(std::move(email))
  , srednia_ocen_(0) {
    verify_email();
}

void osoba::verify_email() const {
    std::string email_prefix  = email_.substr(0, 3);
    std::string email_postfix = email_.substr(email_.size() - 7/* len(@wui.pl) */);
    if (email_prefix != "WUI" || email_postfix != "@wui.pl") {
        throw std::invalid_argument("Podany z�y email adres.");
    }
    // Nie wystarczylo czasu dokonczyc walidacji, ale rzecz jasna :(
}

int                osoba::rok_studiow()  const { return rok_studiow_; }
const std::string &osoba::index()        const { return index_; }
const std::string &osoba::imie()         const { return imie_; }
const std::string &osoba::nazwisko()     const { return nazwisko_; }
const std::string &osoba::email()        const { return email_; }
float              osoba::srednia_ocen() const { return srednia_ocen_; }

void osoba::set_srednia_ocen(float srednia) {
    srednia_ocen_ = srednia;
}

bool osoba::operator==(const osoba &rhs) const {
    return
        rok_studiow() == rhs.rok_studiow() &&
        index() == rhs.index() &&
        imie() == rhs.imie() &&
        nazwisko() == rhs.nazwisko() &&
        email() == rhs.email();
}

bool osoba::operator!=(const osoba &rhs) const {
    return !(*this == rhs);
}

bool osoba::operator<(const osoba &rhs) const {
    if (imie() != rhs.imie()) {
        return imie() < rhs.imie();
    }

    if (nazwisko() != rhs.nazwisko()) {
        return nazwisko() < rhs.nazwisko();
    }

    return index() < rhs.index();
}

bool osoba::operator>(const osoba &rhs) const {
    return !(*this < rhs);
}

bool osoba::operator>=(const osoba &rhs) const {
    if (*this == rhs) {
        return true;
    }
    return *this > rhs;
}

bool osoba::operator<=(const osoba &rhs) const {
    if (*this == rhs) {
        return true;
    }
    return *this < rhs;
}
