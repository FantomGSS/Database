#include "Entry.h"

Entry::Entry()
{
	this->name = "";
	this->path = "";
}

Entry::Entry(string name, string path)
{
	this->name = name;
	this->path = path;
}

Entry::Entry(const Entry& entry)
{
	this->copy(entry);
}

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

ostream& operator<<(ostream& output, const Entry& entry)
{
	output << entry.name << " | " << entry.path << endl;
	return output;
}
