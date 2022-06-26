#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main()
{
	using Date = std::string;
	vector<Date> e;
	copy(
		// Использует operator>>(Date).
		istream_iterator<Date>(cin),
		istream_iterator<Date>(),
		back_inserter(e)
	);
	vector<Date>::iterator first =
		find(e.begin(), e.end(), "01/01/95");
	vector<Date>::iterator last =
		find(e.begin(), e.end(), "12/31/95");
	// [Исправление]
	// last может быть равен e.end() (второй аргумент
	// std::find), что делает невозможным корректное
	// присваивание.
	// @{
	if (last == e.end())
		return -1;
	// @}
	*last = "12/30/95";
	// [Исправление]
	// Не гарантируется, что first < last, чего
	// требует std::copy для корректного выполнения.
	// @{
	if (first > last)
		return -1;
	// @}
	copy(
		first,
		last,
		ostream_iterator<Date>(cout, "\n")
	);
	// [Исходный код]
	// e.insert(--e.end(), "26/06/22");
	//
	// [Исправление]
	// Вместо --e.end(), можно записать e.end() - 1.
	// Также, e может быть пустым, и вызов --e.end()
	// приведёт к непредсказуемым последствиям.
	// @{
	if (e.begin() == e.end())
		return -1;
	e.insert(e.end() - 1, "26/06/22");
	// @}
	//
	// [Исходный код]
	// copy(
	// 	first,
	// 	last,
	// 	ostream_iterator<Date>(cout, "\n")
	// );
	// [Исправление]
	// Имея ввиду специфику работы std::vector, first
	// и last могут оказаться "устаревшими", не указывать
	// на реальные данные после релокации памяти.
	// Вместо этого нужно использовать актуальные итераторы.
	// @{
	copy(
		e.begin(),
		e.end(),
		ostream_iterator<Date>(cout, "\n")
	);
	// @}
}
