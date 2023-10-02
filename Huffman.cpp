#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

int main()
{
    int menuFlag = 0;
    cout << "Закодировать файл - 1." << endl
         << "Раскодировать файл - 2." << endl
         << "Введите команду: ";
    cin >> menuFlag;

    if (menuFlag == 1)
    {
        ifstream readbleFile("text.txt", ifstream::binary); // открывает файл для чтения в бинарном формате
        if (!readbleFile)// если не удалось открыть файл, то ошибка
        {
            cout << "Не удалось открыть файл для чтения" << endl;
            return 1;
        } 

        map<string, int> frequencyOfSymbols; // создаем словарь для подсчета частоты символов

        char ch;
        while (readbleFile.get(ch)) //подсчитываем частоту символов в while
        {
            std::string key;

            if (ch == '\n') //в виндоус есть два специальных символа для переноса строки - это 13(/r) и \n 
            {
                key = "cr";
            }
            else if (ch == 13) //в виндоус есть два специальных символа для переноса строки - это 13(/r) и \n 
            {
                key = "cr";
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
            frequencyOfSymbols[key] += 1;
        }
        frequencyOfSymbols["cr"] /= 2; //делю перенос строки на 2, т.к. на виндоус в файле для преноса строки используется два спецаильных символа - это 13(/r) и \n

        for (const auto &pair : frequencyOfSymbols)
        {
            // pair.first - ключ
            // pair.second - значение
            cout << "Key: " <<pair.first << ", Value: " << pair.second << std::endl;
        }
    }
    else if (menuFlag == 2)
    {
    }
    else
        cout << "Неизвестная команда" << endl;

    return 0;
}