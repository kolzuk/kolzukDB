#include "KolzukDB.h"

std::string KolzukDB::Execute(const std::string& statement) {
    if (statement.empty()) {
        return "Empty request";
    }

    request.push_back(statement);

    parser.Parse(statement);

    if (parser.Take("CREATE")) {
        parser.Expect("TABLE");
        CreateTable();
    } else if (parser.Take("DROP")) {
        parser.Expect("TABLE");
        DropTable();
    } else if (parser.Take("INSERT")) {
        parser.Expect("INTO");
        Insert();
    } else if (parser.Take("SELECT")) {
        return Select();
    } else if (parser.Take("DELETE")) {
        parser.Expect("FROM");
        Delete();
    } else if (parser.Take("UPDATE")) {
        Update();
    } else  {
        throw std::logic_error("Incorrect command: '" + parser.NextWord() + "'");
    }
    return "Success";
}

void KolzukDB::CreateTable() {
    std::string table_name = parser.ParseName();
    if (tables.contains(table_name)) {
        throw std::logic_error("Table is already exits");
    }
    RowProperties row_properties;
    while (parser.HasNextWord()) {
        ColumnProperties column_properties;
        column_properties.name = parser.ParseName();
        column_properties.type = parser.ParseType();

        if (parser.Take("NOT")) {
            parser.Expect("NULL");
            column_properties.is_not_null = true;
        }

        if (parser.Take("PRIMARY")) {
            parser.Expect("KEY");
            column_properties.is_primary_key = true;
            column_properties.is_not_null = true;
        }

        row_properties.SetProperties(column_properties);
    }

    tables[table_name] = Table(row_properties);
}

void KolzukDB::DropTable() {
    std::string table_name = parser.ParseName();
    tables.erase(table_name);
}

void KolzukDB::Insert() {
    std::string table_name = parser.ParseName();
    std::vector<std::string> columns_name;
    std::vector<std::string> values;

    while (!parser.Take("VALUES")) {
        std::string name_of_column = parser.ParseName();
        if (!tables.at(table_name).HasColumn(name_of_column)) {
            throw std::logic_error("Column doesn`t exist: " + name_of_column);
        }
        columns_name.push_back(name_of_column);
    }

    for (const auto& column : columns_name) {
        std::string column_type = tables.at(table_name).GetColumnType(column);
        std::string value = parser.ParseValue();

        if (ParserKolzukDB::ValueType(value) != column_type) {
            throw std::logic_error("Value type is not match with type of column: " + value);
        }

        values.push_back(value);
    }

    tables.at(table_name).InsertRow(columns_name, values);

    if (parser.HasNextWord()) {
        throw std::logic_error("Incorrect values count");
    }
}

std::string KolzukDB::Select() {
    std::vector<std::string> columns_name;
    std::vector<std::string> condition;

    bool is_all_columns = false;

    if (parser.Take("*")) {
        is_all_columns = true;
    }

    while (!parser.Take("FROM")) {
        std::string name_of_column = parser.ParseName();
        columns_name.push_back(name_of_column);
    }

    std::string table_name = parser.ParseName();
    Table table = tables.at(table_name);

    if (parser.Take("INNER")) {
        parser.Expect("JOIN");
        table = InnerJoin(table_name);
    } else if (parser.Take("LEFT")) {
        parser.Expect("JOIN");
        table = LeftJoin(table_name);
    } else if (parser.Take("RIGHT")) {
        parser.Expect("JOIN");
        table = RightJoin(table_name);
    }

    if (parser.Take("WHERE")) {
        while (parser.HasNextWord()) {
            condition.push_back(parser.NextWord());
        }
    }

    if (parser.HasNextWord()) {
        throw std::logic_error("Incorrect query");
    }

    if (is_all_columns) {
        if (!columns_name.empty()) {
            throw std::logic_error("Syntax error! * + column_names");
        }
        RowProperties table_row_properties = table.GetRowProperties();
        for (size_t i = 0; i < table_row_properties.Size(); ++i) {
            columns_name.push_back(table_row_properties[i].name);
        }
    }
    return table.Select(columns_name, condition);
}

void KolzukDB::Delete() {
    std::string table_name = parser.ParseName();

    parser.Expect("WHERE");

    std::vector<std::string> condition;

    while (parser.HasNextWord()) {
        condition.push_back(parser.NextWord());
    }

    tables.at(table_name).Delete(condition);
}

void KolzukDB::Update() {
    std::string table_name = parser.ParseName();

    parser.Expect("SET");

    std::vector<std::string> columns_names;
    std::vector<std::string> values;

    while (!parser.Take("WHERE")) {
        std::string name_of_column = parser.ParseName();
        columns_names.push_back(name_of_column);

        parser.Expect("=");

        std::string column_type = tables.at(table_name).GetColumnType(name_of_column);
        std::string value = parser.ParseValue();

        if (ParserKolzukDB::ValueType(value) != column_type) {
            throw std::logic_error("Value type is not match with type of column: " + value);
        }
        values.push_back(value);
    }

    std::vector<std::string> condition;

    while (parser.HasNextWord()) {
        condition.push_back(parser.NextWord());
    }

    tables.at(table_name).Update(columns_names, values, condition);
}

Table KolzukDB::InnerJoin(const std::string& first_name) {
    std::string second_name = parser.ParseName();

    parser.Expect("ON");

    Table* first_table = &tables.at(first_name);
    Table* second_table = &tables.at(second_name);

    std::vector<Row> new_rows;

    std::string first_column = parser.ParseName();
    parser.Expect("=");
    std::string second_column = parser.ParseName();

    RowProperties first_properties = first_table->GetRowProperties();
    RowProperties second_properties = second_table->GetRowProperties();

    size_t first_index = first_properties.GetColumnIndex(first_column);
    size_t second_index = second_properties.GetColumnIndex(second_column);

    for (size_t i = 0; i < first_table->height; ++i) {
        for (size_t j = 0; j < second_table->height; ++j) {
            if ((*first_table)[i][first_index] == (*second_table)[j][second_index]) {
                new_rows.push_back((*first_table)[i] + (*second_table)[j]);
            }
        }
    }

    for (size_t i = 0; i < second_properties.Size(); ++i) {
        if (!first_properties.HasColumn(second_properties[i].name)) {
            first_properties.SetProperties(second_properties[i]);
        }
    }

    Table new_table(first_properties);

    for (const auto& new_row : new_rows) {
        new_table.InsertRow(new_row);
    }
    return new_table;
}

Table KolzukDB::LeftJoin(const std::string& first_name) {
    std::string second_name = parser.ParseName();

    parser.Expect("ON");

    Table* first_table = &tables.at(first_name);
    Table* second_table = &tables.at(second_name);

    std::vector<Row> new_rows;

    std::string first_column = parser.ParseName();
    parser.Expect("=");
    std::string second_column = parser.ParseName();

    RowProperties first_properties = first_table->GetRowProperties();
    RowProperties second_properties = second_table->GetRowProperties();

    size_t first_index = first_properties.GetColumnIndex(first_column);
    size_t second_index = second_properties.GetColumnIndex(second_column);

    Row null_row(second_properties.Size());

    bool is_find = false;

    for (size_t i = 0; i < first_table->height; ++i) {
        for (size_t j = 0; j < second_table->height; ++j) {
            if ((*first_table)[i][first_index] == (*second_table)[j][second_index]) {
                new_rows.push_back((*first_table)[i] + (*second_table)[j]);
                is_find = true;
            }
        }
        if (!is_find) {
            new_rows.push_back((*first_table)[i] + null_row);
        }
        is_find = false;
    }

    for (size_t i = 0; i < second_properties.Size(); ++i) {
        if (!first_properties.HasColumn(second_properties[i].name)) {
            first_properties.SetProperties(second_properties[i]);
        }
    }

    Table new_table(first_properties);

    for (const auto& new_row : new_rows) {
        new_table.InsertRow(new_row);
    }
    return new_table;
}

Table KolzukDB::RightJoin(const std::string& first_name) {
    std::string second_name = parser.ParseName();

    parser.Expect("ON");

    Table* first_table = &tables.at(first_name);
    Table* second_table = &tables.at(second_name);

    std::vector<Row> new_rows;

    std::string first_column = parser.ParseName();
    parser.Expect("=");
    std::string second_column = parser.ParseName();

    RowProperties first_properties = first_table->GetRowProperties();
    RowProperties second_properties = second_table->GetRowProperties();

    size_t first_index = first_properties.GetColumnIndex(first_column);
    size_t second_index = second_properties.GetColumnIndex(second_column);

    Row null_row(second_properties.Size());

    bool is_find = false;

    for (size_t i = 0; i < second_table->height; ++i) {
        for (size_t j = 0; j < first_table->height; ++j) {
            if ((*second_table)[i][second_index] == (*first_table)[j][first_index]) {
                new_rows.push_back((*first_table)[j] + (*second_table)[i]);
                is_find = true;
            }
        }
        if (!is_find) {
            new_rows.push_back(null_row + (*second_table)[i]);
        }
        is_find = false;
    }

    for (size_t i = 0; i < second_properties.Size(); ++i) {
        if (!first_properties.HasColumn(second_properties[i].name)) {
            first_properties.SetProperties(second_properties[i]);
        }
    }

    Table new_table(first_properties);

    for (const auto& new_row : new_rows) {
        new_table.InsertRow(new_row);
    }
    return new_table;
}

void KolzukDB::WriteInFile(const std::filesystem::path& path) {
    std::ofstream file(path);

    assert(file.is_open());

    for (const auto& s : request) {
        file << s << '\n';
    }

    file.close();
}

void KolzukDB::ReadFromFile(const std::filesystem::path& path) {
    std::ifstream file(path);

    assert(file.is_open());

    std::string execute;

    while (file) {
        std::getline(file, execute);
        Execute(execute);
    }
}


