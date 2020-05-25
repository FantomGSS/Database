#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class Column {
private:
	string name;
	string type;
	vector<T> values;

	void copy(const Column& column)
	{
		this->name = column.name;
		this->type = column.type;
		this->values = column.values;
	}

public:
	Column();
	Column(string, string, vector<T>);
	Column(const Column&);
	Column& operator=(const Column&);

	void setName(string);
	void setType(string);
	void setValues(vector<T>&);
	const string getName() const;
	const string getType() const;
	const vector<T>& getValues() const;
	void addElement(T);
	void changeElement(int, T);

};
