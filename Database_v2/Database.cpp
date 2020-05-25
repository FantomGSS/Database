#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> 
#include <algorithm>
#include "Column.h"
#include "Entry.h"

const char DISTANCE_BETWEEN_COLUMNS = '\t';                                                 ///Разстоянието между колоните
const int TABLE_PRINTING_LIMITATION = 3;                                                    ///Колко реда от таблицата да се принтират с възможност за избор на следваща и предишна страница.

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
    if (filePath.find(">") >= 0 || filePath.find("<") >= 0 || filePath.find("/") >= 0
        || filePath.find("\\") >= 0 || filePath.find(":") >= 0 || filePath.find("|") >= 0)
    {
        cout << "You have entered an invalid path! Forbidden characters are: \\ / : < | >" << endl;
        validFilePath = false;
    }

    return validFilePath;
}

/**
    Функция, която проверява по дадено име на таблица, дали тя е заредена в паметта.
*/
bool checkIfTableIsLoaded(vector<Entry>& catalog, vector<Entry>& entries, string name)
{
    bool tableIsLoaded = false;
    for (int i = 0; i < catalog.size(); i++)
    {
        if (name == catalog[i].getName())
        {
            tableIsLoaded = true;
        }
    }

    for (int i = 0; i < entries.size(); i++)
    {
        if (name == entries[i].getName())
        {
            tableIsLoaded = true;
        }
    }

    return tableIsLoaded;
}

/**
    Функция, която по дадено име на таблица, извлича от базата данни пътя до файла на тази таблица.
*/
string findPathOfTable(vector<Entry>& catalog, vector<Entry>& entries, string name)
{
    string filePath;
    for (int i = 0; i < catalog.size() && filePath.empty(); i++)
    {
        if (catalog[i].getName() == name)
        {
            filePath = catalog[i].getPath();
        }
    }

    for (int i = 0; i < entries.size() && filePath.empty(); i++)
    {
        if (entries[i].getName() == name)
        {
            filePath = entries[i].getPath();
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

    if (inputFileStream.is_open()) {
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
    else if (!inputFileStream.good() && condition == "Catalog") {
        ofstream outputFileStream;
        outputFileStream.open(filePath, ios::out);
        outputFileStream.close();
        printSuccessCreateMessage(filePath);
    }
    else if (condition == "Table")
    {
        printReminderMessage();
    }
    else {
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
        vector<string> splitedLine = splitLine(lines[i], '|');

        for (int k = 0; k < splitedLine.size(); k++)
        {
            words.push_back(splitedLine[k]);
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

    if (inputFileStream.is_open())
    {
        fileExists = true;
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

    if (outputFileStream.is_open()) {
        for (int i = 0; i < entries.size(); i++)
        {
            outputFileStream << entries[i];
        }
        outputFileStream.close();
    }
    else {
        printErrorMessage(filePath);
    }
}

/**
    Функция, която по подаден символен низ (път във файловата система и името, и разширението на файла,
    представляващ таблица) създава двойка, състояща се от името на таблицата и пътя на файла.
*/
void import(string filePath, vector<Entry>& entries, vector<Entry>& catalog)
{
    string name;
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
                name = filePath;
            }
            else
            {
                name = filePath.substr(0, indexPoint);
            }
        }
        else
        {
            name = filePath.substr(indexLastSlash + 1);
            indexPoint = name.find_last_of(".");
            if (indexPoint != -1)
            {
                name = name.substr(0, indexPoint);
            }
        }
    }

    name[0] = toupper(name[0]);
    
    if (checkIfTableIsLoaded(catalog, entries, name))
    {
        cout << "You cannot load a table with a name that already exists in the database!" << endl;
    }
    else
    {
        Entry entry = Entry(name, filePath);
        entries.push_back(entry);

        cout << "The table has been successfully added to the catalog!" << endl;
    }
}

/**
    Функция, която показва заредените таблици в паметта, като се има предвид, 
    че новите вкарани таблици не се съдържат в каталога. За да се вкарат в каталога, 
    трябва да се извърши действието save, тоест да се запазят в него.
*/
void showTables(vector<Entry>& catalog, vector<Entry>& entries)
{
    if (catalog.size() == 0 && entries.size() == 0)
    {
        cout << "There are no loaded tables in the database!" << endl;
        return;
    }

    for (int i = 0; i < catalog.size(); i++)
    {
        if (i == catalog.size() - 1 && entries.size() == 0)
        {
            cout << catalog[i].getName() << endl;
        }
        else
        {
            cout << catalog[i].getName() << ", ";
        }
    }

    for (int i = 0; i < entries.size(); i++)
    {
        if (i == entries.size() - 1)
        {
            cout << entries[i].getName() << endl;
        }
        else
        {
            cout << entries[i].getName() << ", ";
        }
    }
}

/**
    Функция, която показва информация за типовете на колоните
    по подадено име на таблица.
*/
void describe(vector<Entry>& catalog, vector<Entry>& entries, string name)
{
    if (!checkIfTableIsLoaded(catalog, entries, name))
    {
        cout << "This table name does not exist in the database!" << endl;
    }
    else
    {
        vector<string> lines;
        string filePath = findPathOfTable(catalog, entries, name);
        
        getLinesFromFile(filePath, lines, "Table");
        vector<string> cells = splitLines(lines);

        int countColumns = lines.size();
        int countRows = cells.size() / countColumns;

        for (int i = 1; i < cells.size(); i+= countRows)
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
    Функция, която по дадено име на таблица, принтира нейното съдържание.
*/
void print(vector<Entry>& catalog, vector<Entry>& entries, string name)
{
    if (!checkIfTableIsLoaded(catalog, entries, name))
    {
        cout << "This table name does not exist in the database!" << endl;
    }
    else
    {
        vector<string> lines;
        string filePath = findPathOfTable(catalog, entries, name);

        getLinesFromFile(filePath, lines, "Table");
        vector<string> cells = splitLines(lines);

        int countColumns = lines.size();
        int countRows = cells.size() / countColumns;
        vector<int> sizesLongesWords = getSizesOfLongestWordsInColumns(cells, countColumns, countRows);

        cout << "The printing of a table is limited to three rows" << endl;
        cout << "The possible viewing options are: next page, previous page and exit." << endl;
        int counter = 0;
        int size = 0;
        for (int row = 0; row < countRows; ++row)
        {
            if ((row - 2) % TABLE_PRINTING_LIMITATION == 0 && row != 2)
            {
                cout << "";
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
    }
}

int main()
{
    string filePath = "Catalog.txt";
    vector<Entry> catalog;
    vector<Entry> entries;
    loadCatalog(filePath, catalog);
    print(catalog, entries, "Credit");

    //cout << "Enter the operation you want to perform." << endl;
    //cout << "The options regarding working with files are: close, save, save as, help and exit." << endl;
    //cout << "Database options: import, showtables, describe, print." << endl;
}