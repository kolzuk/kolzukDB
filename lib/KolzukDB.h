#include "Table.h"

#include <filesystem>
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include <variant>
#include <map>
#include <cassert>

class KolzukDB {
public:
    std::string Execute(const std::string& statement);
    void WriteInFile(const std::filesystem::path& path);
    void ReadFromFile(const std::filesystem::path& path);
protected:
    void CreateTable();
    void DropTable();
    void Insert();
    std::string Select();
    void Delete();
    void Update();
    Table InnerJoin(const std::string& first_name);
    Table LeftJoin(const std::string& first_name);
    Table RightJoin(const std::string& first_name);
private:
    ParserKolzukDB parser;
    std::map<std::string, Table> tables;
    std::vector<std::string> request;
};