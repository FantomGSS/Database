#include "Entry.h"

/**
	Конструктор по подразбиране за класа Entry.
	Името на табалицата и пътя на файла ги приравняваме на празни символни низове.
*/
Entry::Entry()
{
	this->name = "";
	this->path = "";
}

/**
	Конструктор с параметри за класа Entry.
	По подадени име на таблица и път на файла.
*/
Entry::Entry(string name, string path)
{
	this->name = name;
	this->path = path;
}

/**
	Копиращ конструктор за класа Entry.
	Прилагаме функцията copy върху обекта.
*/
Entry::Entry(const Entry& entry)
{
	this->copy(entry);
}

/**
	Предефиниране на оператор за присвояване =
	@param[in] entry е обектът, който ще присвояваме.
	@return Връща текущия обект, върху който е приложен оператора =
*/
Entry& Entry::operator=(const Entry& entry)
{
	if (this != &entry)
	{
		this->copy(entry);
	}
	return *this;
}

void Entry::setName(string name)
{
	this->name = name;
}

void Entry::setPath(string path)
{
	this->path = path;
}

const string Entry::getName() const
{
	return this->name;
}

const string Entry::getPath() const
{
	return this->path;
}

/**
	Предефиниране на оператора << за стандартния изходен поток.
	@param[in, out] output е потокът, върху който е приложен <<
	@param[in] entry е обектът, който седи от дясно на оператора.
	@return Връща потока подаден на функцията.
*/
ostream& operator<<(ostream& output, const Entry& entry)
{
	output << entry.name << " | " << entry.path << endl;
	return output;
}
