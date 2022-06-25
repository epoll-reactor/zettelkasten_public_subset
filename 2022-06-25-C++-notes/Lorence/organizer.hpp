#ifndef ORGANIZER_HPP
#define ORGANIZER_HPP

#include <QMainWindow>
#include <QLabel>
#include <QDateTime>
#include <QTimeZone>
#include <QCalendarWidget>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QFileInfo>

class Organizer : public QMainWindow
{
  Q_OBJECT

public:
  Organizer(QWidget *parent = nullptr);
  virtual ~Organizer();
  void initDate();
};
#endif // ORGANIZER_HPP
