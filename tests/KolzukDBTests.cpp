#include <gtest/gtest.h>

#include <lib/KolzukDB.h>

TEST(ExpressionTest, 1EqualTest) {
    std::vector<std::string> expression = {"1", "=", "2"};
    ExpressionParser parser(expression);
    EXPECT_EQ(Evaluate(parser.Parse()), false);
}

TEST(ExpressionTest, 2EqualTest) {
    std::vector<std::string> expression = {"1", "=", "1"};
    ExpressionParser parser(expression);
    EXPECT_EQ(Evaluate(parser.Parse()), true);
}

TEST(ExpressionTest, 3EqualTest) {
    std::vector<std::string> expression = {"(", "3", "+", "7", ")", "=", "10"};
    ExpressionParser parser(expression);
    EXPECT_EQ(Evaluate(parser.Parse()), true);
}

TEST(ExpressionTest, 4EqualTest) {
    std::vector<std::string> expression = {"-", "10", "=", "3"};
    ExpressionParser parser(expression);
    EXPECT_EQ(Evaluate(parser.Parse()), false);
}

TEST(ExpressionTest, LessTest) {
    std::vector<std::string> expression = {"7", ">", "3", "AND", "4", "<", "5"};
    ExpressionParser parser(expression);
    EXPECT_EQ(Evaluate(parser.Parse()), true);
}

TEST(ExpressionTest, Less2Test) {
    std::vector<std::string> expression = {"7", ">", "8", "OR", "6", "<", "5"};
    ExpressionParser parser(expression);
    EXPECT_EQ(Evaluate(parser.Parse()), false);
}

TEST(ExpressionTest, Less3Test) {
    std::vector<std::string> expression = {"(", "1", ">", "(", "1", "+", "3", ")", ")"};
    ExpressionParser parser(expression);
    EXPECT_EQ(Evaluate(parser.Parse()), false);
}

TEST(ParserTest, IntTest) {
    ParserKolzukDB parser;
    EXPECT_TRUE(parser.IsInt("123"));
    EXPECT_TRUE(parser.IsInt("100"));
    EXPECT_TRUE(parser.IsInt("213213"));
    EXPECT_TRUE(parser.IsInt("0"));
    EXPECT_TRUE(parser.IsInt("1"));
    EXPECT_TRUE(parser.IsInt("123"));
    EXPECT_FALSE(parser.IsInt("awdq"));
    EXPECT_FALSE(parser.IsInt("0.0001"));
    EXPECT_FALSE(parser.IsInt("-"));
}

TEST(ParserTest, DoubleTest) {
    ParserKolzukDB parser;
    EXPECT_TRUE(parser.IsDouble("0.23"));
    EXPECT_TRUE(parser.IsDouble("123"));
    EXPECT_TRUE(parser.IsDouble("-77"));
    EXPECT_TRUE(parser.IsDouble("-0.73"));
    EXPECT_TRUE(parser.IsDouble("345.567"));
    EXPECT_TRUE(parser.IsDouble("+67"));
    EXPECT_TRUE(parser.IsDouble("1.000"));
    EXPECT_FALSE(parser.IsDouble("a34"));
    EXPECT_FALSE(parser.IsDouble(".56"));
    EXPECT_FALSE(parser.IsDouble("15."));
    EXPECT_FALSE(parser.IsDouble("15-16"));
    EXPECT_FALSE(parser.IsDouble(".7-53"));
}

TEST(ParserTest, BoolTest) {
    ParserKolzukDB parser;
    EXPECT_TRUE(parser.IsBool("TRUE"));
    EXPECT_TRUE(parser.IsBool("FALSE"));
    EXPECT_FALSE(parser.IsBool("aytal"));
    EXPECT_FALSE(parser.IsBool("Fallse"));
    EXPECT_FALSE(parser.IsBool("truuuee"));
    EXPECT_FALSE(parser.IsBool("aytal"));
    EXPECT_FALSE(parser.IsBool("sad"));
    EXPECT_FALSE(parser.IsBool("asdsda"));
}

TEST(ParserTest, IsCorrectName) {
    ParserKolzukDB parser;
    EXPECT_TRUE(parser.IsCorrectName("aytal"));
    EXPECT_TRUE(parser.IsCorrectName("vanya"));
    EXPECT_FALSE(parser.IsCorrectName("aytal_"));
    EXPECT_FALSE(parser.IsCorrectName("Fallse123__"));
    EXPECT_FALSE(parser.IsCorrectName("truuuee_"));
    EXPECT_FALSE(parser.IsCorrectName("ayt@alas"));
    EXPECT_FALSE(parser.IsCorrectName("sadM<<as"));
    EXPECT_FALSE(parser.IsCorrectName("asdsdd+_+_+_+a"));
}