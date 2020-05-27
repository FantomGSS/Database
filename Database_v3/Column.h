#include <iostream>
#include <vector>
using namespace std;

/**
	Клас Column, който служи за имплементация на колона от стойности с еднакъв тип, като за самата колона той е произволен.
*/
template<typename T>
class Column {
private:
	string name;															///Име на колоната
	string type;															///Тип на колоната
	vector<T> values;														///Вектор от стойностите в колоната

	void copy(const Column& column)											///Функция, която копира стойностите на подадения обект.
	{
		this->name = column.name;
		this->type = column.type;
		this->values = column.values;
	}

public:
	Column();																///Конструктор по подразбиране.
	Column(string, string, vector<T>);										///Конструктор с параметри.
	Column(const Column&);													///Копиращ конструктор.
	Column& operator=(const Column&);										///Предефиниране на оператор за присвояване =

	void setName(string);
	void setType(string);
	void setValues(vector<T>&);
	const string getName() const;
	const string getType() const;
	const vector<T>& getValues() const;
};
