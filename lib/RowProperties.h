#include <utility>
#include <vector>
#include <string>
#include <stdexcept>

struct ColumnProperties {
    std::string type;
    std::string name;
    bool is_not_null = false;
    bool is_primary_key = false;

    ColumnProperties() = default;

    ColumnProperties(const std::string& column_type,
                     const std::string& column_name,
                     bool is_null,
                     bool is_primary_key)
                     : type(column_type),
                       name(column_name),
                       is_not_null(is_null),
                       is_primary_key(is_primary_key) {}

};

class RowProperties {
public:
    void SetProperties(const std::string& type_of_column,
                       const std::string& name_of_column,
                       bool _is_not_null,
                       bool _is_primary_key);
    void SetProperties(const ColumnProperties& column_properties);
    bool HasColumn(const std::string& column_name);
    std::string GetColumnType(const std::string& column_name);
    size_t GetColumnIndex(const std::string& column_name);
    [[nodiscard]] size_t Size() const;
    ColumnProperties& operator[](size_t index);
private:
    std::vector<ColumnProperties> column_properties;
};
