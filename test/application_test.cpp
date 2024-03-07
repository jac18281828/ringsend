#include "gtest/gtest.h"

#include "application.h"

TEST(ApplicationTest, TestParseNoArgs)
{
    int argc = 1;
    char *argv[] = {(char *)"./application"};
    struct args args;
    EXPECT_EQ(parse_args(argc, argv, &args), 1);
}

TEST(ApplicationTest, TestParseTooFewArgs)
{
    int argc = 2;
    char *argv[] = {(char *)"./application", (char *)"1"};
    struct args args;
    EXPECT_EQ(parse_args(argc, argv, &args), 1);
}

TEST(ApplicationTest, TestParseArgsCountPort)
{
    int argc = 3;
    char *argv[] = {(char *)"./application", (char *)"1", (char *)"8081"};
    struct args args;
    EXPECT_EQ(parse_args(argc, argv, &args), 0);
    EXPECT_EQ(args.thread_count, 1);
    EXPECT_EQ(args.initial_port, 8081);
}

TEST(ApplicationTest, TestParseArgsPortInvalid)
{
    int argc = 3;
    char *argv[] = {(char *)"./application", (char *)"1", (char *)"abc"};
    struct args args;
    EXPECT_EQ(parse_args(argc, argv, &args), 1);
}

TEST(ApplicationTest, TestParseArgsPortTooLarge)
{
    int argc = 3;
    char *argv[] = {(char *)"./application", (char *)"1", (char *)"65537"};
    struct args args;
    EXPECT_EQ(parse_args(argc, argv, &args), 1);
}

TEST(ApplicationTest, TestParseArgsPortTooSmall)
{
    int argc = 3;
    char *argv[] = {(char *)"./application", (char *)"1", (char *)"1000"};
    struct args args;
    EXPECT_EQ(parse_args(argc, argv, &args), 1);
}

TEST(ApplicationTest, TestParseArgsThreadCountInvalid)
{
    int argc = 3;
    char *argv[] = {(char *)"./application", (char *)"abc", (char *)"8081"};
    struct args args;
    EXPECT_EQ(parse_args(argc, argv, &args), 1);
}

TEST(ApplicationTest, TestParseArgsThreadCountZero)
{
    int argc = 3;
    char *argv[] = {(char *)"./application", (char *)"0", (char *)"8081"};
    struct args args;
    EXPECT_EQ(parse_args(argc, argv, &args), 1);
}

TEST(ApplicationTest, TestParseArgsThreadCountMax)
{
    int argc = 3;
    char *argv[] = {(char *)"./application", (char *)"11", (char *)"8081"};
    struct args args;
    EXPECT_EQ(parse_args(argc, argv, &args), 1);
}