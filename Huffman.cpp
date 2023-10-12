#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>

using namespace std;

class TreeNode
{
public:
    int freq;
    string code;
    string symbol;

    TreeNode *leftChild;
    TreeNode *rightChild;

    TreeNode() : leftChild(nullptr), rightChild(nullptr), freq(0) {}
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

        sortedMap[tmpMinKey] = tmpMinStr;
        mapToSort.erase(tmpMinStr);
    }
    return sortedMap;
}

int write_table_in_file(map<int, string> mapToWrite)
{
    ofstream outputFile("codetext.txt");
    if (!outputFile.is_open())
    {
        cout << "Не удалось открыть файл для записи" << endl;
        return 0;
    }

    for (const auto &pair : mapToWrite)
    {
        outputFile << pair.second << " " << pair.first << " ";
    }
    outputFile << "TAB " << '\n';
    return 1;
}

void create_list_of_freq(list<pair<int, TreeNode>> &listOfFreq, map<int, string> &mapToParse)
{
    for (const auto &pair : mapToParse)
    {
        int freq = pair.first;
        string symbol = pair.second;

        // Создаем TreeNode для текущей пары
        TreeNode node;
        node.freq = freq;
        node.symbol = symbol;

        // Добавляем пару (freq, TreeNode) в список
        listOfFreq.push_back(make_pair(freq, node)); //можно добавлять в конец так как сразу будет отсортирован
    }
}

bool compareElements(const pair<int, TreeNode>& a, const pair<int, TreeNode>& b) {
    if (a.first < b.first) {
        return true;
    } else if (a.first > b.first) {
        return false;
    }
    // Если int равны, то переходим к следующей паре
    return true;
}

void sort_list_of_freq(list<pair<int, TreeNode>> &listOfFreq)
{
    listOfFreq.sort(compareElements);
}

void build_huffman_tree(list<pair<int, TreeNode>> &listOfFreq)
{
    while (listOfFreq.size() > 1)
    {
        // Сортируем лист по частотам
        sort_list_of_freq(listOfFreq);

        // Получаем два узла с наименьшими частотами
        TreeNode leftNode = listOfFreq.front().second;
        listOfFreq.pop_front();
        TreeNode rightNode = listOfFreq.front().second;
        listOfFreq.pop_front();

        // Создаем новый узел с суммой частот
        TreeNode parentNode;
        parentNode.freq = leftNode.freq + rightNode.freq;
        parentNode.symbol = leftNode.symbol + rightNode.symbol;
        parentNode.leftChild = new TreeNode(leftNode);
        parentNode.rightChild = new TreeNode(rightNode);

        // Добавляем новый узел в список
        listOfFreq.push_back(make_pair(parentNode.freq, parentNode));
    }
    // В listOfFreq останется только один элемент - корневой узел дерева Хаффмана
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
        map<int, string> sortedFrequencyOfSymbolsMap;

        if (!create_frequency_map(frequencyOfSymbols))
        {
            cout << "Не удалось создать список частот";
            return 1;
        }

        sortedFrequencyOfSymbolsMap = sort_frequency_map(frequencyOfSymbols);

        if (!write_table_in_file(sortedFrequencyOfSymbolsMap))
            return 1;

        list<pair<int, TreeNode>> myList;
        create_list_of_freq(myList, sortedFrequencyOfSymbolsMap);

        for (const auto &pair : myList)
        {
            cout << "Freq: " << pair.first << ", Symbol: " << pair.second.symbol << ", FreqInNode: " << pair.second.freq << endl;
        }

        build_huffman_tree(myList);

        for (const auto &pair : myList)
        {
            cout << "Freq: " << pair.first << ", Symbol: " << pair.second.symbol << ", FreqInNode: " << pair.second.freq << endl;
        }

    }
    else if (menuFlag == 2)
    {
    }
    else
        cout << "Неизвестная команда" << endl;

    return 0;
}