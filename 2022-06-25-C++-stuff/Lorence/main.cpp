#include "organizer.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Organizer w;
  w.resize(800, 600);
  w.show();
  return a.exec();
}
