#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 定义词法单元类型
typedef enum {
    TOKEN_INT,     // 整数
    TOKEN_PLUS,    // +
    TOKEN_MINUS,   // -
    TOKEN_STAR,    // *
    TOKEN_SLASH,   // /
    TOKEN_LPAREN,  // (
    TOKEN_RPAREN,  // )
    TOKEN_EOF      // 结束标记
} TokenType;

// 词法单元结构
typedef struct {
    TokenType type;
    int value;     // 当TOKEN_INT时使用
} Token;

// 解析器状态
typedef struct {
    char* input;   // 输入字符串
    int position;  // 当前位置
    Token current; // 当前词法单元
} Parser;

// 函数声明
void initParser(Parser* parser, char* input);
void advance(Parser* parser);
void consume(Parser* parser, TokenType type);
int expression(Parser* parser);
int term(Parser* parser);
int factor(Parser* parser);
void error(const char* message);

// 初始化解析器
void initParser(Parser* parser, char* input) {
    parser->input = input;
    parser->position = 0;
    advance(parser); // 获取第一个词法单元
}

// 前进到下一个词法单元
void advance(Parser* parser) {
    char* input = parser->input;
    int pos = parser->position;
    
    // 跳过空白字符
    while (input[pos] != '\0' && isspace(input[pos])) {
        pos++;
    }
    
    // 检查是否到达字符串末尾
    if (input[pos] == '\0') {
        parser->current.type = TOKEN_EOF;
        parser->position = pos;
        return;
    }
    
    // 解析数字
    if (isdigit(input[pos])) {
        int value = 0;
        while (input[pos] != '\0' && isdigit(input[pos])) {
            value = value * 10 + (input[pos] - '0');
            pos++;
        }
        parser->current.type = TOKEN_INT;
        parser->current.value = value;
        parser->position = pos;
        return;
    }
    
    // 解析操作符
    switch (input[pos]) {
        case '+':
            parser->current.type = TOKEN_PLUS;
            break;
        case '-':
            parser->current.type = TOKEN_MINUS;
            break;
        case '*':
            parser->current.type = TOKEN_STAR;
            break;
        case '/':
            parser->current.type = TOKEN_SLASH;
            break;
        case '(':
            parser->current.type = TOKEN_LPAREN;
            break;
        case ')':
            parser->current.type = TOKEN_RPAREN;
            break;
        default:
            error("不认识的字符");
            return;
    }
    
    parser->position = pos + 1;
}

// 消费一个预期的词法单元
void consume(Parser* parser, TokenType type) {
    if (parser->current.type == type) {
        advance(parser);
    } else {
        char message[100];
        sprintf(message, "预期的词法单元类型不匹配，期望 %d，得到 %d", type, parser->current.type);
        error(message);
    }
}

// 语法规则：expression -> term (('+' | '-') term)*
int expression(Parser* parser) {
    int result = term(parser);
    
    while (parser->current.type == TOKEN_PLUS || parser->current.type == TOKEN_MINUS) {
        TokenType op = parser->current.type;
        advance(parser);
        int right = term(parser);
        
        if (op == TOKEN_PLUS) {
            result += right;
        } else {
            result -= right;
        }
    }
    
    return result;
}

// 语法规则：term -> factor (('*' | '/') factor)*
int term(Parser* parser) {
    int result = factor(parser);
    
    while (parser->current.type == TOKEN_STAR || parser->current.type == TOKEN_SLASH) {
        TokenType op = parser->current.type;
        advance(parser);
        int right = factor(parser);
        
        if (op == TOKEN_STAR) {
            result *= right;
        } else {
            if (right == 0) {
                error("除以零错误");
                return 0;
            }
            result /= right;
        }
    }
    
    return result;
}

// 语法规则：factor -> INTEGER | '(' expression ')'
int factor(Parser* parser) {
    if (parser->current.type == TOKEN_INT) {
        int value = parser->current.value;
        advance(parser);
        return value;
    } else if (parser->current.type == TOKEN_LPAREN) {
        advance(parser);
        int result = expression(parser);
        consume(parser, TOKEN_RPAREN);
        return result;
    } else {
        error("预期整数或左括号");
        return 0;
    }
}

// 错误处理
void error(const char* message) {
    fprintf(stderr, "错误: %s\n", message);
    exit(1);
}

// 解析输入并计算结果
int parse(char* input) {
    Parser parser;
    initParser(&parser, input);
    int result = expression(&parser);
    
    // 确保所有输入都被解析
    if (parser.current.type != TOKEN_EOF) {
        error("多余的输入");
    }
    
    return result;
}

// 主函数示例
int main() {
    char input[100];
    
    printf("输入一个表达式 (例如 '3 + 4 * 2'): ");
    fgets(input, sizeof(input), stdin);
    
    int result = parse(input);
    printf("结果: %d\n", result);
    
    return 0;
}