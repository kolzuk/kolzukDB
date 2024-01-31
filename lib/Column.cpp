#include "Column.h"

#include <iostream>

Column::Column(const std::string& column_name, const std::string& column_type) {
    name = column_name;
    type = column_type;
}

void Column::AddValue(const std::string& value) {
    values.emplace_back(value);
}

std::vector<std::string>& Column::GetValues(const std::string& value) {
    return values;
}

std::string& Column::operator[](size_t index) {
    return values[index];
}

size_t Column::Size() const {
    return values.size();
}

std::string Column::GetColumnType() const {
    return type;
}
