#include <iostream>
#include <vector>
using namespace std;

class Entry {
private:
	string name;
	string path;

	void copy(const Entry& entry)
	{
		this->name = entry.name;
		this->path = entry.path;
	}

public:
	Entry();
	Entry(string, string);
	Entry(const Entry&);
	Entry& operator=(const Entry&);

	void setName(string);
	void setPath(string);
	const string getName() const;
	const string getPath() const;

	friend ostream& operator<<(ostream& output, const Entry& entry);
};