#include <string>
#include <vector>
#include <stdexcept>

struct Column {
    std::string type;
    std::string name;
    std::vector<std::string> values;

    explicit Column(const std::string& column_name, const std::string& column_type);
    void AddValue(const std::string& value);
    std::vector<std::string>& GetValues(const std::string& value);
    std::string& operator[](size_t index);
    [[nodiscard]] size_t Size() const;
    [[nodiscard]] std::string GetColumnType() const;
};