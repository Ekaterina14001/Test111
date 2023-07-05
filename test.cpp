#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <regex>
#include <thread>

//функция удаляет "пустые" символы
std::string changeStr(const std::string& str)
{
    if (str.empty())
        return std::string{};

    std::string newStr(str);
    std::size_t found = str.find_first_of(" 0");

    while (found != std::string::npos)
    {
        newStr.erase(found, 1);
        found = newStr.find_first_of(" 0");
    }
    return newStr;
}

//функция сравнивает две строки
bool equal(const std::string& str1,const std::string& str2)
{
    //если обе строки пустые, то они равны
    if (str1.empty() && str2.empty())
        return true;

    //проверка на присутсвие хоть одной буквы в номере
    if (str1.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos && !str1.empty())
        throw("wrong code");

    if (str2.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos && !str2.empty())
        throw("wrong code");

    //проверка на регистр букв
    if (str1.find_first_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos && !str1.empty())
        throw("wrong code");

    if (str2.find_first_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos && !str2.empty())
        throw("wrong code");

    //если одна строка пустая, а вторая нет, то не равны
    if (str1.empty() || str2.empty())
        return false;

    if (str1 == str2)
        return true;

    auto newStr1 = changeStr(str1);
    auto newStr2 = changeStr(str2);

    return newStr1 == newStr2;
}

//предикат для set
struct Less
{
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
        return changeStr(lhs) < changeStr(rhs);
    }
};

void treatment(const std::string nameOfFile)
{
    //проверка имени файла
    if (nameOfFile.find("in") == std::string::npos)
        throw("Invalid file name");

    //чтение файла
    std::ifstream fin(nameOfFile, std::ios_base::in);
    if (!fin.is_open())
        throw("File not opened"); 
    
    std::string str;
    std::set<std::string, Less> set;//обработка файла путем записи только уникальных значений
    
    while (!fin.eof())
    {
        std::getline(fin,str);
        set.insert(str);
    }
    fin.close();

    //запись в файл
    std::string outFile = std::regex_replace(nameOfFile, std::regex("in"), "out");
    
    std::ofstream out; 
    out.open(outFile, std::ios_base::out);
    if (out.is_open())
    {
        for (auto it = set.begin(); it != set.end(); it++)
        {
            out << *it << std::endl;
        }
    }
    out.close();
}

//функция запуска обработки двух файлов в двух потоках
void parallelThread()
{
    std::thread thread1(treatment,"1_in.txt");
    std::thread thread2(treatment,"2_in.txt");

    thread1.join();
    thread2.join();

    return;
}

int main()
{
    parallelThread();
}
