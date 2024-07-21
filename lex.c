#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <lex.h>

const struct KeywordToken keywords[] = {
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


int isKeyword(const char *str) {
    int a = MAX_TOKEN_LEN;
    for (int i = 0; keywords[i].type != NULL; i++) {
        if (strcmp(keywords[i].value, str) == 0) {
            return keywords[i].type;
        }
    }
    return TOKEN_IDENTIFIER;
}

int lineCount = 0;
int lineOffset = 0;

void lexInit() {
    lineCount = 0;
    lineOffset = 0;
}

inline char inc(const char **input) {
    char ch = *(*input)++;
    lineOffset++;
    if (ch == '\n') {
        lineCount++;
        lineOffset = 0;
    }
    return ch;
}

int stripWhitespace(const char **input) {
    // Skip whitespace
    int res = 0;
    while (isspace(**input)) {
        inc(input);
        res++;
    }
    return res;
}

int stripComment(const char **input, Token *token) {
    // Block Comment
    // Comment
    int res = 0;
    if (**input == '/') {
        res++;
        inc(input);
        if (**input == '*') {
            char lastc = inc(input);

            while (**input != '\0') {
                lastc = **input;
                inc(input);
                if (lastc == '*' && **input == '/') {
                    break;
                }
            }

            if (**input == '\0') {
                // unexpected EOF
                token->type = TOKEN_UNEXPECTED;
                return 0;
            }
            
            inc(input);
        } else if (**input == '/') {
            while (**input != '\n' && **input != '\0') {
                inc(input);
            }
        } else {
            // back out no comment
            *(*input)--;
            return 0;
        }
    }
    return res;
}

Token nextToken(const char **input) {
    Token token = {TOKEN_UNKNOWN};
    memset(token.value, 0, sizeof(token.value));

    while (stripWhitespace(input) || stripComment(input, &token) && token.type == TOKEN_UNKNOWN);

    token.lineCount = lineCount;
    token.lineOffset = lineOffset;

    if (token.type == TOKEN_UNEXPECTED) {
        return token;
    }

    int len = 0;

    // Identifiers and keywords
    if (isalpha(**input) || **input == '_') {
        while (isalnum(**input) || **input == '_' || **input == '$') {
            token.value[len++] = inc(input);
        }
        token.type = isKeyword(token.value);
        return token;
    }

    // Hex
    // Bin
    // Octal
    // Float
    if (**input == '0') {
        token.value[len++] = inc(input);
        while (**input == '_') {
            // strip underscore
            inc(input);
        }

        if (**input == 'x' || **input == 'X') {
            token.value[len++] = inc(input);
            while (**input && strchr("0123456789_aAbBcCdDeEfF", **input))  {
                if (**input != '_') {
                    token.value[len++] = **input;
                }
                inc(input);
            }
            token.type = TOKEN_NUMBER_HEX;
            return token;
        } else if (**input == 'b' || **input == 'B') {
            token.value[len++] = inc(input);
            while (**input == '_' || **input == '1' || **input == '0' )  {
                if (**input != '_') {
                    token.value[len++] = **input;
                }
                inc(input);
            }
            token.type = TOKEN_NUMBER_BINARY;
            return token;
        } else if (**input == '.') {
            token.value[len++] = inc(input);
            while (isdigit(**input) || **input == '_') {
                if (**input != '_') {
                    token.value[len++] = **input;
                }
                inc(input);
            }
            token.type = TOKEN_NUMBER_FLOAT;
            return token;
        } else if (**input == 'l' || **input == 'L') {
            // long zero
            token.value[len++] = inc(input);
            token.type = TOKEN_NUMBER_LONG;
            return token;
        } else if (!isdigit(**input)) {
            // just 0;
            token.type = TOKEN_NUMBER;
            return token;
        } 


        while (**input && strchr("_01234567.", **input)) {
            if (**input != '_') {
                token.value[len++] = **input;
            }
            inc(input);
        }
        token.type = TOKEN_NUMBER_OCT;
        return token;
    }

    // int / long / float
    if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        while (isdigit(**input)  || **input == '_' || **input == '.')  {
            if (**input == '.') {
                token.type = TOKEN_NUMBER_FLOAT;
            }

            if (**input != '_') {
                token.value[len++] = **input;
            }
            inc(input);
        }

        if (**input == 'l' || **input == 'L' && token.type != TOKEN_NUMBER_FLOAT) {
            token.type = TOKEN_NUMBER_LONG;
            token.value[len++] = inc(input);
        }

        return token;
    }

    // String
    if (**input == '"') {
        token.value[len++] = inc(input);
        while (**input != '"') {
            if (**input == '\0') {
                // unexpected EOF
                token.type = TOKEN_UNEXPECTED;
                return token;
            }
            token.value[len++] = inc(input);
        }
        token.value[len++] = inc(input);
        token.type = TOKEN_STRING;
        return token;
    }

    // Char
    if (**input == '\'') {
        token.value[len++] = inc(input);
        while (**input != '\'') {
            if (**input == '\0') {
                // unexpected EOF
                token.type = TOKEN_UNEXPECTED;
                return token;
            }
            token.value[len++] = inc(input);
        }
        token.value[len++] = inc(input);
        token.type = TOKEN_CHAR_STRING;
        return token;
    }

    switch (**input) {
        case '\0' : 
            token.type = TOKEN_EOF;
            return token;
        case '+' : // ++ += +
            token.value[len++] = inc(input);
            token.type = TOKEN_PLUS;

            if (**input == '+') {
                token.value[len++] = inc(input);
                token.type = TOKEN_PLUSPLUS;
            } else if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_PLUSEQUALS;
            }

            return token;
        case '-' : // -- -= - ->
            token.value[len++] = inc(input);
            token.type = TOKEN_MINUS;

            if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_MINUSEQUALS;
            } else if (**input == '-') {
                token.value[len++] = inc(input);
                token.type = TOKEN_MINUSMIUS;
            } else if (**input == '>') {
                token.value[len++] = inc(input);
                token.type = TOKEN_MINUSEGREATER;
            }
            
            return token;
        case '*' : // ** *= *
            token.value[len++] = inc(input);
            token.type = TOKEN_STAR;

            if (**input == '*') {
                token.value[len++] = inc(input);
                token.type = TOKEN_STARSTAR;
            } else if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_STAREQUALS;
            }

            return token;
        case '=' : // == = =>
            token.value[len++] = inc(input);
            token.type = TOKEN_EQUALS;

            if (**input == '>') {
                token.value[len++] = inc(input);
                token.type = TOKEN_EQUALSGREATER;
            } else if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_EQUALSEQUALS;
            }

            return token;
        case '%' : // %= %
            token.value[len++] = inc(input);
            token.type = TOKEN_PERCENT;

            if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_PERCENTEQUALS;
            }

            return token;
        case '!' : // != !
            token.value[len++] = inc(input);
            token.type = TOKEN_NOT;

            if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_NOTEQUALS;
            }

            return token;
        case '>' : // >= >
            token.value[len++] = inc(input);
            token.type = TOKEN_GREATER;

            if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_GREATEREQUALS;
            }

            return token;
        case '<' : // <= <
            token.value[len++] = inc(input);
            token.type = TOKEN_LESS;

            if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_LESSEQUALS;
            }

            return token;
        case '^' : // ^= ^
            token.value[len++] = inc(input);
            token.type = TOKEN_CARET;

            if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_CARETEQUALS;
            }

            return token;
        case '/' : // / /=
            token.value[len++] = inc(input);
            token.type = TOKEN_DIV;

            if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_DIVEQUALS;
            }

            return token;
        case '|' : // || | |=
            token.value[len++] = inc(input);
            token.type = TOKEN_OR;

            if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_OREQUALS;
            } else if (**input == '|') {
                token.value[len++] = inc(input);
                token.type = TOKEN_OROR;
            }

            return token;
        case '&' : // && & &=
            token.value[len++] = inc(input);
            token.type = TOKEN_AND;

            if (**input == '=') {
                token.value[len++] = inc(input);
                token.type = TOKEN_ANDEQUALS;
            } else if (**input == '&') {
                token.value[len++] = inc(input);
                token.type = TOKEN_ANDAND;
            }

            return token;
        case '.' : // ... .. .
            token.value[len++] = inc(input);
            token.type = TOKEN_DOT;
            if (**input == '.') {
                token.type = TOKEN_DOTDOT;
                token.value[len++] = inc(input);
                if (**input == '.') {
                    token.value[len++] = inc(input);
                    token.type = TOKEN_ELLIPSIS;
                }
            }
            
            return token;
        case '(' :
            token.value[0] =inc(input);
            token.type = TOKEN_L_PAREN;
            return token;
        case ')' :
            token.value[0] =inc(input);
            token.type = TOKEN_R_PAREN;
            return token;
        case '{' :
            token.value[0] =inc(input);
            token.type = TOKEN_L_BRACE;
            return token;
        case '}' :
            token.value[0] =inc(input);
            token.type = TOKEN_R_BRACE;
            return token;
        case '[' :
            token.value[0] =inc(input);
            token.type = TOKEN_L_BRACKET;
            return token;
        case ']' :
            token.value[0] =inc(input);
            token.type = TOKEN_R_BRACKET;
            return token;
        case ':' :
            token.value[0] =inc(input);
            token.type = TOKEN_COLON;
            return token;
        case ';' :
            token.value[0] =inc(input);
            token.type = TOKEN_SEMICOLON;
            return token;
        case ',' :
            token.value[0] =inc(input);
            token.type = TOKEN_COMMA;
            return token;
        case '~' :
            token.value[0] =inc(input);
            token.type = TOKEN_TILDE;
            return token;
        case '@' :
            token.value[0] =inc(input);
            token.type = TOKEN_AT;
            return token;
        case '?' :
            token.value[0] =inc(input);
            token.type = TOKEN_QUESTION;
            return token;
        default :
            printf("<unrecognised char %c>\n", **input);    
    }

    // Unknown token
    token.value[0] = inc(input);
    token.type = TOKEN_UNKNOWN;
    return token;
}
