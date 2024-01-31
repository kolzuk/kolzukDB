#include <gtest/gtest.h>

#include <lib/KolzukDB.h>

TEST(CorrectInsertTest, InsertTest) {
    ColumnProperties first_column("VARCHAR", "name", "false", "false");
    ColumnProperties second_column("INT", "grade", "false", "false");

    RowProperties row_properties;
    row_properties.SetProperties(first_column);
    row_properties.SetProperties(second_column);

    Table table(row_properties);

    table.InsertRow({"name", "grade"}, {"\"aytal\"", "5"});
    table.InsertRow({"name", "grade"}, {"\"ivan\"", "4"});

    EXPECT_EQ(table[0][0], "\"aytal\"");
    EXPECT_EQ(table[0][1], "5");
    EXPECT_EQ(table[1][0], "\"ivan\"");
    EXPECT_EQ(table[1][1], "4");
}

TEST(CorrectDeleteTest, DeleteTest) {
    ColumnProperties first_column("VARCHAR", "name", "false", "false");
    ColumnProperties second_column("INT", "grade", "false", "false");

    RowProperties row_properties;
    row_properties.SetProperties(first_column);
    row_properties.SetProperties(second_column);

    Table table(row_properties);

    table.InsertRow({"name", "grade"}, {"\"aytal\"", "5"});
    table.InsertRow({"name", "grade"}, {"\"ivan\"", "4"});

    EXPECT_EQ(table[0][0], "\"aytal\"");
    EXPECT_EQ(table[0][1], "5");
    EXPECT_EQ(table[1][0], "\"ivan\"");
    EXPECT_EQ(table[1][1], "4");

    EXPECT_EQ(table.height, 2);

    table.Delete({"name", "=", "\"ivan\""});

    EXPECT_EQ(table.height, 1);

    EXPECT_ANY_THROW(table[1][0]);
    EXPECT_ANY_THROW(table[1][1]);
}

TEST(CorrectUpdateTest, UpdateTest) {
    ColumnProperties first_column("VARCHAR", "name", "false", "false");
    ColumnProperties second_column("INT", "grade", "false", "false");

    RowProperties row_properties;
    row_properties.SetProperties(first_column);
    row_properties.SetProperties(second_column);

    Table table(row_properties);

    table.InsertRow({"name", "grade"}, {"\"aytal\"", "5"});
    table.InsertRow({"name", "grade"}, {"\"ivan\"", "4"});

    EXPECT_EQ(table[0][0], "\"aytal\"");
    EXPECT_EQ(table[0][1], "5");
    EXPECT_EQ(table[1][0], "\"ivan\"");
    EXPECT_EQ(table[1][1], "4");

    EXPECT_EQ(table.height, 2);

    table.Update({"name"}, {"\"egor\""}, {"name", "=", "\"ivan\""});

    EXPECT_EQ(table[0][0], "\"aytal\"");
    EXPECT_EQ(table[0][1], "5");
    EXPECT_EQ(table[1][0], "\"egor\"");
    EXPECT_EQ(table[1][1], "4");

    EXPECT_EQ(table.height, 2);
}

TEST(GetColumnTypeTest, ColumnTypeTest) {
    ColumnProperties first_column("VARCHAR", "name", "false", "false");
    ColumnProperties second_column("INT", "grade", "false", "false");

    RowProperties row_properties;
    row_properties.SetProperties(first_column);
    row_properties.SetProperties(second_column);

    Table table(row_properties);

    table.InsertRow({"name", "grade"}, {"\"aytal\"", "5"});
    table.InsertRow({"name", "grade"}, {"\"ivan\"", "4"});

    EXPECT_TRUE(table.HasColumn("name"));
    EXPECT_TRUE(table.HasColumn("grade"));
    EXPECT_EQ(table[0][1], "5");
    EXPECT_EQ(table[1][0], "\"ivan\"");
    EXPECT_EQ(table[1][1], "4");

    EXPECT_EQ(table.height, 2);

    table.Update({"name"}, {"\"egor\""}, {"name", "=", "\"ivan\""});

    EXPECT_EQ(table[0][0], "\"aytal\"");
    EXPECT_EQ(table[0][1], "5");
    EXPECT_EQ(table[1][0], "\"egor\"");
    EXPECT_EQ(table[1][1], "4");

    EXPECT_EQ(table.height, 2);
}

TEST(GetColumnTypeTest, GetColumnTypeTest) {
    ColumnProperties first_column("VARCHAR", "name", "false", "false");
    ColumnProperties second_column("INT", "grade", "false", "false");

    RowProperties row_properties;
    row_properties.SetProperties(first_column);
    row_properties.SetProperties(second_column);

    Table table(row_properties);

    table.InsertRow({"name", "grade"}, {"\"aytal\"", "5"});
    table.InsertRow({"name", "grade"}, {"\"ivan\"", "4"});

    EXPECT_TRUE(table.HasColumn("name"));
    EXPECT_TRUE(table.HasColumn("grade"));

    EXPECT_EQ(table.height, 2);
}

TEST(GetColumnIndexTest, GetColumnIndex) {
    ColumnProperties first_column("VARCHAR", "name", "false", "false");
    ColumnProperties second_column("INT", "grade", "false", "false");

    RowProperties row_properties;
    row_properties.SetProperties(first_column);
    row_properties.SetProperties(second_column);

    EXPECT_EQ(row_properties.Size(), 2);
    EXPECT_EQ(row_properties.GetColumnIndex("name"), 0);
    EXPECT_EQ(row_properties.GetColumnIndex("grade"), 1);
}


TEST(GetColumnTypeTest, GetColumnType) {
    ColumnProperties first_column("VARCHAR", "name", "false", "false");
    ColumnProperties second_column("INT", "grade", "false", "false");

    RowProperties row_properties;
    row_properties.SetProperties(first_column);
    row_properties.SetProperties(second_column);

    EXPECT_EQ(row_properties.Size(), 2);
    EXPECT_EQ(row_properties.GetColumnType("name"), "VARCHAR");
    EXPECT_EQ(row_properties.GetColumnType("grade"), "INT");
}

TEST(HasColumnTypeTest, HasColumnType) {
    ColumnProperties first_column("VARCHAR", "name", "false", "false");
    ColumnProperties second_column("INT", "grade", "false", "false");

    RowProperties row_properties;
    row_properties.SetProperties(first_column);
    row_properties.SetProperties(second_column);

    EXPECT_EQ(row_properties.Size(), 2);
    EXPECT_TRUE(row_properties.HasColumn("name"));
    EXPECT_TRUE(row_properties.HasColumn("grade"));
}

TEST(DeleteAllTest, DeleteAllTest) {
    ColumnProperties first_column("VARCHAR", "name", "false", "false");
    ColumnProperties second_column("INT", "grade", "false", "false");

    RowProperties row_properties;
    row_properties.SetProperties(first_column);
    row_properties.SetProperties(second_column);

    Table table(row_properties);

    table.InsertRow({"name", "grade"}, {"\"aytal\"", "5"});
    table.InsertRow({"name", "grade"}, {"\"ivan\"", "4"});

    EXPECT_EQ(table[0][0], "\"aytal\"");
    EXPECT_EQ(table[0][1], "5");
    EXPECT_EQ(table[1][0], "\"ivan\"");
    EXPECT_EQ(table[1][1], "4");

    EXPECT_EQ(table.height, 2);

    table.Delete({"name", "=", "\"ivan\""});

    EXPECT_EQ(table.height, 1);

    table.Delete({"name", "=", "\"aytal\""});

    EXPECT_EQ(table.height, 0);
    EXPECT_EQ(table.width, 2);

    EXPECT_ANY_THROW(table[0][0]);
    EXPECT_ANY_THROW(table[0][1]);
    EXPECT_ANY_THROW(table[1][0]);
    EXPECT_ANY_THROW(table[1][1]);
}

TEST(InnerJoin, InnerJoinWithSelf) {
    KolzukDB db;
    db.Execute("CREATE TABLE Authors (AuthorID INT, AuthorName VARCHAR);");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 1, \"BruceEckel\";");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 2, \"RobertLafore\";");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 3, \"AndrewTanenbaum\";");
    EXPECT_EQ(db.Execute("SELECT * FROM Authors INNER JOIN Authors ON AuthorID = AuthorID"),
              "AuthorID|AuthorName       |\n1       |\"BruceEckel\"     |\n2       |\"RobertLafore\"   |\n3       |\"AndrewTanenbaum\"|\n");
    EXPECT_EQ(db.Execute("SELECT * FROM Authors INNER JOIN Authors ON AuthorID = AuthorID"),
              "AuthorID|AuthorName       |\n1       |\"BruceEckel\"     |\n2       |\"RobertLafore\"   |\n3       |\"AndrewTanenbaum\"|\n");
}

TEST(InnerJoin, SimpleInnerJoinTest) {
    KolzukDB db;
    db.Execute("CREATE TABLE Authors (AuthorID INT, AuthorName VARCHAR);");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 1, \"BruceEckel\";");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 2, \"RobertLafore\";");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 3, \"AndrewTanenbaum\";");
    db.Execute("CREATE TABLE Books BookID int BookName VARCHAR;");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 3, \"ModernOperatingSystem\"");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 1, \"ThinkinginJava\"");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 3, \"ComputerArchitecture\"");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 4, \"ProgramminginScala\"");
    EXPECT_EQ(db.Execute("SELECT * FROM Authors INNER JOIN Books ON AuthorID = BookID"),
R"(AuthorID|AuthorName       |BookID|BookName               |
1       |"BruceEckel"     |1     |"ThinkinginJava"       |
3       |"AndrewTanenbaum"|3     |"ModernOperatingSystem"|
3       |"AndrewTanenbaum"|3     |"ComputerArchitecture" |
)");
}

TEST(LeftJoin, SimpleLeftJoinTest) {
    KolzukDB db;
    db.Execute("CREATE TABLE Authors (AuthorID INT, AuthorName VARCHAR);");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 1, \"BruceEckel\";");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 2, \"RobertLafore\";");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 3, \"AndrewTanenbaum\";");
    db.Execute("CREATE TABLE Books BookID int BookName VARCHAR;");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 3, \"ModernOperatingSystem\"");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 1, \"ThinkinginJava\"");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 3, \"ComputerArchitecture\"");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 4, \"ProgramminginScala\"");
    EXPECT_EQ(db.Execute("SELECT * FROM Authors LEFT JOIN Books ON AuthorID = BookID"),
R"(AuthorID|AuthorName       |BookID|BookName               |
1       |"BruceEckel"     |1     |"ThinkinginJava"       |
2       |"RobertLafore"   |NULL  |NULL                   |
3       |"AndrewTanenbaum"|3     |"ModernOperatingSystem"|
3       |"AndrewTanenbaum"|3     |"ComputerArchitecture" |
)");
}

TEST(RightJoin, SimpleRightJoinTest) {
    KolzukDB db;
    db.Execute("CREATE TABLE Authors (AuthorID INT, AuthorName VARCHAR);");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 1, \"BruceEckel\";");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 2, \"RobertLafore\";");
    db.Execute("INSERT INTO Authors AuthorID, AuthorName VALUES 3, \"AndrewTanenbaum\";");
    db.Execute("CREATE TABLE Books BookID int BookName VARCHAR;");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 3, \"ModernOperatingSystem\"");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 1, \"ThinkinginJava\"");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 3, \"ComputerArchitecture\"");
    db.Execute("INSERT INTO Books BookID, BookName VALUES 4, \"ProgramminginScala\"");
    EXPECT_EQ(db.Execute("SELECT * FROM Authors RIGHT JOIN Books ON AuthorID = BookID"),
R"(AuthorID|AuthorName       |BookID|BookName               |
3       |"AndrewTanenbaum"|3     |"ModernOperatingSystem"|
1       |"BruceEckel"     |1     |"ThinkinginJava"       |
3       |"AndrewTanenbaum"|3     |"ComputerArchitecture" |
NULL    |NULL             |4     |"ProgramminginScala"   |
)");
}

TEST(NotNull, NotNullTest) {
    KolzukDB db;
    db.Execute("CREATE TABLE group (ISU INT PRIMARY KEY, name VARCHAR NOT NULL);");
    db.Execute("INSERT INTO group (ISU name) VALUES (1, \"aytal\");");
    EXPECT_ANY_THROW(db.Execute("INSERT INTO group (ISU) VALUES (2);"));
}

TEST(ForeignKey, ForeignKeyTest) {
    KolzukDB db;
    db.Execute("CREATE TABLE group (ISU INT PRIMARY KEY, name VARCHAR NOT NULL);");
    db.Execute("INSERT INTO group (ISU name) VALUES (1, \"aytal\");");
    EXPECT_ANY_THROW(db.Execute("INSERT INTO group (ISU name) VALUES (1, \"Vanya\");"));
}

TEST(ArithmeticOperationTest, AbsTest) {
    KolzukDB db;
    db.Execute("CREATE TABLE group (ISU INT PRIMARY KEY, name VARCHAR NOT NULL);");
    db.Execute("INSERT INTO group (ISU name) VALUES (1, \"Aytal\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (2, \"Ivan\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (3, \"Anya\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (4, \"Vasya\");");
    EXPECT_EQ(db.Execute("SELECT * FROM group WHERE ISU < abs ( - 3 )"),
R"(ISU|name   |
1  |"Aytal"|
2  |"Ivan" |
)");
}

TEST(ArithmeticOperationTest, CosinusTest) {
    KolzukDB db;
    db.Execute("CREATE TABLE group (ISU INT PRIMARY KEY, name VARCHAR NOT NULL);");
    db.Execute("INSERT INTO group (ISU name) VALUES (1, \"Aytal\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (2, \"Ivan\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (3, \"Anya\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (4, \"Vasya\");");
    EXPECT_EQ(db.Execute("SELECT * FROM group WHERE ISU = cos ( 0 ) + 1"),
R"(ISU|name  |
2  |"Ivan"|
)");
}

TEST(LogicalOperationTest, AndTest) {
    KolzukDB db;
    db.Execute("CREATE TABLE group (ISU INT PRIMARY KEY, name VARCHAR NOT NULL);");
    db.Execute("INSERT INTO group (ISU name) VALUES (1, \"Aytal\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (2, \"Ivan\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (3, \"Anya\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (4, \"Vasya\");");
    EXPECT_EQ(db.Execute("SELECT * FROM group WHERE ISU > 1 AND ISU < 3"),
R"(ISU|name  |
2  |"Ivan"|
)");
}

TEST(LogicalOperationTest, OrTest) {
    KolzukDB db;
    db.Execute("CREATE TABLE group (ISU INT PRIMARY KEY, name VARCHAR NOT NULL);");
    db.Execute("INSERT INTO group (ISU name) VALUES (1, \"Aytal\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (2, \"Ivan\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (3, \"Anya\");");
    db.Execute("INSERT INTO group (ISU name) VALUES (4, \"Vasya\");");
    EXPECT_EQ(db.Execute("SELECT * FROM group WHERE ISU = 1 OR name = \"Ivan\""),
R"(ISU|name   |
1  |"Aytal"|
2  |"Ivan" |
)");
}




