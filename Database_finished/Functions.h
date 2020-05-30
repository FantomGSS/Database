#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> 
#include <algorithm>
#include <numeric>
#include "Entry.h"

const char SEPARATOR_TABLE_FILES = '|';                                                                 ///������������ ��� ���������, �������������� ���������, � ������������ �����.
const char DISTANCE_BETWEEN_COLUMNS[7] = "      ";                                                      ///������������ ����� ��������.
const int TABLE_PRINTING_LIMITATION = 3;                                                                ///����� ���� �� ��������� �� �� ���������
                                                                                                        ///� ���������� �� ����� �� �������� � �������� ��������.

const int DECIMAL_PLACES = 3;                                                                           ///��������� ����� ���� ����������� �������, ��� �������� �� ��������� �� ��������� ������.
const string CATALOG_PATH = "Catalog.txt";                                                              ///�����, � ����� �� �� �������� �������� - ��� �� ������� � ���� ��� ���������.
const string ANSWERS[2] = { "No", "Yes" };                                                              ///����������� �������� ��� ������� ������.
const string TYPES[3] = { "int", "double", "string" };                                                  ///����������� ������, � ����� ������ ����� ���� �� ������.
const string INVALID_COLUMN_INDEX = "Invalid column index for the given table";                         ///������ ��� ������� ��������� ������ �� ������.
const string INVALID_COLUMN_ENTRY = "Invalid column entry! The column index must be an integer.";       ///��������� ���� �� ������ �� ������.
const string EMPTY_FILE = "There is no table in this file yet!";                                        ///��� �����, ������������� ������� � ������, �� ������� ���� ���������.
const string TABLE_DOES_NOT_EXIST = "This table name does not exist in the database!";                  ///��� ��������� �� ���������� � ������ �����, �� ������ ���� ���������.
const string TABLE_IS_NOT_SAVED = "To work with this table, you must save the changes in the catalog!"; ///��� ��������� � �������� � �������, �� �� � �������� � ������ ����� (��������).
const string ERROR_FORGOTTEN_PARAMETERS = "You forgot to enter the required number of parameters!";     ///��������� �� ������, ������ �� �� ������� ������ ���� ���������, ��������� �� ���������.

/**
    ������� ������ ������� �� ���������, ��������� ��� ������� ��������� �������� ��� ��� ������, ������ �� ������ � �������.
*/
void printErrorMessage(string filePath) {
    cerr << "\n";
    cerr << "There was a problem opening file with name: " << filePath << endl;
    cerr << "\n";
}

void printReminderMessage() {
    cerr << "\n";
    cerr << "A file with the given name does not exist! Please check your path." << endl;
    cerr << "\n";
}

void printSuccessSaveMessage(string filePath) {
    cout << "===========================================================" << endl;
    cout << "All changes are saved successfully in the file " << filePath << "!" << endl;
    cout << "===========================================================" << endl;
}

void printSuccessCreateMessage(string filePath) {
    cout << "===========================================================" << endl;
    cout << "Successfully created " << filePath << "!" << endl;
    cout << "===========================================================" << endl;
}

/**
    �������, ����� ������� ����������� ��� ����������� ������� �� �����.
*/
void answering(string& answer)
{
    while (1)
    {
        cout << "Answer: ";
        getline(cin, answer);
        if (answer.size() > 0)
        {
            if (find(begin(ANSWERS), end(ANSWERS), answer) != end(ANSWERS))
            {
                break;
            }
            else
            {
                cout << "Allowed answers are Yes or No." << endl;
            }
        }
        else
        {
            cout << "You have not entered a answer!" << endl;
        }
    }
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����)
    ��������� ���� ��� � �������.
*/
bool checkFilePath(string filePath)
{
    bool validFilePath = true;
    if (filePath.find(">") != -1 || filePath.find("<") != -1
        || filePath.find("/") != -1 || filePath.find("|") != -1)
    {
        cout << "You have entered an invalid path! Forbidden characters are: / < | >" << endl;
        validFilePath = false;
    }

    return validFilePath;
}

/**
    �������, ����� ��������� �� ������ ��� �� �������, ���� �� � �������� � �������.
*/
bool checkIfTableExists(vector<Entry> entries, string tableName)
{
    for (int i = 0; i < entries.size(); i++)
    {
        if (entries[i].getName() == tableName)
        {
            return true;
        }
    }

    return false;
}

/**
    �������, ����� ��������� ��������� ��� ���� � ���� ������������ �� ������ �����.
*/
bool checkType(string type)
{
    if (find(begin(TYPES), end(TYPES), type) != end(TYPES))
    {
        return true;
    }
    return false;
}

/**
    �������, ����� �� ������ ��� �� �������, ������� �� ������ ����� ���� �� ����� �� ���� �������.
*/
string findPathOfTable(vector<Entry>& catalog, string tableName)
{
    string filePath;
    for (int i = 0; i < catalog.size() && filePath.empty(); i++)
    {
        if (catalog[i].getName() == tableName)
        {
            filePath = catalog[i].getPath();
        }
    }

    return filePath;
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����)
    ������ ���� ��� ��� �� ���������� ����� - ������� �� ���� ����� � �������� ��� � ����������,
    ��� �������, �� �� ������ �� ����� ������������� ��������. ��� � ������������� �������,
    �� ������ ��������� ���������, �� ����� ���� ���� � ������ �� �� ������� ����.
    �� ��� ���������� �����, ������� ������������ �� � ����� �������� �� ����� ��� ������ lines.
*/
void getLinesFromFile(string filePath, vector<string>& lines, string condition)
{
    string line;
    int lineCounter = 0;
    ifstream inputFileStream;
    inputFileStream.open(filePath, ios::in);

    if (inputFileStream.is_open())
    {
        while (getline(inputFileStream, line))
        {
            lines.push_back(line);
        }
        inputFileStream.close();
    }
    else if (!inputFileStream.good() && condition == "Catalog")
    {
        ofstream outputFileStream;
        outputFileStream.open(filePath, ios::out);
        outputFileStream.close();
        printSuccessCreateMessage(filePath);
    }
    else if (condition == "Table")
    {
        printReminderMessage();
    }
    else
    {
        printErrorMessage(filePath);
    }
}

/**
    �������, ����� ������� ����� �������� ��� ������ ��������� ����������.
    � ����� ������ �� ������, ����� �� ���� ��������� ���� ����.
*/
vector<string> splitLine(string line, char separator)
{
    vector<string> words;
    string word;
    istringstream tokenStream(line);
    while (getline(tokenStream, word, separator))
    {
        words.push_back(word);
    }
    return words;
}

/**
    �������, ����� ������� ������ �� �������� ������ �� ����,
    � ������� �� ������� ������� (splitLine) � ����� ������ �� ���� ����.
*/
vector<string> splitLines(vector<string>& lines)
{
    vector<string> words;
    for (int i = 0; i < lines.size(); i++)
    {
        vector<string> splittedLine = splitLine(lines[i], SEPARATOR_TABLE_FILES);

        for (int k = 0; k < splittedLine.size(); k++)
        {
            words.push_back(splittedLine[k]);
        }
    }

    return words;
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����)
    ��������� ���� ���������� ����� ����.
*/
bool fileExists(string filePath)
{
    bool fileExists = false;
    ifstream inputFileStream;
    inputFileStream.open(filePath, ios::in);

    if (inputFileStream.good())
    {
        fileExists = true;
        inputFileStream.close();
    }

    return fileExists;
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����)
    ������ �����, ������������� ��������, � ������� �� ���� �������� - ��� �� ������� � ���� �� �����.
    ����������� ����� ������������ �� �������� � " | ", ��� ���� ���� ������ �� ���� �� �� �������
    � ����� �� ���� � ������������ ����� �� ������ �� ������� �� ���������� ������� �� ����� ���������.
*/
void loadCatalog(string filePath, vector<Entry>& catalog)
{
    vector<string> lines;
    getLinesFromFile(filePath, lines, "Catalog");
    int indexSeparator = 0;
    string separator = " | ";
    for (int i = 0; i < lines.size(); i++)
    {
        indexSeparator = lines[i].find(separator);
        if (indexSeparator != -1)
        {
            Entry entry = Entry(lines[i].substr(0, indexSeparator), lines[i].substr(indexSeparator + separator.size()));
            catalog.push_back(entry);
        }
    }
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����)
    ������ �����, ������������� ��������, � �� ������� ������ �� ������ (��� �� ������� � ��� �� �����),
    ����� ���� ������ ��� �����.
*/
void completeCatalogFile(string filePath, vector<Entry>& entries)
{
    ofstream outputFileStream;
    outputFileStream.open(filePath, ios::app);

    if (outputFileStream.is_open())
    {
        for (int i = 0; i < entries.size(); i++)
        {
            outputFileStream << entries[i];
        }
        outputFileStream.close();
        printSuccessSaveMessage(filePath);
    }
    else
    {
        printErrorMessage(filePath);
    }
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����,
    ������������� �������) ������� ������, �������� �� �� ����� �� ��������� � ���� �� �����.
*/
void importCommand(string filePath, vector<Entry>& entries, vector<Entry>& catalog)
{
    string tableName;
    string path;
    if (!fileExists(filePath))
    {
        printReminderMessage();
        return;
    }
    else
    {
        int indexLastSlash = filePath.find_last_of("\\");
        int indexPoint = 0;
        if (indexLastSlash == -1)
        {
            indexPoint = filePath.find_last_of(".");
            if (indexPoint == -1)
            {
                tableName = filePath;
            }
            else
            {
                tableName = filePath.substr(0, indexPoint);
            }
        }
        else
        {
            tableName = filePath.substr(indexLastSlash + 1);
            indexPoint = tableName.find_last_of(".");
            if (indexPoint != -1)
            {
                tableName = tableName.substr(0, indexPoint);
            }
        }
    }

    tableName[0] = toupper(tableName[0]);

    if (checkIfTableExists(catalog, tableName))
    {
        cout << "You cannot load a table with a name that already exists in the database!" << endl;
    }
    else
    {
        Entry entry = Entry(tableName, filePath);
        entries.push_back(entry);

        cout << "The table has been successfully added to the catalog!" << endl;
    }
}

/**
    �������, ����� ������� ���������� ������� � �������, ���� �� ��� �������,
    �� ������ ������� ������� �� �� �������� � ��������. �� �� �� ������ � ��������,
    ������ �� �� ������� ���������� save, ����� �� �� ������� � ����.
*/
void showTables(vector<Entry>& catalog)
{
    if (catalog.size() == 0)
    {
        cout << "There are no loaded tables in the database!" << endl;
        return;
    }

    for (int i = 0; i < catalog.size(); i++)
    {
        if (i == catalog.size() - 1)
        {
            cout << catalog[i].getName() << endl;
        }
        else
        {
            cout << catalog[i].getName() << ", ";
        }
    }
}

/**
    �������, ����� ������� ���������� �� �������� �� ��������
    �� �������� ��� �� �������.
*/
void describe(vector<Entry>& catalog, string tableName)
{
    vector<string> lines;
    string filePath = findPathOfTable(catalog, tableName);

    getLinesFromFile(filePath, lines, "Table");
    vector<string> cells = splitLines(lines);

    if (lines.size() == 0)
    {
        cout << EMPTY_FILE << endl;
        return;
    }

    int countColumns = lines.size();
    int countRows = cells.size() / countColumns;

    for (int i = 1; i < cells.size(); i += countRows)
    {
        if (i == cells.size() - countRows + 1)
        {
            cout << cells[i] << endl;
        }
        else
        {
            cout << cells[i] << ", ";
        }
    }
}

/**
    �������, ����� �������� ��������� �� ���������� ����.
*/
bool compareWordSizes(string wordOne, string wordTwo)
{
    return wordOne.size() > wordTwo.size();
}

/**
    �������, ����� ������ ��� � ���-������� ���� ��� ����� ������ �� ���������
    � ����� ������ ������ � �� ����� ��� �������.
*/
vector<int> getSizesOfLongestWordsInColumns(vector<string> cells, int countColumns, int countRows)
{
    vector<int> sizes;
    for (int i = 0; i < countColumns; i++)
    {
        vector<string> wordsColumn;
        for (int k = i * countRows; k < countRows * (i + 1); k++)
        {
            wordsColumn.push_back(cells[k]);
        }

        sort(wordsColumn.begin(), wordsColumn.end(), compareWordSizes);
        int size = wordsColumn[0].size();
        sizes.push_back(size);
    }
    return sizes;
}

/**
    �������, ����� ���� �� ����� �������� ��� � �� �����, ���� �������,
    �� ���� �������� ���, ����� �� ��������� �� �����������, �� ���.
*/
string readOperationFromInput(int row, int countRows)
{
    string operation;
    while (1)
    {
        cout << "Operation: ";
        getline(cin, operation);
        if (operation.size() > 0)
        {
            if (operation == "exit" || operation == "next page")
            {
                break;
            }
            else if (operation == "previous page")
            {
                if (row == 2 + TABLE_PRINTING_LIMITATION)
                {
                    cout << "There is no previous page yet, because you are still at the beginning of the review!" << endl;
                }
                else
                {
                    break;
                }
            }
            else
            {
                cout << "You have entered an invalid operation!" << endl;
            }
        }
        else
        {
            cout << "You have not entered a operation!" << endl;
        }
    }

    return operation;
}

/**
    �������, ����� ��������� ��� ������������� �� ������� ���������� �� �������, �� �� ���� �� �� ���������,
    ���� �� ������� ���������, ��������� � ��������, ������ � �� ���� ����� �� ������ � �������� ��������, � �����.
*/
void tablePresentationSelect(vector<string>& cells, vector<int>& sizesLongesWords, int size, int counter, vector<int> positionsValue, int countRows, int& counterRows)
{
    if ((counterRows - 2) % TABLE_PRINTING_LIMITATION == 0 && counterRows != 2)
    {
        string operation = readOperationFromInput(counterRows, countRows);
        if (operation == "previous page")
        {
            counterRows -= TABLE_PRINTING_LIMITATION * 2;
        }
        else if (operation == "exit")
        {
            return;
        }
    }

    for (int index = positionsValue[counterRows]; index < cells.size(); index += countRows)
    {
        size = sizesLongesWords[counter++];
        cout << cells[index];
        for (int i = 0; i < size - cells[index].size(); i++)
        {
            cout << " ";
        }
        cout << DISTANCE_BETWEEN_COLUMNS;
    }
    counter = 0;
    cout << endl;
}

/**
    �������, ����� ��������� ��� ������������� �� ���������� �� �������, �� �� ���� �� �� ���������,
    ���� �� ������� ���������, ��������� � ��������, ������ � �� ���� ����� �� ������ � �������� ��������, � �����.
*/
void tablePresentationPrint(vector<string>& cells, vector<int>& sizesLongesWords, int size, int counter, int& row, int countRows)
{
    if ((row - 2) % TABLE_PRINTING_LIMITATION == 0 && row != 2)
    {
        string operation = readOperationFromInput(row, countRows);
        if (operation == "previous page")
        {
            row -= TABLE_PRINTING_LIMITATION * 2;
        }
        else if (operation == "exit")
        {
            return;
        }
    }

    for (int index = row; index < cells.size(); index += countRows)
    {
        size = sizesLongesWords[counter++];
        cout << cells[index];
        for (int i = 0; i < size - cells[index].size(); i++)
        {
            cout << " ";
        }
        cout << DISTANCE_BETWEEN_COLUMNS;
    }
    counter = 0;
    cout << endl;
}

/**
    �������, ����� �� ������ ��� �� �������, �������� ������� ����������.
*/
void print(vector<Entry>& catalog, string tableName)
{
    vector<string> lines;
    string filePath = findPathOfTable(catalog, tableName);

    getLinesFromFile(filePath, lines, "Table");
    vector<string> cells = splitLines(lines);

    if (lines.size() == 0)
    {
        cout << EMPTY_FILE << endl;
        return;
    }

    int countColumns = lines.size();
    int countRows = cells.size() / countColumns;
    vector<int> sizesLongesWords = getSizesOfLongestWordsInColumns(cells, countColumns, countRows);

    cout << "The printing of a table is limited to three lines" << endl;
    cout << "The possible viewing options are: next page, previous page and exit." << endl;
    int counter = 0;
    int size = 0;
    for (int row = 0; row < countRows; ++row)
    {
        tablePresentationPrint(cells, sizesLongesWords, size, counter, row, countRows);
    }
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����),
    ������� ����� ���� � �� ����� ��� ���������� ������ �� ��������� ������.
*/
void createFile(string filePath, vector<string>& lines)
{
    ofstream outputFileStream;
    outputFileStream.open(filePath, ios::out);
    if (outputFileStream.is_open())
    {
        for (int i = 0; i < lines.size(); i++)
        {
            outputFileStream << lines[i] << endl;
        }
        outputFileStream.close();
        printSuccessCreateMessage(filePath);
    }
    else
    {
        printErrorMessage(filePath);
    }
}

/**
    �������, ����� ������ ������������ �� ������ ������� � ���� ����, ���� ����� ���� �� �������
    � ������� �� ������� ������� (createFile).
*/
void exportTable(vector<Entry>& catalog, string tableName, string filePath)
{
    if (fileExists(filePath))
    {
        cout << "There is already a file with that name!" << endl;
    }
    else
    {
        string pathFromDB = findPathOfTable(catalog, tableName);
        vector<string> lines;
        getLinesFromFile(pathFromDB, lines, "Table");
        createFile(filePath, lines);
    }
}

/**
    �������, ����� �������� ���� ���������� �������� � �� ����, ����� �� ������� �� ��������.
*/
bool typeOfValueIsSuitable(string line, string targetValue, string errorMessage)
{
    bool typeOfValueIsSuitable = true;
    int indexFirstVerticalBar = line.find_first_of(SEPARATOR_TABLE_FILES);
    line = line.substr(indexFirstVerticalBar + 1);
    int indexSecondVerticalBar = line.find_first_of(SEPARATOR_TABLE_FILES);
    string type = line.substr(0, indexSecondVerticalBar);
    if (type == TYPES[0])
    {
        try
        {
            stoi(targetValue);
        }
        catch (...)
        {
            cout << errorMessage << endl;
            typeOfValueIsSuitable = false;
        }
    }
    else if (type == TYPES[1])
    {
        try
        {
            stod(targetValue);
        }
        catch (...)
        {
            cout << errorMessage << endl;
            typeOfValueIsSuitable = false;
        }
    }

    return typeOfValueIsSuitable;
}

/**
    �������, ����� ������� ������ ������ �� ���������, ����� �������� ���������� �value�,
    ����� �� ����� � �������� � ������� ����� columnN.
*/
void select(vector<Entry>& catalog, int columnN, string value, string tableName)
{
    vector<string> lines;
    string filePath = findPathOfTable(catalog, tableName);

    getLinesFromFile(filePath, lines, "Table");
    vector<string> cells = splitLines(lines);

    if (lines.size() == 0)
    {
        cout << EMPTY_FILE << endl;
        return;
    }

    int countColumns = lines.size();
    int countRows = cells.size() / countColumns;
    vector<int> sizesLongesWords = getSizesOfLongestWordsInColumns(cells, countColumns, countRows);

    vector<int> positionsValue;
    positionsValue.push_back(0);
    positionsValue.push_back(1);
    if (columnN - 1 >= 0 && columnN - 1 < lines.size())
    {
        if (value != "NULL")
        {
            if (!typeOfValueIsSuitable(lines[columnN - 1], value, "The value you are looking for in the column is not of its type!"))
            {
                return;
            }
        }

        vector<string> values = splitLine(lines[columnN - 1], SEPARATOR_TABLE_FILES);
        for (int i = 0; i < values.size(); i++)
        {
            if (values[i] == value)
            {
                positionsValue.push_back(i);
            }
        }

        if (positionsValue.size() == 2)
        {
            cout << "This value does not appear in the column you are looking for." << endl;
        }
        else
        {
            cout << "The printing of the rows from your selection is limited to three lines" << endl;
            cout << "The possible viewing options are: next page, previous page and exit." << endl;
            int counter = 0;
            int size = 0;
            for (int counterRows = 0; counterRows < positionsValue.size(); ++counterRows)
            {
                tablePresentationSelect(cells, sizesLongesWords, size, counter, positionsValue, countRows, counterRows);
            }
        }
    }
    else
    {
        cout << INVALID_COLUMN_INDEX << endl;
    }
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����),
    ������ ����� � �� ������� � ���, �������� �� ������� �� �������� ������.
*/
void appendNewColumnToTableFile(string filePath, vector<string>& newColumn)
{
    ofstream outputFileStream;
    outputFileStream.open(filePath, ios::app);

    if (outputFileStream.is_open())
    {
        for (int i = 0; i < newColumn.size(); i++)
        {
            outputFileStream << newColumn[i];
            if (i != newColumn.size() - 1)
            {
                outputFileStream << SEPARATOR_TABLE_FILES;
            }
            else
            {
                outputFileStream << endl;
            }
        }
        outputFileStream.close();
    }
    else
    {
        printErrorMessage(filePath);
    }
}

/**
    �������, ����� ��������� ���� ����� �� ��������, ����� ������ �� �� ������, � ��������.
*/
void checkColumnName(bool& columnNameIsUnique, vector<string> lines, string columnName)
{
    for (int i = 0; i < lines.size(); i++)
    {
        string name;
        int indexFirstVerticalBar = lines[i].find_first_of(SEPARATOR_TABLE_FILES);
        name = lines[i].substr(0, indexFirstVerticalBar);

        if (columnName == name)
        {
            columnNameIsUnique = false;
            break;
        }
    }
}

/**
    �������, ����� ������ ���� ������ � ������� � ������� �� ������� ������� (appendNewColumnToTableFile).
*/
void addColumn(vector<Entry>& catalog, string tableName, string columnName, string columnType)
{
    if (!checkType(columnType))
    {
        cout << "The type you entered is not supported by the database!" << endl;
    }
    else
    {
        string filePath = findPathOfTable(catalog, tableName);
        vector<string> lines;
        getLinesFromFile(filePath, lines, "Table");

        if (lines.size() == 0)
        {
            cout << EMPTY_FILE << endl;
            return;
        }
        bool columnNameIsUnique = true;
        checkColumnName(columnNameIsUnique, lines, columnName);

        if (!columnNameIsUnique)
        {
            cout << "The column name must be unique!" << endl;
            return;
        }

        vector<string> line = splitLine(lines[0], SEPARATOR_TABLE_FILES);
        vector<string> newColumn;
        newColumn.push_back(columnName);
        newColumn.push_back(columnType);
        for (int i = 0; i < line.size() - 2; i++)
        {
            newColumn.push_back("NULL");
        }

        appendNewColumnToTableFile(filePath, newColumn);

        cout << "The column has been successfully added to the table!" << endl;
    }
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����),
    ���������� ��������� ��� �����.
*/
void overwriteTableFile(string filePath, vector<string>& cells, int countRows, int countColumns)
{
    ofstream outputFileStream;
    outputFileStream.open(filePath, ios::out);

    if (outputFileStream.is_open())
    {
        for (int i = 0; i < countColumns; i++)
        {
            for (int k = i * countRows; k < (i + 1) * countRows; k++)
            {
                outputFileStream << cells[k];
                if (k % countRows != countRows - 1)
                {
                    outputFileStream << SEPARATOR_TABLE_FILES;
                }
                else
                {
                    outputFileStream << endl;
                }
            }
        }
        outputFileStream.close();
    }
    else
    {
        printErrorMessage(filePath);
    }
}

/**
    �������, ����� �� ������ ������ � ���������, ����� ������ � ������� ����� searchColumn �������
    ���������� searchValue �� �������� ����, �� �������� �� � ������� ����� targetColumn
    �� ������ �������� targetValue.
*/
void update(vector<Entry>& catalog, string tableName, int searchColumn, string searchValue, int targetColumn, string targetValue)
{
    vector<string> lines;
    string filePath = findPathOfTable(catalog, tableName);
    getLinesFromFile(filePath, lines, "Table");
    vector<string> cells = splitLines(lines);

    if (lines.size() == 0)
    {
        cout << EMPTY_FILE << endl;
        return;
    }

    int countColumns = lines.size();
    int countRows = cells.size() / countColumns;

    vector<int> positionsValue;
    if (searchColumn - 1 >= 0 && searchColumn - 1 < lines.size() && targetColumn - 1 >= 0 && targetColumn - 1 < lines.size())
    {
        if (searchValue != "NULL")
        {
            if (!typeOfValueIsSuitable(lines[searchColumn - 1], searchValue, "The value you are looking for in the column is not of its type!"))
            {
                return;
            }
        }

        if (targetValue != "NULL")
        {
            if (!typeOfValueIsSuitable(lines[targetColumn - 1], targetValue, "The value you entered is not of the type that the column can accept!"))
            {
                return;
            }
        }

        vector<string> values = splitLine(lines[searchColumn - 1], SEPARATOR_TABLE_FILES);
        for (int i = 0; i < values.size(); i++)
        {
            if (values[i] == searchValue)
            {
                positionsValue.push_back(i);
            }
        }

        if (positionsValue.size() == 0)
        {
            cout << "This value does not appear in the column you are looking for." << endl;
        }
        else
        {
            for (int row = 0; row < positionsValue.size(); ++row)
            {
                for (int index = positionsValue[row]; index < cells.size(); index += countRows)
                {
                    if (index == (targetColumn - 1) * countRows + positionsValue[row])
                    {
                        cells[index] = targetValue;
                    }
                }
            }

            overwriteTableFile(filePath, cells, countRows, countColumns);

            cout << "The table was successfully updated!" << endl;
        }
    }
    else
    {
        cout << INVALID_COLUMN_INDEX << endl;
    }
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����),
    ���������� �������, ���� ����� ���� ��������� �� ������� � ������� � ��������� � ������� ������� (deleteSpecifiedRows),
    ����� ������� ��������, ����� � �����.
*/
void deleteRowsInFile(string filePath, vector<string>& cells, int countRows, int countColumns)
{
    ofstream outputFileStream;
    outputFileStream.open(filePath, ios::out);

    if (outputFileStream.is_open())
    {
        int counter = 0;
        for (int i = 0; i < countColumns; i++)
        {
            for (int k = i; k < cells.size(); k += countColumns)
            {
                counter++;
                outputFileStream << cells[k];
                if (counter != countRows)
                {
                    outputFileStream << SEPARATOR_TABLE_FILES;
                }
                else
                {
                    outputFileStream << endl;
                }
            }
            counter = 0;
        }

        outputFileStream.close();
    }
    else
    {
        printErrorMessage(filePath);
    }
}

/**
    �������, ����� ������� ������ ������ � ���������,
    ����� ������ searchColumn ������� ���������� searchValue.
*/
void deleteSpecifiedRows(vector<Entry>& catalog, string tableName, int searchColumn, string searchValue)
{
    vector<string> lines;
    string filePath = findPathOfTable(catalog, tableName);

    getLinesFromFile(filePath, lines, "Table");
    vector<string> cells = splitLines(lines);

    if (lines.size() == 0)
    {
        cout << EMPTY_FILE << endl;
        return;
    }

    int countColumns = lines.size();
    int countRows = cells.size() / countColumns;

    vector<int> positionsValue;
    if (searchColumn - 1 >= 0 && searchColumn - 1 < lines.size())
    {
        if (searchValue != "NULL")
        {
            if (!typeOfValueIsSuitable(lines[searchColumn - 1], searchValue, "The value you are looking for in the column is not of its type!"))
            {
                return;
            }
        }

        vector<string> values = splitLine(lines[searchColumn - 1], SEPARATOR_TABLE_FILES);
        for (int i = 0; i < values.size(); i++)
        {
            if (values[i] == searchValue)
            {
                positionsValue.push_back(i);
            }
        }

        if (positionsValue.size() == 0)
        {
            cout << "This value does not appear in the column you are looking for." << endl;
        }
        else
        {
            vector<string> newCells;
            for (int row = 0; row < countRows; ++row)
            {
                for (int index = row; index < cells.size(); index += countRows)
                {
                    if (!count(positionsValue.begin(), positionsValue.end(), row))
                    {
                        newCells.push_back(cells[index]);
                    }
                }
            }

            deleteRowsInFile(filePath, newCells, countRows - positionsValue.size(), countColumns);

            if (positionsValue.size() == 1)
            {
                cout << "The row has been successfully deleted from the table!" << endl;
            }
            else
            {
                cout << "The rows have been successfully deleted from the table!" << endl;
            }
        }
    }
    else
    {
        cout << INVALID_COLUMN_INDEX << endl;
    }
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����),
    ���������� �������, ���� ������ ��� ��� � ���������.
*/
void addRow(string filePath, vector<string>& lines, vector<string>& values)
{
    ofstream outputFileStream;
    outputFileStream.open(filePath, ios::out);

    if (outputFileStream.is_open())
    {
        for (int i = 0; i < lines.size(); i++)
        {
            outputFileStream << lines[i] << SEPARATOR_TABLE_FILES << values[i] << endl;
        }
    }
    else {
        printErrorMessage(filePath);
    }
}

/**
    �������, ����� ������ � ������� ���, ���� ����� ���� �� ���������� �����������
    ���� �� ��������� �� �������� � ���������.
*/
void insert(vector<Entry>& catalog, string tableName, vector<string>& values)
{
    vector<string> lines;
    string filePath = findPathOfTable(catalog, tableName);
    getLinesFromFile(filePath, lines, "Table");

    if (lines.size() == 0)
    {
        cout << EMPTY_FILE << endl;
        return;
    }

    if (lines.size() != values.size())
    {
        cout << "The number of values does not match the number of columns!" << endl;
        return;
    }

    for (int i = 0; i < lines.size(); i++)
    {
        if (!typeOfValueIsSuitable(lines[i], values[i], "The value you entered is not of the type that the column can accept!"))
        {
            return;
        }
    }

    addRow(filePath, lines, values);
    cout << "The row has been successfully added to the table!" << endl;
}

/**
    �������, ����� ���������� ��� �� ������� � ��������.
*/
void renameTable(string filePath, vector<Entry>& catalog, string oldTableName, string newTableName)
{
    for (int i = 0; i < catalog.size(); i++)
    {
        if (catalog[i].getName() == oldTableName)
        {
            catalog[i].setName(newTableName);
        }
    }

    ofstream outputFileStream;
    outputFileStream.open(filePath, ios::out);

    string line;
    if (outputFileStream.is_open())
    {
        for (int i = 0; i < catalog.size(); i++)
        {
            outputFileStream << catalog[i];

        }
        outputFileStream.close();
    }
    else
    {
        printErrorMessage(filePath);
    }

    cout << "The table was successfully renamed!" << endl;
}

/**
    �������, ����� ������ ���� �� �������� � ���������, ����� ������ searchColumn
    ������� ���������� �������� searchValue.
*/
void count(vector<Entry>& catalog, string tableName, int searchColumn, string searchValue)
{
    vector<string> lines;
    string filePath = findPathOfTable(catalog, tableName);

    getLinesFromFile(filePath, lines, "Table");
    vector<string> cells = splitLines(lines);

    if (lines.size() == 0)
    {
        cout << EMPTY_FILE << endl;
        return;
    }

    int countColumns = lines.size();
    int countRows = cells.size() / countColumns;

    vector<int> positionsValue;
    if (searchColumn - 1 >= 0 && searchColumn - 1 < lines.size())
    {
        if (searchValue != "NULL")
        {
            if (!typeOfValueIsSuitable(lines[searchColumn - 1], searchValue, "The value you are looking for in the column is not of its type!"))
            {
                return;
            }
        }

        vector<string> values = splitLine(lines[searchColumn - 1], SEPARATOR_TABLE_FILES);
        for (int i = 0; i < values.size(); i++)
        {
            if (values[i] == searchValue)
            {
                positionsValue.push_back(i);
            }
        }

        if (positionsValue.size() == 0)
        {
            cout << "This value does not appear in the column you are looking for." << endl;
        }
        else
        {
            cout << "The number of rows where this value is contained in the specified column is " << positionsValue.size() << "." << endl;
        }
    }
    else
    {
        cout << INVALID_COLUMN_INDEX << endl;
    }
}

/**
    �������, ����� ��������� ���� ����� �� �������� � ������.
*/
bool columnIsNumeric(string line)
{
    bool typeOfValueIsSuitable = true;
    int indexFirstVerticalBar = line.find_first_of(SEPARATOR_TABLE_FILES);
    line = line.substr(indexFirstVerticalBar + 1);
    int indexSecondVerticalBar = line.find_first_of(SEPARATOR_TABLE_FILES);
    string type = line.substr(0, indexSecondVerticalBar);

    if (type == TYPES[0] || type == TYPES[1])
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
    �������, ����� �������� ������ �������� ����� ����������� �� �������� targetColumn �� ������ ������,
    ����� ������ � ����� searchColumn �������� ����������  searchValue. ���������� �������� ��:
    sum, product, maximum, minimum.
*/
void aggregate(vector<Entry>& catalog, string tableName, int searchColumn, string searchValue, int targetColumn, string operation)
{
    vector<string> lines;
    string filePath = findPathOfTable(catalog, tableName);
    getLinesFromFile(filePath, lines, "Table");
    vector<string> cells = splitLines(lines);

    if (lines.size() == 0)
    {
        cout << EMPTY_FILE << endl;
        return;
    }

    int countColumns = lines.size();
    int countRows = cells.size() / countColumns;

    vector<int> positionsValue;
    if (searchColumn - 1 >= 0 && searchColumn - 1 < lines.size() && targetColumn - 1 >= 0 && targetColumn - 1 < lines.size())
    {
        if (searchValue != "NULL")
        {
            if (!typeOfValueIsSuitable(lines[searchColumn - 1], searchValue, "The value you are looking for in the column is not of its type!"))
            {
                return;
            }
        }

        if (!columnIsNumeric(lines[targetColumn - 1]))
        {
            cout << "This column is not of numeric type!" << endl;
        }

        vector<string> values = splitLine(lines[searchColumn - 1], SEPARATOR_TABLE_FILES);
        for (int i = 0; i < values.size(); i++)
        {
            if (values[i] == searchValue)
            {
                positionsValue.push_back(i);
            }
        }

        if (positionsValue.size() == 0)
        {
            cout << "This value does not appear in the column you are looking for." << endl;
        }
        else
        {
            vector<double> nums;

            for (int row = 0; row < positionsValue.size(); ++row)
            {
                for (int index = positionsValue[row]; index < cells.size(); index += countRows)
                {
                    if (index == (targetColumn - 1) * countRows + positionsValue[row])
                    {
                        if (cells[index] != "NULL")
                        {
                            nums.push_back(stod(cells[index]));
                        }
                    }
                }
            }

            double result = 0.0;
            if (operation == "sum")
            {
                for (int i = 0; i < nums.size(); i++)
                {
                    result += nums[i];
                }
            }
            else if (operation == "product")
            {
                result += 1.0;
                for (int i = 0; i < nums.size(); i++)
                {
                    result *= nums[i];
                }
            }
            else if (operation == "maximum")
            {
                sort(nums.begin(), nums.end(), greater<double>());
                result = nums[0];
            }
            else if (operation == "minimum")
            {
                sort(nums.begin(), nums.end());
                result = nums[0];
            }
            else
            {
                cout << "This operation is not recognized!" << endl;
                return;
            }

            if (result == floor(result))
            {
                cout << "The result is: " << result << endl;
            }
            else
            {
                cout << "The result is: " << fixed << setprecision(DECIMAL_PLACES) << result << endl;
            }
        }
    }
    else
    {
        cout << INVALID_COLUMN_INDEX << endl;
    }
}

/**
    �������, ����� �� ������� �������� ��� (��� ��� ��������� ������� � �����, � ������������ �� �����),
    ���������� �������� � ��� ����.
*/
void saveAs(string filePath, string newFilePath)
{
    if (fileExists(newFilePath))
    {
        cout << "There is already a file with that name!" << endl;
    }
    else
    {
        vector<string> lines;
        getLinesFromFile(filePath, lines, "Catalog");
        createFile(newFilePath, lines);
    }
}

/**
    �������, ����� ������� ���������� �� ���������� ������ � ��������� � ����������.
*/
void helpCommand()
{
    cout << "The following commands are supported:" << endl;
    cout << "close                      closes currently opened file" << endl;
    cout << "save                       saves the currently open file" << endl;
    cout << "saveas <file>              saves the currently open file in <file>" << endl;
    cout << "help                       prints this information" << endl;
    cout << "exit                       exits the program" << '\n' << endl;

    cout << "import <file name>         adds a new file table to the database" << endl;
    cout << "showtables                 displays a list of the names of all loaded tables" << endl;
    cout << "describe <name>            displays information about the column types of a table" << endl;
    cout << "print <name>               displays all rows in a table" << endl;
    cout << "export <name> <file name>  saves a table to a file" << endl;

    cout << endl;
    cout << "select <column-n> <value> <table name>" << endl;
    cout << "Displays all rows in the table that contain the value \"value\" in the cell with the given sequence number" << endl;

    cout << endl;
    cout << "addcolumn <table name> <column name> <column type>" << endl;
    cout << "Adds a new column (with the largest number) to a table. For all existing rows in the table, the value of this column should be empty" << endl;

    cout << endl;
    cout << "update <table name> <search column n> <search value> <target column n> <target value>" << endl;
    cout << "For all rows in the table whose column with sequence number <search column n> contains the value <search column value> are changed "
        << "so that their column with sequence number <target column n> gets value <target value>. The NULL value is maintained" << endl;

    cout << endl;
    cout << "delete <table name> <search column n> <search value>" << endl;
    cout << "Deletes all rows in the table whose column <search column n> contains the value <search column value>" << endl;

    cout << endl;
    cout << "insert <table name> <column 1> ... <column n>" << endl;
    cout << "Inserts a new row in the table with the corresponding values" << endl;

    cout << endl;
    cout << "rename <old name> <new name>" << endl;
    cout << "Renames a table. Prints an error if the new name is not unique" << endl;

    cout << endl;
    cout << "count <table name> <search column n> <search value>" << endl;
    cout << "Finds the number of rows in the table whose columns contain the given value" << endl;

    cout << endl;
    cout << "aggregate <table name> <search column n> <search value> <target column n> <operation>" << endl;
    cout << "Performs an operation on the values in the <target column n> column of all rows whose columns numbered <search column n> "
         << "contain the value <search value>. The possible operations are sum, product, maximum, minimum." << endl;
    cout << endl;
}

/**
    �������, ����� ��������� ��������� describe � ��������� ���� ��������� ����������.
*/
void describeCommand(vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    string tableName = splittedCommand[1];
    if (checkIfTableExists(catalog, tableName))
    {
        describe(catalog, tableName);
    }
    else if (!checkIfTableExists(entries, tableName))
    {
        cout << TABLE_DOES_NOT_EXIST << endl;
    }
    else
    {
        cout << TABLE_IS_NOT_SAVED << endl;
    }
}

/**
    �������, ����� ��������� ��������� print � ��������� ���� ��������� ����������.
*/
void printCommand(vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    string tableName = splittedCommand[1];
    if (checkIfTableExists(catalog, tableName))
    {
        print(catalog, tableName);
    }
    else if (!checkIfTableExists(entries, tableName))
    {
        cout << TABLE_DOES_NOT_EXIST << endl;
    }
    else
    {
        cout << TABLE_IS_NOT_SAVED << endl;
    }
}

/**
    �������, ����� ��������� ��������� export, ��������� ���� ��������� ����������
    � ���� ���� ��� ����� ���� � �������.
*/
void exportCommand(vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    string tableName = splittedCommand[1];
    if (checkIfTableExists(catalog, tableName))
    {
        string commandPath = splittedCommand[2];
        if (checkFilePath(commandPath))
        {
            exportTable(catalog, tableName, commandPath);
        }
    }
    else if (!checkIfTableExists(entries, tableName))
    {
        cout << TABLE_DOES_NOT_EXIST << endl;
    }
    else
    {
        cout << TABLE_IS_NOT_SAVED << endl;
    }
}

/**
    �������, ����� ��������� ��������� select, ��������� ���� ��������� ����������
    � ��������� �� ���������� �����������.
*/
void selectCommand(vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    bool everythingIsFine = false;
    int columnN = 0;
    try
    {
        columnN = stoi(splittedCommand[1]);
        everythingIsFine = true;
    }
    catch (...)
    {
        cout << INVALID_COLUMN_ENTRY << endl;
    }
    if (everythingIsFine)
    {
        string value = splittedCommand[2];
        string tableName = splittedCommand[3];
        if (checkIfTableExists(catalog, tableName))
        {
            select(catalog, columnN, value, tableName);
        }
        else if (!checkIfTableExists(entries, tableName))
        {
            cout << TABLE_DOES_NOT_EXIST << endl;
        }
        else
        {
            cout << TABLE_IS_NOT_SAVED << endl;
        }
    }
}

/**
    �������, ����� ��������� ��������� addcolumn � ��������� ���� ��������� ����������.
*/
void addColumnCommand(vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    string tableName = splittedCommand[1];
    if (checkIfTableExists(catalog, tableName))
    {
        string columnName = splittedCommand[2];
        string columnType = splittedCommand[3];
        addColumn(catalog, tableName, columnName, columnType);
    }
    else if (!checkIfTableExists(entries, tableName))
    {
        cout << TABLE_DOES_NOT_EXIST << endl;
    }
    else
    {
        cout << TABLE_IS_NOT_SAVED << endl;
    }
}

/**
    �������, ����� ��������� ��������� update, ��������� ���� ��������� ����������
    � ��������� �� ���������� �����������.
*/
void updateCommand(vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    bool everythingIsFine = false;
    int searchColumn = 0;
    int targetColumn = 0;
    try
    {
        searchColumn = stoi(splittedCommand[2]);
        targetColumn = stoi(splittedCommand[4]);
        everythingIsFine = true;
    }
    catch (...)
    {
        cout << INVALID_COLUMN_ENTRY << endl;
    }
    if (everythingIsFine)
    {
        string tableName = splittedCommand[1];
        if (checkIfTableExists(catalog, tableName))
        {
            string searchValue = splittedCommand[3];
            string targetValue = splittedCommand[5];
            update(catalog, tableName, searchColumn, searchValue, targetColumn, targetValue);
        }
        else if (!checkIfTableExists(entries, tableName))
        {
            cout << TABLE_DOES_NOT_EXIST << endl;
        }
        else
        {
            cout << TABLE_IS_NOT_SAVED << endl;
        }
    }
}

/**
    �������, ����� ��������� ��������� delete, ��������� ���� ��������� ����������
    � ��������� �� ���������� �����������.
*/
void deleteCommand(vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    bool everythingIsFine = false;
    int searchColumn = 0;
    try
    {
        searchColumn = stoi(splittedCommand[2]);
        everythingIsFine = true;
    }
    catch (...)
    {
        cout << INVALID_COLUMN_ENTRY << endl;
    }
    if (everythingIsFine)
    {
        string tableName = splittedCommand[1];
        if (checkIfTableExists(catalog, tableName))
        {
            string searchValue = splittedCommand[3];
            deleteSpecifiedRows(catalog, tableName, searchColumn, searchValue);
        }
        else if (!checkIfTableExists(entries, tableName))
        {
            cout << TABLE_DOES_NOT_EXIST << endl;
        }
        else
        {
            cout << TABLE_IS_NOT_SAVED << endl;
        }
    }
}

/**
    �������, ����� ��������� ��������� insert � ��������� ���� ��������� ����������.
*/
void insertCommand(vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    string tableName = splittedCommand[1];
    if (checkIfTableExists(catalog, tableName))
    {
        vector<string> values;
        for (int i = 2; i < splittedCommand.size(); i++)
        {
            values.push_back(splittedCommand[i]);
        }

        insert(catalog, tableName, values);
    }
    else if (!checkIfTableExists(entries, tableName))
    {
        cout << TABLE_DOES_NOT_EXIST << endl;
    }
    else
    {
        cout << TABLE_IS_NOT_SAVED << endl;
    }
}

/**
    �������, ����� ��������� ��������� rename � ��������� ���� ��������� ����������.
*/
void renameCommand(string filePath, vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    string oldTableName = splittedCommand[1];
    if (checkIfTableExists(catalog, oldTableName))
    {
        string newTableName = splittedCommand[2];
        if (checkIfTableExists(catalog, newTableName))
        {
            cout << "This name already exists in the catalog!" << endl;
        }
        else if (!checkIfTableExists(entries, newTableName))
        {
            renameTable(filePath, catalog, oldTableName, newTableName);
        }
        else
        {
            cout << "A table with this name is already loaded, but it is not yet saved in the catalog!" << endl;
        }
    }
    else if (!checkIfTableExists(entries, oldTableName))
    {
        cout << TABLE_DOES_NOT_EXIST << endl;
    }
    else
    {
        cout << TABLE_IS_NOT_SAVED << endl;
    }
}

/**
    �������, ����� ��������� ��������� count, ��������� ���� ��������� ����������
    � ��������� �� ���������� �����������.
*/
void countCommand(vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    bool everythingIsFine = false;
    int searchColumn = 0;
    try
    {
        searchColumn = stoi(splittedCommand[2]);
        everythingIsFine = true;
    }
    catch (...)
    {
        cout << INVALID_COLUMN_ENTRY << endl;
    }
    if (everythingIsFine)
    {
        string tableName = splittedCommand[1];
        string searchValue = splittedCommand[3];
        if (checkIfTableExists(catalog, tableName))
        {
            count(catalog, tableName, searchColumn, searchValue);
        }
        else if (!checkIfTableExists(entries, tableName))
        {
            cout << TABLE_DOES_NOT_EXIST << endl;
        }
        else
        {
            cout << TABLE_IS_NOT_SAVED << endl;
        }
    }
}

/**
    �������, ����� ��������� ��������� aggregate, ��������� ���� ��������� ����������
    � ��������� �� ���������� �����������.
*/
void aggregateCommand(vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    bool everythingIsFine = false;
    int searchColumn = 0;
    int targetColumn = 0;
    try
    {
        searchColumn = stoi(splittedCommand[2]);
        targetColumn = stoi(splittedCommand[4]);
        everythingIsFine = true;
    }
    catch (...)
    {
        cout << INVALID_COLUMN_ENTRY << endl;
    }
    if (everythingIsFine)
    {
        string tableName = splittedCommand[1];
        if (checkIfTableExists(catalog, tableName))
        {
            string searchValue = splittedCommand[3];
            string operation = splittedCommand[5];
            aggregate(catalog, tableName, searchColumn, searchValue, targetColumn, operation);
        }
        else if (!checkIfTableExists(entries, tableName))
        {
            cout << TABLE_DOES_NOT_EXIST << endl;
        }
        else
        {
            cout << TABLE_IS_NOT_SAVED << endl;
        }
    }
}

/**
    �������, ����� ������� ��������� � ��������.
*/
void saveCommand(string filePath, vector<Entry>& catalog, vector<Entry>& entries)
{
    completeCatalogFile(filePath, entries);
    entries.clear();
    catalog.clear();
    loadCatalog(filePath, catalog);
}

/**
    �������, ����� ��������� ��������� saveAs � ��������� ���� ���� �� ����� ���� � �������.
*/
void saveAsCommand(string filePath, vector<string>& splittedCommand)
{
    string newCatalogPath = splittedCommand[1];
    if (checkFilePath(newCatalogPath))
    {
        saveAs(filePath, newCatalogPath);
    }
}

/**
    �������, ����� �������� ���������� ����� � �������.
*/
void closeCommand(string filePath, vector<Entry>& catalog, vector<Entry>& entries)
{
    if (!entries.empty())
    {
        string answer;
        cout << "Do you want to keep the changes made in database ?" << endl;
        answering(answer);
        if (answer == ANSWERS[1])
        {
            saveCommand(filePath, catalog, entries);
        }
        else
        {
            entries.clear();
        }
    }

    catalog.clear();
    cout << "The catalog has been successfully closed!" << endl;
}

/**
    �������, ����� �������� ��� ������� �� �� ������� � ���������� �� �����.
*/
void commandPattern(string filePath, vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    if (splittedCommand[0] == "import" && splittedCommand.size() == 2)
    {
        string commandPath = splittedCommand[1];
        if (checkFilePath(commandPath))
        {
            importCommand(commandPath, entries, catalog);
        }
    }
    else if (splittedCommand[0] == "import" && splittedCommand.size() == 1)
    {
        cout << "You forgot to enter the path to the table file!" << endl;
    }
    else if (splittedCommand[0] == "showtables" && splittedCommand.size() == 1)
    {
        showTables(catalog);
    }
    else if (splittedCommand[0] == "describe" && splittedCommand.size() == 2)
    {
        describeCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "describe" && splittedCommand.size() == 1)
    {
        cout << "You forgot to enter the table name!" << endl;
    }
    else if (splittedCommand[0] == "print" && splittedCommand.size() == 2)
    {
        printCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "print" && splittedCommand.size() == 1)
    {
        cout << "You forgot to enter the table name!" << endl;
    }
    else if (splittedCommand[0] == "export" && splittedCommand.size() == 3)
    {
        exportCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "export" && splittedCommand.size() < 3)
    {
        cout << "You forgot to enter the name of the table or the path where you want to save it!" << endl;
    }
    else if (splittedCommand[0] == "import" || splittedCommand[0] == "export" || splittedCommand[0] == "showtables"
        || splittedCommand[0] == "describe" || splittedCommand[0] == "print")
    {
        cout << "You have entered more parameters than required of you!" << endl;
    }
    else if (splittedCommand[0] == "select" && splittedCommand.size() == 4)
    {
        selectCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "select" && splittedCommand.size() < 4)
    {
        cout << ERROR_FORGOTTEN_PARAMETERS << endl;
    }
    else if (splittedCommand[0] == "addcolumn" && splittedCommand.size() == 4)
    {
        addColumnCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "addcolumn" && splittedCommand.size() < 4)
    {
        cout << ERROR_FORGOTTEN_PARAMETERS << endl;
    }
    else if (splittedCommand[0] == "update" && splittedCommand.size() == 6)
    {
        updateCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "update" && splittedCommand.size() < 6)
    {
        cout << ERROR_FORGOTTEN_PARAMETERS << endl;
    }
    else if (splittedCommand[0] == "delete" && splittedCommand.size() == 4)
    {
        deleteCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "delete" && splittedCommand.size() < 4)
    {
        cout << ERROR_FORGOTTEN_PARAMETERS << endl;
    }
    else if (splittedCommand[0] == "insert" && splittedCommand.size() < 3)
    {
        cout << "You have not entered a table name or the values you want to add for a row in it!" << endl;
    }
    else if (splittedCommand[0] == "insert")
    {
        insertCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "rename" && splittedCommand.size() == 3)
    {
        renameCommand(filePath, splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "rename" && splittedCommand.size() < 3)
    {
        cout << ERROR_FORGOTTEN_PARAMETERS << endl;
    }
    else if (splittedCommand[0] == "count" && splittedCommand.size() == 4)
    {
        countCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "count" && splittedCommand.size() < 4)
    {
        cout << ERROR_FORGOTTEN_PARAMETERS << endl;
    }
    else if (splittedCommand[0] == "aggregate" && splittedCommand.size() == 6)
    {
        aggregateCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "aggregate" && splittedCommand.size() < 6)
    {
        cout << ERROR_FORGOTTEN_PARAMETERS << endl;
    }
    else if (splittedCommand[0] == "save" && splittedCommand.size() == 1)
    {
        saveCommand(filePath, catalog, entries);
    }
    else if (splittedCommand[0] == "saveas" && splittedCommand.size() == 2)
    {
        saveAsCommand(filePath, splittedCommand);
    }
    else if (splittedCommand[0] == "close" && splittedCommand.size() == 1)
    {
        closeCommand(filePath, catalog, entries);
    }
    else if (splittedCommand[0] == "help" && splittedCommand.size() == 1)
    {
        helpCommand();
    }
    else if (splittedCommand[0] == "exit" && splittedCommand.size() == 1)
    {
        exit(0);
    }
    else
    {
        cout << "This command is not recognized!" << endl;
    }
}