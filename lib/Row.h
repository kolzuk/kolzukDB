#include <variant>
#include <string>
#include <vector>
#include <ostream>

class Row {
public:
    Row() = default;
    explicit Row(size_t size_of_row);
    std::string& operator[](size_t index);
    Row& operator=(const Row& other);
    Row operator+(Row& other);
    [[nodiscard]] size_t Size() const;
    void Print(std::ostream& stream) const;
private:
    std::vector<std::string> row;
};

std::ostream& operator<<(std::ostream& stream, const Row& row);