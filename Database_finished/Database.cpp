#include "Functions.h"

const char SEPARATOR_USER_INPUT = '-';                                          ///От потребителя се изисква като разделител да използва тирето, когато въвежда своите команди.

int main()
{
    string filePath = CATALOG_PATH;
    vector<Entry> catalog;
    vector<Entry> entries;
    loadCatalog(filePath, catalog);

    cout << "Enter the operation you want to perform." << endl;
    cout << "The options regarding working with files are: close, save, saveas, help and exit." << endl;
    cout << "Database options: import, showtables, describe, print, export." << endl;
    cout << "The options for working with tables are: select, addcolumn, update, delete, insert, rename, count, aggregate." << endl;
    cout << "Use a dash (-) as a separator!" << endl;

    string command;
    while (1)
    {
        cout << "Command: ";
        getline(cin, command);
        vector<string> splittedCommand = splitLine(command, SEPARATOR_USER_INPUT);

        commandPattern(filePath, splittedCommand, catalog, entries);
    }
}