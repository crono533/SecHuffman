#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include <sstream>
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
    //cout << "Inter" << endl;
    ifstream readbleFile("text.txt"); // открывает файл для чтения в бинарном формате
    if (!readbleFile)                                   // если не удалось открыть файл, то ошибка
    {
        cout << "Не удалось открыть файл для чтения" << endl;
        return 0;
    }

    char ch;
    string key;
    while (readbleFile.get(ch)) // подсчитываем частоту символов в while
    {
        //cout << "Ch " << ch << endl;
        // Используйем оператор [] для увеличения счетчика
        key = string(1,ch);
        //cout <<"Key " <<key<< endl;
        myMap[key] += 1;
    }
    
    readbleFile.close();

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

// int write_table_in_file(list<pair<int, string>> listToWrite)
// {
//     ofstream outputFile("codetext.txt");
//     if (!outputFile.is_open())
//     {
//         cout << "Не удалось открыть файл для записи" << endl;
//         return 0;
//     }

//     for (const auto &pair : listToWrite)
//     {
//         outputFile << pair.second << " " << pair.first << " ";
//     }
//     outputFile << "TAB " << '\n';

//     // Закрыть файл после записи
//     outputFile.close();
//     return 1;
// }

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
        //хай
        // Добавляем новый узел в список
        listOfFreq.push_back(make_pair(parentNode.freq, parentNode));
    }
    // В listOfFreq останется только один элемент - корневой узел дерева Хаффмана
}

bool fill_codetext_file(map<string, string> huffmanCodes, list<pair<int, string>> listToWrite)
{
    ofstream outputFile("codetext.txt", ios::binary);
    if (!outputFile.is_open()) {
        cout << "Не удалось открыть файл для записи" << endl;
        return false;
    }

    int sizeOfList = listToWrite.size();
    outputFile.write(reinterpret_cast<const char*>(&sizeOfList), sizeof(sizeOfList)); // Записываем размер списка

    //cout << "173" << endl;

    for (const auto &pair : listToWrite) {
        int first = pair.first;
        const string& second = pair.second;

        outputFile.write(second.c_str(), second.size()); // Записываем string как последовательность символов
        outputFile.write(reinterpret_cast<const char*>(&first), sizeof(first)); // Записываем int
        
    }

    ifstream readbleFile("text.txt", ifstream::binary); // открывает файл для чтения в бинарном формате
    if (!readbleFile)                                   // если не удалось открыть файл, то ошибка
    {
        cout << "Не удалось открыть файл для чтения" << endl;
        return false;
    }

    char ch;
    string key;
    map<string, string>::iterator it;
    string message;

        // for (const auto &pair : huffmanCodes)
        // {
        //     if(pair.first == "\n")  
        //         cout << "FIND";
        // }
    
    while (readbleFile.get(ch)) // подсчитываем частоту символов в while
    {
            key = string(1, ch);
            // cout << "ch "<<(int)ch << endl; 
            // cout << "find " << key << endl;
            for (const auto &pair : huffmanCodes)
            {
                if (pair.first == key || pair.first == "\r"){
                    message += pair.second;
                    //continue;
                    }
            }
            //it = huffmanCodes.find(key);
            //message += it->second;
    }
    //outputFile << '\n';
    // cout << message;
    // cout << endl << "размер " << message.size() << endl;
    // fileToWriteCodes << message.size();
    // fileToWriteCodes << '\n';

    int byte_count = message.size() / 8; // 1 byte = 8 bits
    int modulo = message.size() % 8;

    // cout << "modulo - " << modulo << endl;


        int i = 0;
        for (; i < byte_count; ++i)
        {
            bitset<8> b(message.substr(i*8, 8));
            //cout << b <<endl;
            char value = static_cast<char>(b.to_ulong());
            //cout << "gg " << value << endl;
            outputFile.write(reinterpret_cast<char*>(&value), sizeof value);
        }

if (modulo > 0)
{
    bitset<8> b(message.substr(i * 8, modulo) + string(8 - modulo, '0')); // Дополните нулями до 8 бит
    char value = static_cast<char>(b.to_ulong());
    outputFile.write(reinterpret_cast<char*>(&value), sizeof value);
}
        outputFile.close();
        readbleFile.close();
    
    // else
    // {
    //     int i = 0;
    //     for (; i < byte_count; ++i)
    //     {
    //         bitset<8> b(message.substr(i , 8));
    //         char value = b.to_ulong();
    //         cout << "gg " << value << endl;
    //         fileToWriteCodes.write(reinterpret_cast<char *>(&value), sizeof value);
    //     }

    // }
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

int string_to_int(const std::string &str)
{
    int result;
    std::istringstream iss(str);

    if (!(iss >> result))
    {
        // не удалось преобразовать строку в int
        std::cerr << "Ошибка преобразования строки в int." << std::endl;
    }
    return result;
}

int parse_table_from_file(list<pair<int, string>> &listOfFreq, ifstream& table)
{
    // ifstream table("codetext.txt", ifstream::binary);
    // string line;
    // int countOfSymbols = 0;
    // bool inTable = false; // Флаг, указывающий, что мы находимся в таблице

    // while (getline(table, line))
    // {
    //     istringstream iss(line);
    //     string symbol;
    //     int key;

    //     while (iss >> symbol >> key)
    //     {
    //         listOfFreq.push_back(make_pair(key, symbol));
    //         countOfSymbols += key;
    //     }
    // }
    // return countOfSymbols;
    int key, q;
    unsigned long long len=0;
    table.read((char*)&q,sizeof(q));
    char s;
    // cout << "TEST IN PARSE Q" << q << endl;
    for(int i=0;i<q;i++){
        table.read((char*)&s, sizeof(s)); //читаем символ
        //cout << "TEST IN PARSE 1 "<< endl;
        table.read((char*)&key,sizeof(key)); //читаем кол-во
        listOfFreq.push_back(make_pair(key,string(1,s)));
        len+=key;
    }
    return len;
}

void decode_huffman_message(map<string, string> &huffmanCodes, int countOfSymbols, ifstream& readbleFile)
{
    //ifstream readbleFile("codetext.txt", ifstream::binary);
    ofstream writableFile("decode.txt");
    string line;
    cout << countOfSymbols << endl;
    // while (getline(readbleFile, line)) //пропускаем таблицу
    // {
    //     if (line.find("TAB") != string::npos)
    //     {
    //         break;
    //     }
    // }

    char buffer;
    char bit;
    // getline(readbleFile,line);
    // string count_of_bits = line;
    //int number_of_bits = stoi(count_of_bits);
    //cout << "how many bits? - " << number_of_bits << endl;
    std::bitset<8> bitset; // Буфер для чтения 8 бит
    line.clear();

    while (readbleFile.get(buffer)) {
        bitset = std::bitset<8>(buffer); // Преобразовать байт в биты
        for (int i = 7; i >= 0; i--) {
            bit = bitset[i] ? '1' : '0';
            line += bit;
            for(const auto &pair : huffmanCodes)
            {
                if(pair.second == line)
                {
                    // cout << line;
                    //cout << pair.first <<" " << pair.second << endl;
                    if(pair.first == "sp"){writableFile <<" ";}
                    else if(pair.first == "cr"){writableFile << '\n';}
                    else {writableFile << pair.first;}
                    line.clear();
                    countOfSymbols--;
                    if(countOfSymbols == 0)
                        return;
                }
            }
        }
    }
}

// void decode_huffman_message(map<string, string> &huffmanCodes, int countOfSymbols, int number_of_bits)
// {
//     ifstream readbleFile("codetext.txt", ifstream::binary);
//     ofstream writableFile("decode.txt");
//     string line;

//     while (getline(readbleFile, line))
//     {
//         if (line.find("TAB") != string::npos)
//         {
//             break;
//         }
//     }

//     char buffer;
//     char bit;
//     getline(readbleFile, line);
//     string count_of_bits = line;
//     int remaining_bits = number_of_bits;  // Оставшиеся биты для декодирования
//     std::bitset<8> bitset; // Буфер для чтения 8 бит
//     line.clear();

//     while (readbleFile.get(buffer) && remaining_bits > 0)
//     {
//         bitset = std::bitset<8>(buffer); // Преобразовать байт в биты
//         for (int i = 7; i >= 0 && remaining_bits > 0; i--)
//         {
//             bit = bitset[i] ? '1' : '0';
//             line += bit;
//             for (const auto &pair : huffmanCodes)
//             {
//                 if (pair.second == line)
//                 {
//                     cout << line;
//                     if (pair.first == "sp") { writableFile << " "; }
//                     else if (pair.first == "cr") { writableFile << '\n'; }
//                     else { writableFile << pair.first; }
//                     line.clear();
//                     remaining_bits - stoi(pair.second);
//                 }
//             }
//         }
//     }
// }



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

        // for (const auto &pair : frequencyOfSymbols)
        // {
        //     cout << "sim: " << pair.first << ", freq " << pair.second << endl;
        // }


        sort_list_of_freq_not_node(sortedFrequencyOfSymbolsList);

        // if (!write_table_in_file(sortedFrequencyOfSymbolsList))
        //     return 1;

        list<pair<int, TreeNode>> myList;
        create_list_of_freq(myList, sortedFrequencyOfSymbolsList);

        build_huffman_tree(myList);

        // for (const auto &pair : myList)
        // {
        //     cout << "freq: " << pair.first << ", Huffman Code: " << pair.second.symbol << endl;
        // }

        map<string, string> huffmanCodes;
        TreeNode *root = &myList.front().second; // Получаем адрес корневого узла из списка
        generate_huffman_codes(root, "", huffmanCodes);

        // for (const auto &pair : huffmanCodes)
        // {
        //     cout << "Symbol: " << pair.first << ", Huffman Code: " << pair.second << endl;
        // }
        if(sortedFrequencyOfSymbolsList.size() == 1)
        {
            for( auto &pair : huffmanCodes)
            {
                pair.second = "0";
            }
        }
        // for (const auto &pair : huffmanCodes)
        // {
        //     cout << "Symbol: " << pair.first << ", Huffman Code: " << pair.second << endl;
        // }

        fill_codetext_file(huffmanCodes, sortedFrequencyOfSymbolsList);
        cout << "Успешно сжато";
    }
    else if (menuFlag == 2)
    {
        ifstream readbleFile("codetext.txt", ios::binary); // открывает файл для чтения в бинарном формате
        if (!readbleFile)                                   // если не удалось открыть файл, то ошибка
        {
            cout << "Не удалось открыть файл для чтения" << endl;
            return 0;
        }

    // char ch;
    // string key;
    // while (readbleFile.get(ch)) // подсчитываем частоту символов в while
    // {
    //     cout << "Ch " << ch << endl;
    // }
    
    // readbleFile.close();


        int countOfSymbols = 0;
        list<pair<int, string>> sortedFrequencyOfSymbolsList;
        countOfSymbols = parse_table_from_file(sortedFrequencyOfSymbolsList, readbleFile);
        //cout << "TEST" << endl;
        //sort_list_of_freq_not_node(sortedFrequencyOfSymbolsList);

        // for (const auto &pair : sortedFrequencyOfSymbolsList)
        // {
        //     cout << "Key: " << pair.first << ", Symb: " << pair.second << endl;
        // }

        list<pair<int, TreeNode>> myList;
        create_list_of_freq(myList, sortedFrequencyOfSymbolsList);
        // cout << "Лист частот" <<endl;
        // for (const auto &pair : myList)
        // {
        //     cout << "Key: " << pair.first << ", Symb: " << pair.second.symbol << endl;
        // }

        build_huffman_tree(myList);
        // cout << "Коды хаффмана" << endl;


        map<string, string> huffmanCodes;
        TreeNode *root = &myList.front().second; // Получаем адрес корневого узла из списка
        generate_huffman_codes(root, "", huffmanCodes);
        // for (const auto &pair : huffmanCodes)
        // {
        //     cout << "Symbol: " << pair.first << ", Huffman Code: " << pair.second << endl;
        // }

        if(sortedFrequencyOfSymbolsList.size() == 1)
        {
            for( auto &pair : huffmanCodes)
            {
                pair.second = "0";
            }
        }
        // for (const auto &pair : huffmanCodes)
        // {
        //     cout << "Symbol: " << pair.first << ", Huffman Code: " << pair.second << endl;
        // }

        //decode_huffman_message(huffmanCodes, countOfSymbols, remaining_bits);
        decode_huffman_message(huffmanCodes, countOfSymbols, readbleFile);
        cout << "Успешно распаковано";

    }
    else
        cout << "Неизвестная команда" << endl;

    return 0;
}