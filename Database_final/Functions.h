#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> 
#include <algorithm>
#include <numeric>
#include "Entry.h"
#include "Messages.h"

const char SEPARATOR_TABLE_FILES = '|';                                                                 ///Разделителят във файловете, представляващи таблиците, е вертикалната черта.
const char DISTANCE_BETWEEN_COLUMNS[7] = "      ";                                                      ///Разстоянието между колоните.
const int TABLE_PRINTING_LIMITATION = 3;                                                                ///Колко реда от таблицата да се принтират
                                                                                                        ///с възможност за избор за следваща и предишна страница.

const int DECIMAL_PLACES = 2;                                                                           ///Десетични знаци след десетичната запетая, при печатане на резултата от числовите колони.
const string CATALOG_PATH = "Catalog.txt";                                                              ///Файла, в който ще се записват двойките - име на таблица и пътя към таблицата.
const string ANSWERS[2] = { "No", "Yes" };                                                              ///Позволените отговори при зададен въпрос.
const string TYPES[3] = { "int", "double", "string" };                                                  ///Позволените типове, с които базата данни може да работи.
const string OPERATIONS[3] = { "previous page", "next page", "exit" };                                  ///Операциите, които трябва да се въведат с цел избор за следваща, предишна страница или изход.
const string MATH_OPERATIONS[4] = { "sum", "product", "maximum", "minimum" };                           ///Операциите, които може да се извършват с колоните от числов тип.

/**
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла)
    взема само името и разширението на файла.
*/
string getNameFile(string filePath)
{
    int index = filePath.find_last_of('\\');
    string name;
    if (index != -1)
    {
        name = filePath.substr(index + 1);
    }
    else
    {
        name = filePath;
    }

    return name;
}

/**
    Долните четири функции са съобщения, изкарвани при успешно извършено действие или при грешка, докато се работи с файлове.
*/
void printErrorMessage(string filePath) {
    cerr << "\n";
    cerr << "There was a problem opening file with name: " << getNameFile(filePath) << endl;
    cerr << "\n";
}

void printReminderMessage() {
    cerr << "\n";
    cerr << "A file with the given name does not exist! Please check your path." << endl;
    cerr << "\n";
}

void printSuccessSaveMessage(string filePath) {
    cout << "===========================================================" << endl;
    cout << "All changes are saved successfully in the file " << getNameFile(filePath) << "!" << endl;
    cout << "===========================================================" << endl;
}

void printSuccessCreateMessage(string filePath) {
    cout << "===========================================================" << endl;
    cout << "Successfully created " << getNameFile(filePath) << "!" << endl;
    cout << "===========================================================" << endl;
}

/**
    Функция, която изисква положителен или отрицателен отговор от входа.
*/
void answering(string& answer)
{
    while (1)
    {
        cout << ENTER_ANSWER;
        getline(cin, answer);
        if (answer.size() > 0)
        {
            if (find(begin(ANSWERS), end(ANSWERS), answer) != end(ANSWERS))
            {
                break;
            }
            else
            {
                cout << ALLOWED_ANSWERS << endl;
            }
        }
        else
        {
            cout << NOT_ENTERED_ANSWER << endl;
        }
    }
}

/**
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла)
    проверява дали той е валиден.
*/
bool checkFilePath(string filePath)
{
    bool validFilePath = true;
    if (filePath.find(">") != -1 || filePath.find("<") != -1
        || filePath.find("/") != -1 || filePath.find("|") != -1)
    {
        cout << INVALID_PATH << endl;
        validFilePath = false;
    }

    return validFilePath;
}

/**
    Функция, която проверява по дадено име на таблица, дали тя е заредена в паметта.
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
    Функция, която проверява подадения тип дали е сред поддържаните от базата данни.
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
    Функция, която по дадено име на таблица, извлича от базата данни пътя до файла на тази таблица.
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
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла)
    отваря файл или ако не съществува такъв - създава на това място с даденото име и разширение,
    при условие, че се отнася за файла представляващ каталога. Ако е представляващ таблица,
    ще изведе напомнящо съобщение, че такъв файл няма и трябва да си провери пътя.
    Но ако съществува файла, прочита съдържанието му и пълни редовете от файла във вектор lines.
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
    Функция, която разделя даден символен низ според подадения разделител.
    И връща вектор от думите, които са били разделяни чрез него.
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
    Функция, която разделя вектор от символни низове на думи,
    с помощта на горната функция (splitLine) и връща вектор от тези думи.
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
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла)
    проверява дали съществува такъв файл.
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
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла)
    отваря файла, представляващ каталога, и изважда от него двойката - име на таблица и пътя на файла.
    Разделителя между компонентите на двойката е " | ", тъй като този символ не може да се съдържа
    в името на файл и следователно риска се свежда до минимум за неправилно вземане на някой компонент.
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
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла)
    отваря файла, представляващ каталога, и по подаден вектор от двойки (име на таблица и път на файла),
    пълни тези двойки във файла.
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
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла,
    представляващ таблица) създава двойка, състояща се от името на таблицата и пътя на файла.
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

    if (checkIfTableExists(catalog, tableName) || checkIfTableExists(entries, tableName))
    {
        cout << CANNOT_LOAD_TABLE << endl;
    }
    else
    {
        Entry entry = Entry(tableName, filePath);
        entries.push_back(entry);

        cout << TABLE_SUCCESSFULLY_ADDED << endl;
    }
}

/**
    Функция, която показва заредените таблици в паметта, като се има предвид,
    че новите вкарани таблици не се съдържат в каталога. За да се вкарат в каталога,
    трябва да се извърши действието save, тоест да се запазят в него.
*/
void showTables(vector<Entry>& catalog)
{
    if (catalog.size() == 0)
    {
        cout << NO_LOADED_TABLES << endl;
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
    Функция, която показва информация за типовете на колоните
    по подадено име на таблица.
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
    Функция, която сравнява размерите на подадените думи.
*/
bool compareWordSizes(string wordOne, string wordTwo)
{
    return wordOne.size() > wordTwo.size();
}

/**
    Функция, която намира коя е най-дългата дума във всяка колона от таблицата
    и взема нейния размер и го пълни във вектора.
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
    Функция, която чете от входа символен низ и го връща, като опциите,
    за този символен низ, които се предлагат на потребителя, са три.
*/
string readOperationFromInput(int row, int countRows)
{
    string operation;
    while (1)
    {
        cout << ENTER_OPERATION;
        getline(cin, operation);
        if (operation.size() > 0)
        {
            if (operation == OPERATIONS[2] || operation == OPERATIONS[1])
            {
                break;
            }
            else if (operation == OPERATIONS[0])
            {
                if (row == 2 + TABLE_PRINTING_LIMITATION)
                {
                    cout << NO_PREVIOUS_PAGE << endl;
                }
                else
                {
                    break;
                }
            }
            else
            {
                cout << INVALID_OPERATION << endl;
            }
        }
        else
        {
            cout << NOT_ENTERED_OPERATION << endl;
        }
    }

    return operation;
}

/**
    Функция, която позволява при представянето на избрано съдържание от таблица, то да може да се управлява,
    като се показва максимума, определен в началото, редове и се дава опция за предна и следваща страница, и изход.
*/
void tablePresentationSelect(vector<string>& cells, vector<int>& sizesLongesWords, int size, int counter, vector<int> positionsValue, int countRows, int& counterRows)
{
    if ((counterRows - 2) % TABLE_PRINTING_LIMITATION == 0 && counterRows != 2)
    {
        string operation = readOperationFromInput(counterRows, countRows);
        if (operation == OPERATIONS[0])
        {
            counterRows -= TABLE_PRINTING_LIMITATION * 2;
        }
        else if (operation == OPERATIONS[2])
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
    Функция, която позволява при представянето на съдържание на таблица, то да може да се управлява,
    като се показва максимума, определен в началото, редове и се дава опция за предна и следваща страница, и изход.
*/
void tablePresentationPrint(vector<string>& cells, vector<int>& sizesLongesWords, int size, int counter, int& row, int countRows)
{
    if ((row - 2) % TABLE_PRINTING_LIMITATION == 0 && row != 2)
    {
        string operation = readOperationFromInput(row, countRows);
        if (operation == OPERATIONS[0])
        {
            row -= TABLE_PRINTING_LIMITATION * 2;
        }
        else if (operation == OPERATIONS[2])
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
    Функция, която по дадено име на таблица, принтира нейното съдържание.
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

    cout << TABLE_INFO_VIEW << endl;
    cout << POSSIBLE_OPERATIONS << endl;
    int counter = 0;
    int size = 0;
    for (int row = 0; row < countRows; ++row)
    {
        tablePresentationPrint(cells, sizesLongesWords, size, counter, row, countRows);
    }
}

/**
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла),
    създава такъв файл и го пълни със символните низове от подадения вектор.
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
    Функция, която копира съдържанието на дадена таблица в друг файл, като преди това го създава
    с помощта на горната функция (createFile).
*/
void exportTable(vector<Entry>& catalog, string tableName, string filePath)
{
    if (fileExists(filePath))
    {
        cout << FILE_EXISTS_SAME_NAME << endl;
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
    Функция, която поверява дали подадената стойност е от типа, който се изисква от колоната.
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
    Функция, която извежда всички редове от таблицата, които съдържат стойността “value”,
    която се търси в колоната с пореден номер columnN.
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
            if (!typeOfValueIsSuitable(lines[columnN - 1], value, VALUE_IS_NOT_SAME_TYPE))
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
            cout << VALUE_DOES_NOT_APPEAR << endl;
        }
        else
        {
            cout << SELECTION_VIEW_INFO << endl;
            cout << POSSIBLE_OPERATIONS << endl;
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
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла),
    отваря файла и го допълва с ред, съставен от вектора от символни низове.
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
    Функция, която проверява дали името на колоната, която трябва да се добави, е уникално.
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
    Функция, която добавя нова колона в таблица с помощта на горната функция (appendNewColumnToTableFile).
*/
void addColumn(vector<Entry>& catalog, string tableName, string columnName, string columnType)
{
    if (!checkType(columnType))
    {
        cout << TYPE_NOT_SUPPORTED << endl;
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
            cout << COLUMN_NAME_MUST_UNIQUE << endl;
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

        cout << COLUMN_SUCCESSFULLY_ADDED << endl;
    }
}

/**
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла),
    пренаписва таблицата във файла.
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
    Функция, която за всички редове в таблицата, чиято колона с пореден номер searchColumn съдържа
    стойността searchValue се променят така, че колоната им с пореден номер targetColumn
    да получи стойност targetValue.
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
            if (!typeOfValueIsSuitable(lines[searchColumn - 1], searchValue, VALUE_IS_NOT_SAME_TYPE))
            {
                return;
            }
        }

        if (targetValue != "NULL")
        {
            if (!typeOfValueIsSuitable(lines[targetColumn - 1], targetValue, COLUMN_CANNOT_ACCEPT_VALUE))
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
            cout << VALUE_DOES_NOT_APPEAR << endl;
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

            cout << TABLE_SUCCESSFULLY_UPDATED << endl;
        }
    }
    else
    {
        cout << INVALID_COLUMN_INDEX << endl;
    }
}

/**
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла),
    пренаписва таблица, като преди това таблицата се зарежда в паметта и обработва с долната функция (deleteSpecifiedRows),
    която изтрива редовете, които е нужно.
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
    Функция, която изтрива всички редове в таблицата,
    чиято колона searchColumn съдържа стойността searchValue.
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
            if (!typeOfValueIsSuitable(lines[searchColumn - 1], searchValue, VALUE_IS_NOT_SAME_TYPE))
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
            cout << VALUE_DOES_NOT_APPEAR << endl;
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
                cout << ROW_SUCCESSFULLY_DELETED << endl;
            }
            else
            {
                cout << ROWS_SUCCESSFULLY_DELETED << endl;
            }
        }
    }
    else
    {
        cout << INVALID_COLUMN_INDEX << endl;
    }
}

/**
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла),
    пренаписва таблица, като добавя нов ред в таблицата.
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
    Функция, която вмъква в таблица ред, като преди това се проверяват стойностите
    дали са подходящи за колоните в таблицата.
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
        cout << MORE_VALUES << endl;
        return;
    }

    for (int i = 0; i < lines.size(); i++)
    {
        if (!typeOfValueIsSuitable(lines[i], values[i], COLUMN_CANNOT_ACCEPT_VALUE))
        {
            return;
        }
    }

    addRow(filePath, lines, values);
    cout << ROW_SUCCESSFULLY_ADDED << endl;
}

/**
    Функция, която преименува име на таблица в каталога.
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

    cout << TABLE_SUCCESSFULLY_RENAMED << endl;
}

/**
    Функция, която намира броя на редовете в таблицата, чиято колона searchColumn
    съдържа подадената стойност searchValue.
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
            if (!typeOfValueIsSuitable(lines[searchColumn - 1], searchValue, VALUE_IS_NOT_SAME_TYPE))
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
            cout << VALUE_DOES_NOT_APPEAR << endl;
        }
        else
        {
            cout << NUMBER_ROWS_SPECIFIED_VALUE << positionsValue.size() << "." << endl;
        }
    }
    else
    {
        cout << INVALID_COLUMN_INDEX << endl;
    }
}

/**
    Функция, която проверява дали типът на колоната е числов.
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
    Функция, която извършва дадена операция върху стойностите от колоната targetColumn на всички редове,
    чиито колони с номер searchColumn съдържат стойността  searchValue. Възможните операции са:
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
            if (!typeOfValueIsSuitable(lines[searchColumn - 1], searchValue, VALUE_IS_NOT_SAME_TYPE))
            {
                return;
            }
        }

        if (!columnIsNumeric(lines[targetColumn - 1]))
        {
            cout << COLUMN_IS_NOT_NUMERIC << endl;
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
            cout << VALUE_DOES_NOT_APPEAR << endl;
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
            if (operation == MATH_OPERATIONS[0])
            {
                for (int i = 0; i < nums.size(); i++)
                {
                    result += nums[i];
                }
            }
            else if (operation == MATH_OPERATIONS[1])
            {
                result += 1.0;
                for (int i = 0; i < nums.size(); i++)
                {
                    result *= nums[i];
                }
            }
            else if (operation == MATH_OPERATIONS[2])
            {
                sort(nums.begin(), nums.end(), greater<double>());
                result = nums[0];
            }
            else if (operation == MATH_OPERATIONS[3])
            {
                sort(nums.begin(), nums.end());
                result = nums[0];
            }
            else
            {
                cout << MATH_OPERATION_NOT_RECOGNIZED << endl;
                return;
            }

            if (result == floor(result))
            {
                cout << RESULT_IS << floor(result) << endl;
            }
            else
            {
                cout << RESULT_IS << fixed << setprecision(DECIMAL_PLACES) << result << endl;
            }
        }
    }
    else
    {
        cout << INVALID_COLUMN_INDEX << endl;
    }
}

/**
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла),
    презаписва каталога в нов файл.
*/
void saveAs(string filePath, string newFilePath)
{
    if (fileExists(newFilePath))
    {
        cout << FILE_EXISTS_SAME_NAME << endl;
    }
    else
    {
        vector<string> lines;
        getLinesFromFile(filePath, lines, "Catalog");
        createFile(newFilePath, lines);
    }
}

/**
    Функция, която изкарва информация за правилната работа с командите в програмата.
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
    cout << "export <name> <file name>  saves a table to a file" << '\n' << endl;

    cout << "select <column-n> <value> <table name>" << endl;
    cout << "Displays all rows in the table that contain the value \"value\" in the cell with the given sequence number" << endl;
    cout << "Example: select;2;\"Sofia\";Addresses" << endl;
    cout << endl;

    cout << "addcolumn <table name> <column name> <column type>" << endl;
    cout << "Adds a new column (with the largest number) to a table. For all existing rows in the table, the value of this column should be empty" << endl;
    cout << "If the name of the column contains more than one word, use a underscore as a separator" << endl;
    cout << "Example: addcolumn;Clients;age;int" << endl;
    cout << endl;

    cout << "update <table name> <search column n> <search value> <target column n> <target value>" << endl;
    cout << "For all rows in the table whose column with sequence number <search column n> contains the value <search column value> are changed "
         << "so that their column with sequence number <target column n> gets value <target value>. The NULL value is maintained" << endl;
    cout << "Example: update;Credit;2;NULL;2;25.48" << endl;
    cout << endl;

    cout << "delete <table name> <search column n> <search value>" << endl;
    cout << "Deletes all rows in the table whose column <search column n> contains the value <search column value>" << endl;
    cout << "Example: delete;Credit;2;25.48" << endl;
    cout << endl;

    cout << "insert <table name> <column 1> ... <column n>" << endl;
    cout << "Inserts a new row in the table with the corresponding values" << endl;
    cout << "Example: insert;Clients;5;\"Svetlin Dimitrov\";\"no\";52" << endl;
    cout << endl;

    cout << "rename <old name> <new name>" << endl;
    cout << "Renames a table. Prints an error if the new name is not unique" << endl;
    cout << "If the new name contains more than one word, use a underscore as a separator" << endl;
    cout << "Example: rename;Addresses;Place_of_residence" << endl;
    cout << endl;

    cout << "count <table name> <search column n> <search value>" << endl;
    cout << "Finds the number of rows in the table whose columns contain the given value" << endl;
    cout << "Example: count;Place_of_residence;2;\"Varna\"" << endl;
    cout << endl;

    cout << "aggregate <table name> <search column n> <search value> <target column n> <operation>" << endl;
    cout << "Performs an operation on the values in the <target column n> column of all rows whose columns numbered <search column n> "
         << "contain the value <search value>. The possible operations are sum, product, maximum, minimum." << endl;
    cout << "Example: aggregate;Credit;1;1;2;sum" << endl;
    cout << endl;
}

/**
    Функция, която изпълнява функцията describe и проверява дали таблицата съществува.
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
    Функция, която изпълнява функцията print и проверява дали таблицата съществува.
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
    Функция, която изпълнява функцията export, проверява дали таблицата съществува
    и дали пътя към новия файл е валиден.
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
    Функция, която изпълнява функцията select, проверява дали таблицата съществува
    и проверява за коректност параметрите.
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
    Функция, която изпълнява функцията addcolumn и проверява дали таблицата съществува.
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
    Функция, която изпълнява функцията update, проверява дали таблицата съществува
    и проверява за коректност параметрите.
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
    Функция, която изпълнява функцията delete, проверява дали таблицата съществува
    и проверява за коректност параметрите.
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
    Функция, която изпълнява функцията insert и проверява дали таблицата съществува.
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
    Функция, която изпълнява функцията rename и проверява дали таблицата съществува.
*/
void renameCommand(string filePath, vector<string>& splittedCommand, vector<Entry>& catalog, vector<Entry>& entries)
{
    string oldTableName = splittedCommand[1];
    if (checkIfTableExists(catalog, oldTableName))
    {
        string newTableName = splittedCommand[2];
        if (checkIfTableExists(catalog, newTableName))
        {
            cout << NAME_EXISTS_CATALOG << endl;
        }
        else if (!checkIfTableExists(entries, newTableName))
        {
            renameTable(filePath, catalog, oldTableName, newTableName);
        }
        else
        {
            cout << NEW_TABLE_NAME_LOADED << endl;
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
    Функция, която изпълнява функцията count, проверява дали таблицата съществува
    и проверява за коректност параметрите.
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
    Функция, която изпълнява функцията aggregate, проверява дали таблицата съществува
    и проверява за коректност параметрите.
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
    Функция, която запазва промените в каталога.
*/
void saveCommand(string filePath, vector<Entry>& catalog, vector<Entry>& entries)
{
    completeCatalogFile(filePath, entries);
    entries.clear();
    catalog.clear();
    loadCatalog(filePath, catalog);
}

/**
    Функция, която изпълнява функцията saveAs и проверява дали пътя на новия файл е валиден.
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
    Функция, която изчиства заредените данни в паметта.
*/
void closeCommand(string filePath, vector<Entry>& catalog, vector<Entry>& entries)
{
    if (!entries.empty())
    {
        string answer;
        cout << DO_YOU_WANT_KEEP_CHANGES << endl;
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
    cout << CATALOG_SUCCESSFULLY_CLOSED << endl;
}

/**
    Функция, която определя коя команда да се изпълни в зависимост от входа.
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
        cout << NOT_ENTERED_PATH << endl;
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
        cout << NOT_ENTERED_TABLE_NAME << endl;
    }
    else if (splittedCommand[0] == "print" && splittedCommand.size() == 2)
    {
        printCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "print" && splittedCommand.size() == 1)
    {
        cout << NOT_ENTERED_TABLE_NAME << endl;
    }
    else if (splittedCommand[0] == "export" && splittedCommand.size() == 3)
    {
        exportCommand(splittedCommand, catalog, entries);
    }
    else if (splittedCommand[0] == "export" && splittedCommand.size() < 3)
    {
        cout << NOT_ENTERED_PATH_OR_NAME << endl;
    }
    else if (splittedCommand[0] == "import" || splittedCommand[0] == "export" || splittedCommand[0] == "showtables"
            || splittedCommand[0] == "describe" || splittedCommand[0] == "print")
    {
        cout << ERROR_MORE_PARAMETERS << endl;
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
        cout << NOT_ENTERED_NAME_OR_VALUES << endl;
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
    else if (splittedCommand[0] == "select" || splittedCommand[0] == "update" || splittedCommand[0] == "addcolumn"
            || splittedCommand[0] == "rename" || splittedCommand[0] == "delete" || splittedCommand[0] == "count" || splittedCommand[0] == "aggregate")
    {
    cout << ERROR_MORE_PARAMETERS << endl;
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
        cout << COMMAND_NOT_RECOGNIZED << endl;
    }
}