#include "Functions.h"

const char SEPARATOR_USER_INPUT = ';';                                          ///От потребителя се изисква като разделител да използва точка и запетая, когато въвежда своите команди.

int main()
{
    string filePath = CATALOG_PATH;
    vector<Entry> catalog;
    vector<Entry> entries;
    loadCatalog(filePath, catalog);

    cout << INTRODUCTION << endl;
    cout << FILE_OPTIONS << endl;
    cout << DATABASE_OPTIONS << endl;
    cout << TABLE_OPTIONS << endl;
    cout << USE_SEMICOLON << endl;

    string command;
    while (1)
    {
        cout << ENTER_COMMAND;
        getline(cin, command);
        vector<string> splittedCommand = splitLine(command, SEPARATOR_USER_INPUT);

        commandPattern(filePath, splittedCommand, catalog, entries);
    }
}