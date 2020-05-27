#include <iostream>
#include <vector>
using namespace std;

/**
	Клас Entry, който служи за имплементация на двойката име на таблица и пътя на файла с тази таблица.
*/
class Entry {
private:
	string name;																///Име на таблицата
	string path;																///Път на файла, където се пази таблицата

	void copy(const Entry& entry)												///Функция, която копира стойностите на подадения обект.
	{
		this->name = entry.name;
		this->path = entry.path;
	}

public:
	Entry();																	///Конструктор по подразбиране.
	Entry(string, string);														///Конструктор с параметри.
	Entry(const Entry&);														///Копиращ конструктор.
	Entry& operator=(const Entry&);												///Предефиниране на оператор за присвояване =

	void setName(string);
	void setPath(string);
	const string getName() const;
	const string getPath() const;

	friend ostream& operator<<(ostream& output, const Entry& entry);			///Предефиниране на оператор за вмъкване <<
};