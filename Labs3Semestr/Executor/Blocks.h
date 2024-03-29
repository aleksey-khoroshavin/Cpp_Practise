#ifndef EXECUTOR_BLOCKS_H
#define EXECUTOR_BLOCKS_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <stdexcept>
#include <algorithm>

enum Type_block
{
    in,//только чтение
    out,//только запись
    in_out//чтение и запись
};

//Базовый интерфейс для блока (определенные нами блоки являются его наследниками)
class IBlock
{
public:
    virtual std::vector<std::string> Do_command(std::vector<std::string>& some_text) = 0;
    virtual Type_block Return_type_of_block() = 0;
    virtual ~IBlock() = default;
};

//Класс интерфейс для создателей блоков
class IBlockMaker
{
public:
    virtual IBlock* Create(std::list<std::string>& block_description) = 0;
    virtual ~IBlockMaker() = default;
};

//Класс-синглтон, который реализует фабрику по созданию блоков
class BlockFactory
{
public:
    void RegisterMaker(const std::string& key, IBlockMaker * maker);
    static BlockFactory& Instance();//Создает один объект класса и возвращает ссылку на него
    IBlock* Create(std::list<std::string>& block_description);

private:
    std::map<std::string, IBlockMaker*> makers;
};

//Шаблонный класс, который будет создавать блок с конкретным именем
template <typename T>
class BlockMaker : public IBlockMaker
{
public:
    BlockMaker(const std::string& key)
    {
        BlockFactory::Instance().RegisterMaker(key, this);
    }
    IBlock* Create(std::list<std::string>& block_description) override
    {
        return new T(block_description);
    }
};

//Блок READFILE
class Readfile : public IBlock {
public:
    Readfile(std::list<std::string>& arguments);
    std::vector<std::string> Do_command(std::vector<std::string>& some_text) override;
    Type_block Return_type_of_block() override;
private:
    std::string in_name;//Содержит имя файла, который мы подаем в наш блок из списка аргументов
    std::ifstream input_file;//Файл для чтения и получения из него текста
    std::vector<std::string> text;//Текст который мы считаем из нашего файла
};

//Блок WRITEFILE
class Writefile : public IBlock{
public:
    Writefile(std::list<std::string>& arguments);
    std::vector<std::string> Do_command(std::vector<std::string>& some_text) override;
    Type_block Return_type_of_block() override;
private:
    std::string out_name;
    std::ofstream output_file;
    std::vector<std::string> text;
};

//Блок GREP
class Grep : public IBlock{
public:
    Grep(std::list<std::string>& arguments);
    std::vector<std::string> Do_command(std::vector<std::string>& some_text) override;
    Type_block Return_type_of_block() override;
private:
    std::string argument_grep;
    std::vector<std::string> text;
};

//Блок REPLACE
class Replace: public IBlock{
public:
    Replace(std::list<std::string>& arguments);
    std::vector<std::string> Do_command(std::vector<std::string>& some_text) override;
    Type_block Return_type_of_block() override;
private:
    std::string first_replace_arg;
    std::string second_replace_arg;
    std::vector<std::string> text;
};

//Блок SORT
class Sort: public IBlock{
public:
    Sort(std::list<std::string>& arguments);
    std::vector<std::string> Do_command(std::vector<std::string>& some_text) override;
    Type_block Return_type_of_block() override;
private:
    std::vector<std::string> text;
};

//Блок DUMP
class Dump: public IBlock{
public:
    Dump(std::list<std::string>& arguments);
    std::vector<std::string> Do_command(std::vector<std::string>& some_text) override;
    Type_block Return_type_of_block() override;
private:
    std::string log_name;
    std::ofstream log_file;
    std::vector<std::string> text;
};
#endif
