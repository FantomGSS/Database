#include "Column.h"

template<typename T>
Column<T>::Column()
{
	this->name = "";
	this->type = "";
	this->values;
}

template<typename T>
Column<T>::Column(string name, string type, vector<T> values)
{
	this->name = name;
	this->type = type;
	this->values = values;
}

template<typename T>
Column<T>::Column(const Column& column)
{
	this->copy(column);
}

template<typename T>
Column<T>& Column<T>::operator=(const Column& column)
{
	if (this != &column)
	{
		this->copy(column);
	}
	return *this;
}

template<typename T>
void Column<T>::setName(string name)
{
	this->name = name;
}

template<typename T>
void Column<T>::setType(string type)
{
	this->type = type;
}

template<typename T>
void Column<T>::setValues(vector<T>& values)
{
	this->values = values;
}

template<typename T>
const string Column<T>::getName() const
{
	return this->name;
}

template<typename T>
const string Column<T>::getType() const
{
	return this->type;
}

template<typename T>
const vector<T>& Column<T>::getValues() const
{
	return this->values;
}