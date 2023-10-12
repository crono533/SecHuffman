#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>

using namespace std;

class TreeNode
{
public:
    map<int, string> leftMap;
    map<int, string> rightMap;
    TreeNode* leftChild;
    TreeNode* rightChild;

    TreeNode() : leftChild(nullptr), rightChild(nullptr) {}
};

int create_frequency_map(map<string, int> &myMap)
{
    ifstream readbleFile("text.txt", ifstream::binary); // открывает файл для чтения в бинарном формате
    if (!readbleFile)                                   // если не удалось открыть файл, то ошибка
    {
        cout << "Не удалось открыть файл для чтения" << endl;
        return 0;
    }

    char ch;
    while (readbleFile.get(ch)) // подсчитываем частоту символов в while
    {
        std::string key;

        if (ch == '\n' || ch == 13) // на виндоус в файле для преноса строки используется два спецаильных символа - это 13(/r) и \n
        {
            key = "cr"; // Используем один ключ для переноса строки
        }
        else if (ch == ' ')
        {
            key = "sp";
        }
        else
        {
            key = std::string(1, ch);
        }

        // Используйем оператор [] для увеличения счетчика
        myMap[key] += 1;
    }
    myMap["cr"] /= 2; // делю перенос строки на 2, т.к. на виндоус в файле для преноса строки используется два спецаильных символа - это 13(/r) и \n

    return 1;
}

map<int, string> sort_frequency_map(map<string, int> &mapToSort)
{
    string tmpMinStr;
    int tmpMinKey = 0;
    map<int, string> sortedMap;

    while (!mapToSort.empty())
    {
        tmpMinKey = 30000;
        for (auto it = mapToSort.begin(); it != mapToSort.end(); it++)
        {
            if (it->second <= tmpMinKey)
            {
                tmpMinStr = it->first;
                tmpMinKey = it->second;
            }
        }

        cout << tmpMinStr << " " << tmpMinKey << " penis" << endl;

        sortedMap[tmpMinKey] = tmpMinStr;
        for (const auto &pair : sortedMap)
        {
            cout << "SortedKey: " << pair.first << ", Value: " << pair.second << std::endl;
        }

        mapToSort.erase(tmpMinStr);
    }
    return sortedMap;
}

int write_table_in_file(map<int, string> mapToWrite)
{
    ofstream outputFile("codetext.txt");
    if(!outputFile.is_open())
    {
        cout << "Не удалось открыть файл для записи" << endl;
        return 0;
    }

    for(const auto &pair : mapToWrite)
    {
        outputFile << pair.second << " " << pair.first << " ";
    }
    outputFile << "TAB " << '\n';
    return 1;
}

int main()
{
    int menuFlag = 0;
    cout << "Закодировать файл - 1." << endl
         << "Раскодировать файл - 2." << endl
         << "Введите команду: ";
    cin >> menuFlag;

    if (menuFlag == 1)
    {
        map<string, int> frequencyOfSymbols;
        map<int, string> sortedFrequencyOfSymbols;

        if (!create_frequency_map(frequencyOfSymbols))
        {
            cout << "Не удалось создать список частот";
            return 1;
        }

        sortedFrequencyOfSymbols = sort_frequency_map(frequencyOfSymbols);

        if(!write_table_in_file(sortedFrequencyOfSymbols)) return 1;


        // for (const auto &pair : sortedFrequencyOfSymbols)
        // {
        //     // pair.first - ключ
        //     // pair.second - значение
        //     cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        // }

    }
    else if (menuFlag == 2)
    {
    }
    else
        cout << "Неизвестная команда" << endl;

    return 0;
}