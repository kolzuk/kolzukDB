#include "Row.h"

Row::Row(size_t size_of_row) {
    row.resize(size_of_row, "NULL");
}

std::string& Row::operator[](size_t index) {
    return row[index];
}

size_t Row::Size() const {
    return row.size();
}

Row& Row::operator=(const Row& other) = default;

Row Row::operator+(Row& other) {
    Row new_row(row.size() + other.Size());
    for (int i = 0; i < row.size(); ++i) {
        new_row[i] = row[i];
    }
    for (int i = 0; i < other.Size(); ++i) {
        new_row[i + row.size()] = other[i];
    }
    return new_row;
}

void Row::Print(std::ostream& stream) const {
    for (const auto& i : row) {
        stream << i << " | ";
    }
}

std::ostream& operator<<(std::ostream& stream, const Row& row) {
    row.Print(stream);
    return stream;
}


