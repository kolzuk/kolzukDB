#include "ParserKolzukDB.h"

#include <sstream>
#include <iostream>

void ParserKolzukDB::Parse(const std::string& source) {
    std::istringstream ss(source);

    std::string word;

    bool is_WHERE = false;

    while (ss >> word) {
        if (!is_WHERE) {
            word.erase(std::remove(word.begin(), word.end(), '('), word.end());
            word.erase(std::remove(word.begin(), word.end(), ')'), word.end());
        }

        word.erase(std::remove(word.begin(), word.end(), ','), word.end());
        word.erase(std::remove(word.begin(), word.end(), ';'), word.end());

        if (word.empty()) {
            continue;
        }

        std::string word_in_upper_key = word;
        std::transform(word_in_upper_key.begin(), word_in_upper_key.end(), word_in_upper_key.begin(), toupper);

        if (kKeyWords.contains(word_in_upper_key)) {
            word = word_in_upper_key;
        }

        statement_words.push(word);

        if (word == "WHERE") {
            is_WHERE = true;
        }
    }
}

std::string ParserKolzukDB::ParseName() {
    if (!HasNextWord()) {
        throw std::logic_error("Syntax Error");
    }

    std::string column_name = NextWord();

    if (!IsCorrectName(column_name)) {
        throw std::logic_error("Incorrect column name");
    }

    return column_name;
}

std::string ParserKolzukDB::ParseValue() {
    if (!HasNextWord()) {
        throw std::logic_error("Incorrect values count");
    }
    std::string value = NextWord();
    if (!IsCorrectValue(value)) {
        throw std::logic_error("Wrong type of value: '" + value + "'");
    }
    return value;
}

std::string ParserKolzukDB::ParseType() {
    if (Take("BOOL")) {
        return "BOOl";
    } else if (Take("INT")) {
        return "INT";
    } else if (Take("FLOAT")) {
        return "FLOAT";
    } else if (Take("DOUBLE")) {
        return "DOUBLE";
    } else if (Take("VARCHAR")) {
        return "VARCHAR";
    } else {
        throw std::logic_error("Incorrect type of value");
    }
}

std::string ParserKolzukDB::ValueType(const std::string& value) {
    if (IsBool(value)) {
        return "BOOL";
    } else if (IsInt(value)) {
        return "INT";
    } else if (IsDouble(value)) {
        return "DOUBLE";
    } else if (IsVarchar(value)) {
        return "VARCHAR";
    }
    throw std::logic_error("Incorrect type of value");
}

std::string ParserKolzukDB::NextWord() {
    std::string word = statement_words.front();
    statement_words.pop();
    return word;
}

bool ParserKolzukDB::HasNextWord() {
    return (!statement_words.empty());
}

bool ParserKolzukDB::IsCorrectName(const std::string& name) {
    if (std::all_of(name.begin(), name.end(), IsCorrectSymbol)) {
        return true;
    }
    return false;
}

bool ParserKolzukDB::IsCorrectValue(const std::string& value) {
    return (IsBool(value) || IsInt(value) || IsDouble(value) || IsVarchar(value));
}

bool ParserKolzukDB::IsCorrectSymbol(char symbol) {
    if (symbol >= '0' && symbol <= '9' ||
        symbol >= 'a' && symbol <= 'z' ||
        symbol >= 'A' && symbol <= 'Z') {
        return true;
    }
    return false;
}

bool ParserKolzukDB::Take(const std::string& word) {
    if (HasNextWord() && statement_words.front() == word) {
        NextWord();
        return true;
    }
    return false;
}

void ParserKolzukDB::Expect(const std::string& word) {
    if (!Take(word)) {
        throw std::logic_error("Expected: '" + word + "', found: '" + NextWord() + "'");
    }
}

bool ParserKolzukDB::IsBool(const std::string& value) {
    return (value == "TRUE" || value == "FALSE");
}

bool ParserKolzukDB::IsInt(const std::string& value) {
    std::regex e("([-+]?[1-9][0-9]*)|0");
    return (std::regex_match(value, e));
}

bool ParserKolzukDB::IsDouble(const std::string& value) {
    std::regex e("[-+]?([0-9]+\\.[0-9]+|[1-9][0-9]*)");
    return (std::regex_match(value, e));
}

bool ParserKolzukDB::IsVarchar(const std::string& value) {
    return (value.front() == '"' && value.back() == '"');
}



