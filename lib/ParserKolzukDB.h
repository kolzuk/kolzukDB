#include "data/KeyWords.h"

#include <set>
#include <string>
#include <queue>
#include <regex>

class ParserKolzukDB {
public:
    void Parse(const std::string& statement);
    std::string NextWord();
    std::string ParseName();
    std::string ParseValue();
    std::string ParseType();
    static std::string ValueType(const std::string& value);
    bool HasNextWord();
    bool Take(const std::string& word);
    void Expect(const std::string& word);
    static bool IsCorrectName(const std::string& name);
    static bool IsCorrectValue(const std::string& value);
    static bool IsCorrectSymbol(char symbol);
    static bool IsBool(const std::string& value);
    static bool IsInt(const std::string& value);
    static bool IsDouble(const std::string& value);
    static bool IsVarchar(const std::string& value);
private:
    std::queue<std::string> statement_words;
};