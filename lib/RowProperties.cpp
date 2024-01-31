#include "RowProperties.h"

void RowProperties::SetProperties(const std::string& type_of_column,
                                  const std::string& name_of_column,
                                  bool is_not_null,
                                  bool is_primary_key) {
    column_properties.emplace_back(type_of_column, name_of_column, is_not_null, is_primary_key);
}

void RowProperties::SetProperties(const ColumnProperties& column_properties) {
    SetProperties(column_properties.type, column_properties.name, column_properties.is_not_null, column_properties.is_primary_key);
}


bool RowProperties::HasColumn(const std::string& column_name) {
    for (const auto& column : column_properties) {
        if (column.name == column_name) {
            return true;
        }
    }
    return false;
}

std::string RowProperties::GetColumnType(const std::string& column_name) {
    for (size_t i = 0; i < column_properties.size(); ++i) {
        if (column_properties[i].name == column_name) {
            return column_properties[i].type;
        }
    }
    throw std::logic_error("Column isn`t exist");
}

size_t RowProperties::GetColumnIndex(const std::string& column_name) {
    for (size_t i = 0; i < column_properties.size(); ++i) {
        if (column_properties[i].name == column_name) {
            return i;
        }
    }
    throw std::logic_error("Column isn`t exist");
}

size_t RowProperties::Size() const {
    return column_properties.size();
}

ColumnProperties& RowProperties::operator[](size_t index) {
    return column_properties[index];
}



