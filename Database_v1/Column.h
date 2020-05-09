#include <iostream>
#include <vector>
using namespace std;

template <typename T>
struct Cell
{
	T value;
	Cell <T>* nextValue;

	Cell(const T& value, Cell <T>* nextValue)
	{
		this->value = value;
		this->nextValue = nextValue;
	}
};

template<typename T>
class Column {
private:
	Cell<T>* first;
	Cell<T>* last;
	int size;
	string type;

	void copy(const Column& other)
	{
		Cell<T>* temp = other.first;
		for (int i = 0; i < other.size; i++)
		{
			push_back(temp->value);
			temp = temp->nextValue;
		}
		this->type = other.type;
	}

	void erase()
	{
		while (size != 0)
		{
			this->pop_front();
		}
	}

public:
	Column()
	{
		this->first = nullptr;
		this->last = nullptr;
		this->size = 0;
		this->type = "";
	}

	Column(vector<T> values, string type)
	{
		Column column;
		
		for (int i = 0; i < values.size(); i++)
		{
			column.push_back(values[i]);
		}
		column.setType(type);
		this->copy(column);
		
	}

	Column(const Column& other)
	{
		this->copy(other);
	}

	Column& operator=(const Column& other)
	{
		if (this != &other)
		{
			this->erase();
			this->copy(other);
		}
		return *this;
	}

	~Column()
	{
		this->erase();
	}

	void setType(string type)
	{
		this->type = type;
	}

	const string getType() const
	{
		return this->type;
	}

	void push_back(T value)
	{
		Cell<T>* temp = new Cell<T>(value, nullptr);
		if (this->size == 0)
		{
			this->first = temp;
			this->last = temp;
		}
		else
		{
			this->last->nextValue = temp;
			this->last = temp;
		}
		this->size++;
	}

	void push_front(T value)
	{
		Cell<T>* temp = new Cell<T>(value, this->first);
		this->first = temp;
		this->size++;
	}

	void pop_back()
	{
		if (size == 0) return;
		Cell<T>* buffer = last;
		Cell<T>* temp = first;
		while (temp->nextValue != this->last)
		{
			temp = temp->nextValue;
		}

		temp->nextValue = nullptr;
		this->last = temp;
		delete buffer;
		this->size--;
	}

	void pop_front()
	{
		if (size == 0) return;
		Cell<T>* temp = first;
		if (size == 1)
		{
			this->first = nullptr;
			this->last = nullptr;
			delete temp;
			size--;
		}

		else
		{
			first = first->nextValue;
			delete temp;
			size--;
		}
	}

	void push(T value, int index)
	{
		Cell<T>* buffer = first;
		Cell<T>* temp = new Cell<T>(value, nullptr);
		while ((index - 1) != 0)
		{
			buffer = buffer->nextValue;
			index--;
		}
		Cell<T>* save = buffer->nextValue;
		buffer->nextValue = temp;
		temp->nextValue = save;
		size++;
	}

	T get(int index)
	{
		Cell<T>* temp = first;
		for (int i = 0; i < index; i++)
		{
			temp = temp->nextValue;
		}
		return temp->value;
	}

	void changeValue(int index, T newValue)
	{
		Cell<T>* temp = first;
		for (int i = 0; i < index; i++)
		{
			temp = temp->nextValue;
		}
		temp->value = newValue;
	}

	void print()
	{
		Cell<T>* temp = first;
		if (size != 0)
		{
			while (temp->nextValue != nullptr)
			{
				cout << temp->value << " ";
				temp = temp->nextValue;
			}
			cout << temp->value;
		}
	}
};