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
    string key;
    while (readbleFile.get(ch)) // подсчитываем частоту символов в while
    {

        if (ch == '\n' || ch == 13)
        {
            key = "cr"; // Используем один ключ для переноса строки
        }
        else if (ch == ' ')
        {
            key = "sp";
        }
        else
        {
            key = string(1, ch);
        }
        // cout << "key:" << key << endl;

        // Используйем оператор [] для увеличения счетчика
        myMap[key] += 1;
    }
    key = "cr";
    map<string, int>::iterator it = myMap.find(key);

    if (it->first == key)
    {
        myMap["cr"] /= 2;
    }

    return 1;
}

list<pair<int, string>> sort_frequency_map(map<string, int> mapToSort)
{
    list<pair<int, string>> sortedList;

    for (const auto &pair : mapToSort)
    {
        sortedList.push_back(make_pair(pair.second, pair.first));
    }

    return sortedList;
}

int write_table_in_file(list<pair<int, string>> listToWrite)
{
    ofstream outputFile("codetext.txt");
    if (!outputFile.is_open())
    {
        cout << "Не удалось открыть файл для записи" << endl;
        return 0;
    }

    for (const auto &pair : listToWrite)
    {
        outputFile << pair.second << " " << pair.first << " ";
    }
    outputFile << "TAB " << '\n';
    return 1;
}

void create_list_of_freq(list<pair<int, TreeNode>> &listOfFreq, list<pair<int, string>> &listToParse)
{
    for (const auto &pair : listToParse)
    {
        int freq = pair.first;
        string symbol = pair.second;

        // Создаем TreeNode для текущей пары
        TreeNode node;
        node.freq = freq;
        node.symbol = symbol;

        // Добавляем пару (freq, TreeNode) в список
        listOfFreq.push_back(make_pair(freq, node)); // можно добавлять в конец так как сразу будет отсортирован
    }
}

bool compareElements(const pair<int, TreeNode> &a, const pair<int, TreeNode> &b)
{
    if (a.first < b.first)
    {
        return true;
    }
    else if (a.first > b.first)
    {
        return false;
    }
    // Если int равны, то переходим к следующей паре
    return true;
}
bool compareElements_not_node(const pair<int, string> &a, const pair<int, string> &b)
{
    if (a.first < b.first)
    {
        return true;
    }
    else if (a.first > b.first)
    {
        return false;
    }
    // Если int равны, то переходим к следующей паре
    return true;
}

void sort_list_of_freq(list<pair<int, TreeNode>> &listOfFreq)
{
    listOfFreq.sort(compareElements);
}

void sort_list_of_freq_not_node(list<pair<int, string>> &listOfFreq)
{
    listOfFreq.sort(compareElements_not_node);
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

void generate_huffman_codes(TreeNode *rootNode, string code, map<string, string> &huffmanCodes)
{
    if (rootNode == nullptr)
        return;

    if (rootNode->leftChild == nullptr && rootNode->rightChild == nullptr)
    {
        // Этот узел представляет символ, сохраняем его код
        huffmanCodes[rootNode->symbol] = code;
        return;
    }

    // Рекурсивно спускаемся по левой и правой ветви
    generate_huffman_codes(rootNode->leftChild, code + "0", huffmanCodes);
    generate_huffman_codes(rootNode->rightChild, code + "1", huffmanCodes);
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
        list<pair<int, string>> sortedFrequencyOfSymbolsList;

        if (!create_frequency_map(frequencyOfSymbols))
        {
            cout << "Не удалось создать список частот";
            return 1;
        }

        sortedFrequencyOfSymbolsList = sort_frequency_map(frequencyOfSymbols);

        sort_list_of_freq_not_node(sortedFrequencyOfSymbolsList);

        if (!write_table_in_file(sortedFrequencyOfSymbolsList))
            return 1;

        list<pair<int, TreeNode>> myList;
        create_list_of_freq(myList, sortedFrequencyOfSymbolsList);

        build_huffman_tree(myList);

        for (const auto &pair : myList)
        {
            cout << "Freq: " << pair.first << ", Symbol: " << pair.second.symbol << ", FreqInNode: " << pair.second.freq << endl;
        }

        map<string, string> huffmanCodes;
        TreeNode *root = &myList.front().second; // Получаем адрес корневого узла из списка
        generate_huffman_codes(root, "", huffmanCodes);

        for (const auto &pair : huffmanCodes)
        {
            cout << "Symbol: " << pair.first << ", Huffman Code: " << pair.second << endl;
        }
    }
    else if (menuFlag == 2)
    {
    }
    else
        cout << "Неизвестная команда" << endl;

    return 0;
}