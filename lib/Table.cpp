#include "Table.h"

Table::Table(RowProperties& _row_properties)  {
    row_properties = _row_properties;
    width = row_properties.Size();
    height = 0;
    current_index = 0;
}

void Table::InsertRow(const std::vector<std::string>& column_names, const std::vector<std::string>& values) {
    rows[current_index] = Row(row_properties.Size());
    height++;

    for (size_t i = 0; i < column_names.size(); ++i) {
        size_t column_index = row_properties.GetColumnIndex(column_names[i]);
        rows.at(current_index)[column_index] = values[i];
    }


    for (size_t i = 0; i < width; ++i) {
        if (row_properties[i].is_not_null && rows.at(current_index)[i] == "NULL") {
            throw std::logic_error("Not add to not null column: '" + row_properties[i].name + "'");
        }
    }

    for (size_t i = 0; i < width; ++i) {
        if (row_properties[i].is_primary_key) {
            std::unordered_set<std::string> unique_values;
            for (int j = 0; j < height; ++j) {
                std::string value = (*this)[j][i];
                if (unique_values.contains(value)) {
                    throw std::logic_error("Not unique value: '" + value + "'");
                }
                unique_values.insert(value);
            }
        }
    }

    current_index++;
}

void Table::InsertRow(const Row& row) {
    rows[current_index] = row;
    current_index++;
    height++;
}

Table::Table() {
    width = 0;
    height = 0;
    current_index = 0;
}

bool Table::HasColumn(const std::string& column_name) {
    return row_properties.HasColumn(column_name);
}

std::string Table::GetColumnType(const std::string& column_name) {
    return row_properties.GetColumnType(column_name);
}

std::string Table::Select(const std::vector<std::string>& columns_name, const std::vector<std::string>& condition) {
    std::vector<std::vector<std::string>> table(columns_name.size());
    std::vector<size_t> max_sizes(columns_name.size(), 0);

    for (size_t i = 0; i < columns_name.size(); ++i) {
        table[i].push_back(columns_name[i]);
        max_sizes[i] = std::max(max_sizes[i], table[i].back().size());
    }

    for (auto& row : rows) {
        if (!IsCondition(row.second, condition)) {
            continue;
        }
        for (size_t i = 0; i < columns_name.size(); ++i) {
            size_t index = row_properties.GetColumnIndex(columns_name[i]);
            table[i].push_back(row.second[index]);
            max_sizes[i] = std::max(max_sizes[i], table[i].back().size());
        }
    }

    return Print(table, max_sizes);
}

bool Table::IsCondition(Row& row, std::vector<std::string> condition) {
    if (condition.empty()) {
        return true;
    }

    for (auto& token : condition) {
        if (row_properties.HasColumn(token)) {
            size_t index = row_properties.GetColumnIndex(token);
            if (row[index] == "NULL") {
                return false;
            }
            token = row[index];
        }
    }

    for (size_t i = 1; i < condition.size() - 1; ++i) {
        std::string prev = condition[i - 1];
        std::string next = condition[i + 1];
        if (condition[i] == "=" && ParserKolzukDB::IsVarchar(prev) && ParserKolzukDB::IsVarchar(next)) {
            if (condition[i - 1] == condition[i + 1]) {
                condition[i - 1] = "1";
                condition[i + 1] = "1";
            } else {
                condition[i - 1] = "0";
                condition[i + 1] = "1";
            }
        } else if (condition[i] == "=" && (ParserKolzukDB::IsVarchar(prev) && !ParserKolzukDB::IsVarchar(next)
        || !ParserKolzukDB::IsVarchar(prev) && ParserKolzukDB::IsVarchar(next))) {
            throw std::logic_error("Other types: '" + prev + "' and '" + next);
        }
    }

    ExpressionParser parser(condition);
    return Evaluate(parser.Parse());
}

void Table::Delete(const std::vector<std::string>& condition) {
    std::vector<size_t> deleted_rows;
    for (auto& row : rows) {
        if (IsCondition(row.second, condition)) {
            deleted_rows.push_back(row.first);
        }
    }

    for (const auto& index : deleted_rows) {
        rows.erase(index);
    }

    height--;
}
void Table::Update(const std::vector<std::string>& column_names,
                   const std::vector<std::string>& values,
                   const std::vector<std::string>& condition) {
    for (auto& row_pair : rows) {
        auto& row = row_pair.second;
        if (IsCondition(row, condition)) {
            for (size_t i = 0; i < column_names.size(); ++i) {
                size_t index = row_properties.GetColumnIndex(column_names[i]);
                row[index] = values[i];
            }
        }
    }
}

RowProperties Table::GetRowProperties() {
    return row_properties;
}

Row& Table::operator[](size_t index) {
    size_t counter = 0;
    for (auto& row : rows) {
        if (counter == index) {
            return row.second;
        }
        counter++;
    }
    throw std::logic_error("Out of range: '" + std::to_string(index) + "', size of table: " + std::to_string(height));
}

std::string Table::Print(const std::vector<std::vector<std::string>>& table, const std::vector<size_t>& max_sizes) {
    size_t n = table.size();
    size_t m = table[0].size();
    std::stringstream ss;

    for (int i = 0; i < n; ++i) {
        ss << std::left << std::setw(max_sizes[i]) << table[i][0] << '|';
    }
    ss << '\n';
    for (size_t j = 1; j < m; ++j) {
        for (size_t i = 0; i < n; ++i) {
            ss << std::left << std::setw(max_sizes[i]) << table[i][j] << '|';
        }
        ss << '\n';
    }
    return ss.str();
}

std::string Table::TestPrint(const std::vector<std::vector<std::string>>& table) {
    std::string result;
    size_t n = table.size();
    size_t m = table[0].size();

    for (int i = 0; i < n; ++i) {
        result += table[i][0];
        result += '|';
    }
    result += '\n';
    for (size_t j = 1; j < m; ++j) {
        for (size_t i = 0; i < n; ++i) {
            result += table[i][j];
            result += '|';
        }
        result += '\n';
    }
}

