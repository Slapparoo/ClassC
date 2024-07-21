#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include <lex.h>

// @TODO move to own header
#define assertStr(expected, actual)							\
      if (strcmp(expected, actual)) {	                    \
        char buffer[1024];							        \
        sprintf(buffer, "%s expected \"%s\" == actual \"%s\"", #actual, expected, actual);    \
        __assert_fail (buffer, __FILE__, __LINE__, __ASSERT_FUNCTION);	                    \
      } \
    
#define assertInt(expected, actual, message)							\
      if (expected != actual) {	                    \
        char buffer[1024];							        \
        sprintf(buffer, "%s : %s expected %d == actual %d", message, #actual, expected, actual);    \
        __assert_fail (buffer, __FILE__, __LINE__, __ASSERT_FUNCTION);	                    \
      } \

struct TestToken {
    int type;
    char *value;
    char *expectedValue;
};

struct TestSequence {
    char *value;
    int types[20];
};

char message[100];

void testSomeCode() {
    lexInit();
    struct TestSequence testSequence = {
        "/* */... ++ +=//\n@static\n/** */\"String.\"\n@fina1l\nclass int main() { return 0b1001_0011; }",

        {TOKEN_ELLIPSIS, TOKEN_PLUSPLUS, TOKEN_PLUSEQUALS, TOKEN_AT, TOKEN_IDENTIFIER,
        TOKEN_STRING, TOKEN_AT, TOKEN_IDENTIFIER, TOKEN_CLASS, TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_L_PAREN, TOKEN_R_PAREN,
        TOKEN_L_BRACE, TOKEN_RETRUN, TOKEN_NUMBER_BINARY, TOKEN_SEMICOLON, TOKEN_R_BRACE, TOKEN_EOF, 0}
    };

    for (int i = 0; testSequence.types[i]; i++) {
        Token token = nextToken(&testSequence.value);
        sprintf(message, "i=%d %s", i, token.value);
        assertInt(token.type, testSequence.types[i], message);
    }
}


void testOctNumbers() {

    struct TestToken numberTokens[] = {
        {TOKEN_NUMBER_OCT, "01", "01"},
        {TOKEN_NUMBER_OCT, "010", "010"},
        {TOKEN_NUMBER_OCT, "011_", "011"},
        {TOKEN_NUMBER_OCT, "010_", "010"},
        {0}};

    for (int i = 0; numberTokens[i].type; i++) {
        Token token = nextToken(&numberTokens[i].value);

        // printf("Token: Type = %x %d : %d , line = %d, offset = %d, Value = '%s'\n", &token, token.type, numberTokens[i].type, token.lineCount, token.lineOffset, token.value);
        sprintf(message, "i=%d, %s", i, token.value);
        assertInt(numberTokens[i].type, token.type, message);
        assertStr(numberTokens[i].expectedValue, token.value);

        token = nextToken(&numberTokens[i].value);
        assertInt(TOKEN_EOF, token.type, message);
    }
}

void testFloatNumbers() {
    struct TestToken numberTokens[] = {
        {TOKEN_NUMBER_FLOAT, "0.1_", "0.1"},
        {TOKEN_NUMBER_FLOAT, "0.0", "0.0"},
        {TOKEN_NUMBER_FLOAT, "12.0_", "12.0"},
        {TOKEN_NUMBER_FLOAT, "12.", "12."},
        {TOKEN_NUMBER_FLOAT, "1_123.012_123", "1123.012123"},
        {0}};

    for (int i = 0; numberTokens[i].type; i++) {
        Token token = nextToken(&numberTokens[i].value);

        // printf("Token: Type = %x %d : %d , line = %d, offset = %d, Value = '%s'\n", &token, token.type, numberTokens[i].type, token.lineCount, token.lineOffset, token.value);
        sprintf(message, "i=%d, %s", i, token.value);
        assertInt(numberTokens[i].type, token.type, message);
        assertStr(numberTokens[i].expectedValue, token.value);

        token = nextToken(&numberTokens[i].value);
        assertInt(TOKEN_EOF, token.type, message);
    }
}

void testNumbers() {
    struct TestToken numberTokens[] = {
        {TOKEN_NUMBER, "0", "0"},
        {TOKEN_NUMBER, "1", "1"},
        {TOKEN_NUMBER, "0_", "0"},
        {TOKEN_NUMBER, "1_", "1"},
        {TOKEN_NUMBER, "1_2_34", "1234"},
        {0}};

    for (int i = 0; numberTokens[i].type; i++) {
        Token token = nextToken(&numberTokens[i].value);

        sprintf(message, "i=%d, %s", i, token.value);
        assertInt(numberTokens[i].type, token.type, message);
        assertStr(numberTokens[i].expectedValue, token.value);

        token = nextToken(&numberTokens[i].value);
        assertInt(TOKEN_EOF, token.type, message);
    }
}

// void testNumberUnexpected() {
    
//     struct TestToken numberTokens[] = {
//         // invalid/incorrect number combinations
//         {TOKEN_NUMBER_BINARY, "0b2", "0b", TOKEN_NUMBER},
//         {TOKEN_NUMBER_OCT, "078", "07", TOKEN_NUMBER},
//         {TOKEN_NUMBER_OCT, "07a", "07", TOKEN_IDENTIFIER},
//         {TOKEN_NUMBER_OCT, "07a", "07", TOKEN_IDENTIFIER},
//         {0}};

//     for (int i = 0; numberTokens[i].type; i++) {
//         Token token = nextToken(&numberTokens[i].value);

//         sprintf(message, "i=%d, %s", i, token.value);
//         // printf("Token: Type = %x %d : %d , line = %d, offset = %d, Value = '%s'\n", &token, token.type, numberTokens[i].type, token.lineCount, token.lineOffset, token.value);
//         assertInt(numberTokens[i].type, token.type, message);
//         assertStr(numberTokens[i].expectedValue, token.value);

//         token = nextToken(&numberTokens[i].value);
//         // printf("Token: Type = %x %d : %d , line = %d, offset = %d, Value = '%s'\n", &token, token.type, numberTokens[i].type, token.lineCount, token.lineOffset, token.value);
//         sprintf(message, "i=%d, %s", i, token.value);
//         assertInt(numberTokens[i].expectedType, token.type, message);
//     }
// }


void testBinaryNumbers() {
    struct TestToken numberTokens[] = {
        {TOKEN_NUMBER_BINARY, "0b0", "0b0"},
        {TOKEN_NUMBER_BINARY, "0B_1", "0B1"},
        {TOKEN_NUMBER_BINARY, "0b00_00", "0b0000"},
        {0}};

    for (int i = 0; numberTokens[i].type; i++) {
        Token token = nextToken(&numberTokens[i].value);

        sprintf(message, "i=%d, %s", i, token.value);
        // printf("Token: Type = %x %d : %d , line = %d, offset = %d, Value = '%s'\n", &token, token.type, numberTokens[i].type, token.lineCount, token.lineOffset, token.value);
        assertInt(numberTokens[i].type, token.type, message);
        assertStr(numberTokens[i].expectedValue, token.value);

        token = nextToken(&numberTokens[i].value);
        // printf("Token: Type = %x %d : %d , line = %d, offset = %d, Value = '%s'\n", &token, token.type, numberTokens[i].type, token.lineCount, token.lineOffset, token.value);
        sprintf(message, "i=%d, %s", i, token.value);
        assertInt(TOKEN_EOF, token.type, message);
    }
}

void testHexNumbers() {
    struct TestToken numberTokens[] = {
        {TOKEN_NUMBER_HEX, "0xAA", "0xAA"},
        {TOKEN_NUMBER_HEX, "0x_11", "0x11"},
        {TOKEN_NUMBER_HEX, "0xabcdef0_", "0xabcdef0"},
        {0}};

    for (int i = 0; numberTokens[i].type; i++) {
        Token token = nextToken(&numberTokens[i].value);

        sprintf(message, "i=%d, %s", i, token.value);
        assertInt(numberTokens[i].type, token.type, message);
        assertStr(numberTokens[i].expectedValue, token.value);

        token = nextToken(&numberTokens[i].value);
        assertInt(TOKEN_EOF, token.type, message);
    }
}


void testNumber() {
    testNumbers();
    testFloatNumbers();
    testOctNumbers();
    testBinaryNumbers();
    testHexNumbers();
    // testNumberUnexpected();
}

void testId() {
    struct TestToken numberTokens[] = {
        {TOKEN_IDENTIFIER, "az", "az"},
        {TOKEN_IDENTIFIER, "AZ", "AZ"},
        {TOKEN_IDENTIFIER, "b", "b"},
        {TOKEN_IDENTIFIER, "_A", "_A"},
        {TOKEN_IDENTIFIER, "c_", "c_"},
        {TOKEN_IDENTIFIER, "_d_", "_d_"},
        {TOKEN_IDENTIFIER, "e1", "e1"},
        {TOKEN_IDENTIFIER, "_a1$1_", "_a1$1_"},
        {0}};

    for (int i = 0; numberTokens[i].type; i++) {
        Token token = nextToken(&numberTokens[i].value);

        sprintf(message, "i=%d, %s", i, token.value);
        assertInt(numberTokens[i].type, token.type, message);
        assertStr(numberTokens[i].expectedValue, token.value);

        token = nextToken(&numberTokens[i].value);
        assertInt(TOKEN_EOF, token.type, message);
    }

    struct TestSequence testSequence = {
       "a A _A a_ _a_ a1 _a1$1_",

        {TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_IDENTIFIER,
        TOKEN_IDENTIFIER, TOKEN_IDENTIFIER, TOKEN_EOF, 0}
    };

    for (int i = 0; testSequence.types[i]; i++) {
        Token token = nextToken(&testSequence.value);
        sprintf(message, "i=%d %s", i, token.value);
        assertInt(token.type, testSequence.types[i], message);
    }
}

void testStrip() {
    lexInit();
    const char *source_code = " /* */ //\n /**  */";
    Token token = nextToken(&source_code);
    assert(token.type == TOKEN_EOF);

    const char *source_code_2 = "/* *";
    token = nextToken(&source_code_2);
    // printf("Token: Type = %d, line = %d, offset = %d, Value = '%s'\n", token.type, token.lineCount, token.lineOffset, token.value);

    assert(token.type == TOKEN_UNEXPECTED);
}

void testTokenTypes() {

    struct KeywordToken tokens[] = {
        {TOKEN_EQUALS, "="},
        {TOKEN_EQUALSEQUALS, "=="},
        {TOKEN_EQUALSGREATER, "=>"},
        {TOKEN_PLUS, "+"},
        {TOKEN_PLUSPLUS, "++"},
        {TOKEN_PLUSEQUALS, "+="},
        {TOKEN_MINUSEQUALS, "-="},
        {TOKEN_MINUS, "-"},
        {TOKEN_MINUSEGREATER, "->"},
        {TOKEN_STAR, "*"},
        {TOKEN_STAREQUALS, "*="},
        {TOKEN_STARSTAR, "**"},
        {TOKEN_DIV, "/"},
        {TOKEN_DIVEQUALS, "/="},
        {TOKEN_PERCENT, "%"},
        {TOKEN_PERCENTEQUALS, "%="},
        {TOKEN_NOT, "!"},
        {TOKEN_NOTEQUALS, "!="},
        {TOKEN_TILDE, "~"},
        {TOKEN_AT, "@"},
        {TOKEN_CARET, "^"},
        {TOKEN_CARETEQUALS, "^="},
        {TOKEN_AND, "&"},
        {TOKEN_ANDAND, "&&"},
        {TOKEN_ANDEQUALS, "&="},
        {TOKEN_L_PAREN, "("},
        {TOKEN_R_PAREN, ")"},
        {TOKEN_L_BRACE, "{"},
        {TOKEN_R_BRACE, "}"},
        {TOKEN_L_BRACKET, "["},
        {TOKEN_R_BRACKET, "]"},
        {TOKEN_OR, "|"},
        {TOKEN_OROR, "||"},
        {TOKEN_OREQUALS, "|="},
        {TOKEN_LESS, "<"},
        {TOKEN_LESSEQUALS, "<="},
        {TOKEN_GREATER, ">"},
        {TOKEN_GREATEREQUALS, ">="},
        {TOKEN_COMMA, ","},
        {TOKEN_DOT, "."},
        {TOKEN_DOTDOT, ".."},
        {TOKEN_ELLIPSIS, "..."},
        {TOKEN_QUESTION, "?"},
        {TOKEN_SEMICOLON, ";"},
        {TOKEN_COLON, ":"},
        {0}};

    for (int i = 0; tokens[i].type; i++) {
        Token token = nextToken(&tokens[i].value);

        // printf("Token: Type = %d : %d , line = %d, offset = %d, Value = '%s'\n", token.type, tokens[i].type, token.lineCount, token.lineOffset, token.value);
        assert(token.type == tokens[i].type);
    }
}

void testString() {
    const char *source_code = "\"String\" \"a";
    int len = 18;

    Token token = nextToken(&source_code);
    assert(token.type == TOKEN_STRING);

    token = nextToken(&source_code);
    assert(token.type == TOKEN_UNEXPECTED);
}

void testChar() {
    const char *source_code = "'b' 'c";
    int len = 18;

    Token token = nextToken(&source_code);
    // printf("Token: Type = %d, line = %d, offset = %d, Value = '%s'\n", token.type, token.lineCount, token.lineOffset, token.value);
    assert(token.type == TOKEN_CHAR_STRING);

    token = nextToken(&source_code);
    assert(token.type == TOKEN_UNEXPECTED);
}

void testKeywords() {

const struct KeywordToken langaugeKeywords[] = {
    {TOKEN_INT, "int"}, 
    {TOKEN_I8, "i8"},
    {TOKEN_U8, "u8"},
    {TOKEN_I16, "i16"},
    {TOKEN_U16, "u16"},
    {TOKEN_I32, "i32"},
    {TOKEN_U32, "u32"},
    {TOKEN_I64, "i64"},
    {TOKEN_U64, "u64"},
    {TOKEN_F64, "f64"},
    {TOKEN_F32, "f32"},
    {TOKEN_BOOLEAN, "boolean"},
    {TOKEN_TRUE, "true"},
    {TOKEN_FALSE, "false"},
    {TOKEN_VOID, "void"}, 
    {TOKEN_CHAR, "char"}, 
    {TOKEN_NULL, "NULL"},

    {TOKEN_RETRUN, "return"}, 
    {TOKEN_IF, "if"}, 
    {TOKEN_ELSE, "else"}, 
    {TOKEN_FOR, "for"}, 
    {TOKEN_WHILE, "while"}, 
    // "double", 
    // "float",
    {TOKEN_TYPEDEF, "typedef"},
    {TOKEN_STRUCT, "struct"},
    {TOKEN_UNION, "union"},    
    {TOKEN_CLASS, "class"},
    {TOKEN_SWITCH, "switch"},
    {TOKEN_CASE, "case"},
    {TOKEN_DEFAULT, "default"},
    {TOKEN_DO, "do"},
    {TOKEN_BREAK, "break"},
    {TOKEN_CONTINUE, "continue"},
    {TOKEN_ENUM, "enum"},
    {TOKEN_GOTO, "goto"},
    // "static",
    // "const",
    NULL
};


    for (int i = 0; langaugeKeywords[i].type; i++) {
        Token token = nextToken(&langaugeKeywords[i].value);

        // printf("Token: Type = %d : %d , line = %d, offset = %d, Value = '%s'\n", token.type, langaugeKeywords[i].type, token.lineCount, token.lineOffset, token.value);
        assert(token.type == langaugeKeywords[i].type);
    }
}

int main() {
    testSomeCode();
    testNumber();
    testId();
    testStrip();
    testTokenTypes();
    testString();
    testChar();
    testKeywords();
    return 0;
}
