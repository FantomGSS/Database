#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> 
#include <algorithm>
#include "Column.h"
#include "Entry.h"

const string EMPTY_FILE = "There is no table in this file yet!";
const string TABLE_DOES_NOT_EXIST = "This table name does not exist in the database!";
const string TABLE_IS_NOT_SAVED = "To work with this table, you must save the changes in the catalog!";
const string ERROR_FORGOTTEN_PARAMETERS = "You forgot to enter the required number of parameters!";     ///Съобщение за грешка, когато не се въведат нужния брой параметри, изисквани от функцията.
const char DISTANCE_BETWEEN_COLUMNS[7] = "      ";                                                      ///Разстоянието между колоните
const int TABLE_PRINTING_LIMITATION = 3;                                                                ///Колко реда от таблицата да се принтират 
                                                                                                        ///с възможност за избор на следваща и предишна страница.
/**
    Долните четири функции са съобщения, изкарвани при успешно извършено действие или при грешка, докато се работи с файлове.
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

void printSuccessReadMessage(string filePath) {
    cout << "===========================================================" << endl;
    cout << "Successfully read " << filePath << "!" << endl;
    cout << "===========================================================" << endl;
}

void printSuccessCreateMessage(string filePath) {
    cout << "===========================================================" << endl;
    cout << "Successfully created " << filePath << "!" << endl;
    cout << "===========================================================" << endl;
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
        cout << "You have entered an invalid path! Forbidden characters are: / < | >" << endl;
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
    if (type == "int" || type == "double" || type == "string")
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
        if (condition == "Catalog")
        {
            printSuccessReadMessage(filePath);
        }
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
        vector<string> splittedLine = splitLine(lines[i], '|');

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
    в името на файл и следователно риска се свежда до минимум за неправилно взема на някой компонент.
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
    Функция, която показва заредените таблици в паметта, като се има предвид, 
    че новите вкарани таблици не се съдържат в каталога. За да се вкарат в каталога, 
    трябва да се извърши действието save, тоест да се запазят в него.
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
    Функция, която позволява при представянето на съдържание на таблица, то да може да се управлява, 
    като се показва максимума, определен в началото, редове и се дава опция за предна, следваща страница и изход.
*/
void tablePresentation(vector<string>& cells, vector<int>& sizesLongesWords, int size, int counter, int row, int countRows)
{
    if ((row - 2) % TABLE_PRINTING_LIMITATION == 0 && row != 2)
    {
        string operation = readOperationFromInput(row, countRows);
        if (operation == "previous page")
        {
            row -= TABLE_PRINTING_LIMITATION;
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

    cout << "The printing of a table is limited to three lines" << endl;
    cout << "The possible viewing options are: next page, previous page and exit." << endl;
    int counter = 0;
    int size = 0;
    for (int row = 0; row < countRows; ++row)
    {
        tablePresentation(cells, sizesLongesWords, size, counter, row, countRows);
    }
}

/**
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла),
    създава такъв файл и го пълни със символните низове от подадения вектор.
*/
void createTableFile(string filePath, vector<string>& lines)
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
    с помощта на горната функция (createTableFile).
*/
void exportCommand(vector<Entry>& catalog, string tableName, string filePath)
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
        createTableFile(filePath, lines);
    }
}

/**
    Функция, която поверява дали подадената стойност е от типа, който се изисква от колоната.
*/
bool typeOfValueIsSuitable(string line, string targetValue, string errorMessage)
{
    bool typeOfValueIsSuitable = true;
    int indexFirstVerticalBar = line.find_first_of('|');
    line = line.substr(indexFirstVerticalBar + 1);
    int indexSecondVerticalBar = line.find_first_of('|');
    string type = line.substr(0, indexSecondVerticalBar);
    if (type == "int")
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
    else if (type == "double")
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
            if (!typeOfValueIsSuitable(lines[columnN - 1], value, "The value you are looking for in the column is not of its type!"))
            {
                return;
            }
        }

        vector<string> values = splitLine(lines[columnN - 1], '|');
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
            for (int row = 0; row < positionsValue.size(); ++row)
            {
                tablePresentation(cells, sizesLongesWords, size, counter, positionsValue[row], countRows);
            }
        }
    }
    else
    {
        cout << "Invalid column index for the given table" << endl;
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
                outputFileStream << '|';
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
    Функция, която добавя нова колона в таблица с помощта на горната функция (appendNewColumnToTableFile).
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

        vector<string> line = splitLine(lines[0], '|');
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
                    outputFileStream << '|';
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

        vector<string> values = splitLine(lines[searchColumn - 1], '|');
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
        cout << "Invalid column index for the given table" << endl;
    }
}

/**
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла),
    пренаписва таблица, като преди това таблицата се зарежда в паметта и обработва с долната функция (deleteRowsInFile), 
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
            for (int k = i; k < cells.size(); k+=countColumns)
            {
                counter++;
                outputFileStream << cells[k];
                if (counter != countRows)
                {
                    outputFileStream << '|';
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
void deleteCommand(vector<Entry>& catalog, string tableName, int searchColumn, string searchValue)
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

        vector<string> values = splitLine(lines[searchColumn - 1], '|');
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
        cout << "Invalid column index for the given table!" << endl;
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
            outputFileStream << lines[i] << '|' << values[i] << endl;
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

void renameCommand(string filePath, vector<Entry>& catalog, string oldTableName, string newTableName)
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

int main()
{
    string filePath = "Catalog.txt";
    vector<Entry> catalog;
    vector<Entry> entries;
    loadCatalog(filePath, catalog);

    cout << "Enter the operation you want to perform." << endl;
    cout << "The options regarding working with files are: close, save, save as, help and exit." << endl;
    cout << "Database options: import, showtables, describe, print, export, select, addcolumn, update, delete, insert, rename." << endl;

    string command;
    while (1)
    {
        cout << "Command: ";
        getline(cin, command);
        vector<string> splittedCommand = splitLine(command, ' ');

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
        else if (splittedCommand[0] == "describe" && splittedCommand.size() == 1)
        {
            cout << "You forgot to enter the table name!" << endl;
        }
        else if (splittedCommand[0] == "print" && splittedCommand.size() == 2)
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
        else if (splittedCommand[0] == "print" && splittedCommand.size() == 1)
        {
            cout << "You forgot to enter the table name!" << endl;
        }
        else if (splittedCommand[0] == "export" && splittedCommand.size() == 3)
        {
            string tableName = splittedCommand[1];
            if (checkIfTableExists(catalog, tableName))
            {
                string commandPath = splittedCommand[2];
                exportCommand(catalog, tableName, commandPath);
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
        else if (splittedCommand[0] == "export" && splittedCommand.size() < 3)
        {
            cout << "You forgot to enter the name of the table or the path where you want to save it!" << endl;
        }
        else if (splittedCommand[0] == "import" || splittedCommand[0] == "export" || splittedCommand[0] == "showtables"
            || splittedCommand[0] == "describe" || splittedCommand[0] == "print")
        {
            cout << "You have entered more parameters than required of you!" << endl;
        }

        if (splittedCommand[0] == "select" && splittedCommand.size() == 4)
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
                cout << "Invalid column entry!" << endl;
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
        else if (splittedCommand[0] == "select" && splittedCommand.size() < 4 )
        {
            cout << ERROR_FORGOTTEN_PARAMETERS << endl;
        }
        else if (splittedCommand[0] == "addcolumn" && splittedCommand.size() == 4)
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
        else if (splittedCommand[0] == "addcolumn" && splittedCommand.size() < 4)
        {
            cout << ERROR_FORGOTTEN_PARAMETERS << endl;
        }
        if (splittedCommand[0] == "update" && splittedCommand.size() == 6)
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
                cout << "Invalid column entry!" << endl;
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
        else if (splittedCommand[0] == "update" && splittedCommand.size() < 6)
        {
            cout << ERROR_FORGOTTEN_PARAMETERS << endl;
        }
        else if (splittedCommand[0] == "delete" && splittedCommand.size() == 4)
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
                cout << "Invalid column entry!" << endl;
            }
            if (everythingIsFine)
            {
                string tableName = splittedCommand[1];
                if (checkIfTableExists(catalog, tableName))
                {
                    string searchValue = splittedCommand[3];
                    deleteCommand(catalog, tableName, searchColumn, searchValue);
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
        else if (splittedCommand[0] == "rename" && splittedCommand.size() == 3)
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
                    renameCommand(filePath, catalog, oldTableName, newTableName);
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
        else if (splittedCommand[0] == "rename" && splittedCommand.size() < 3)
        {
            cout << ERROR_FORGOTTEN_PARAMETERS << endl;
        }





        if (splittedCommand[0] == "save")
        {
            completeCatalogFile(filePath, entries);
            entries.clear();
            catalog.clear();
            loadCatalog(filePath, catalog);
        }
    }
}