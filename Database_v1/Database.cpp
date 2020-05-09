#include <iostream>
#include "Column.h"
using namespace std;

int main()
{
	Column<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);
	list.push_front(20);
	list.push(50, 2);
	//list.print();

	list.pop_back();
	list.pop_front();
	//list.print();

	list.pop_back();
	list.pop_front();
	//list.print();

	list.pop_front();
	list.pop_front();
	list.pop_front();
	list.pop_back();
	//list.print();

	Column<int> column({1, 2, 3, 4, 5, 6}, "int");
	column.print();
	cout << endl;

	Column<double> column2({ 1.0, 2.2324, 3.994, 4.2452454, 5.12121, 6.6464 }, "double");
	column2.print();
	cout << endl;

	Column<string> column3({ "asdasda", "wefwfw"}, "string");
	column3.print();
}