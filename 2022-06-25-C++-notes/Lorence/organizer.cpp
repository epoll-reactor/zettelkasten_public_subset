#include "organizer.hpp"

Organizer::Organizer(QWidget* parent)
  : QMainWindow(parent)
{
  initDate();
}

void Organizer::initDate()
{
  QDate* date = new QDate(QDate::currentDate().year(),
                          QDate::currentDate().month(),
                          QDate::currentDate().day());
  QLabel* lbl = new QLabel(this);

  lbl->setGeometry(40, 15, 225, 40);
  lbl->setText("Дата: " + date->toString());
  lbl->show();

  QPushButton* calendarButton = new QPushButton(this);
  calendarButton->setGeometry(225 + 40, 15, 40, 40);
  calendarButton->show();
  connect(calendarButton, &QPushButton::pressed, [](){
    QCalendarWidget* wid = new QCalendarWidget();
    wid->show();
  });
}

Organizer::~Organizer()
{

}
