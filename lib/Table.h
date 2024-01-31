#include "Column.h"
#include "Row.h"
#include "RowProperties.h"
#include "ExpressionParser.h"
#include "ParserKolzukDB.h"

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <iomanip>
#include <unordered_set>

class Table {
public:
    Table();
    explicit Table(RowProperties& _row_properties);
    void InsertRow(const std::vector<std::string>& column_names,
                   const std::vector<std::string>& values);
    void InsertRow(const Row& row);
    bool HasColumn(const std::string& column_name);
    std::string GetColumnType(const std::string& column_name);
    std::string Select(const std::vector<std::string>& columns_name, const std::vector<std::string>& condition);
    void Delete(const std::vector<std::string>& condition);
    void Update(const std::vector<std::string>& column_names,
                const std::vector<std::string>& values,
                const std::vector<std::string>& condition);
    RowProperties GetRowProperties();
    Row& operator[](size_t index);
    size_t height;
    size_t width;
    std::string TestPrint(const std::vector<std::vector<std::string>>& table);
protected:
    bool IsCondition(Row& row, std::vector<std::string> condition);
    std::string Print(const std::vector<std::vector<std::string>>& table, const std::vector<size_t>& max_sizes);
private:
    RowProperties row_properties;
    std::map<size_t, Row> rows;
    size_t current_index;
};
