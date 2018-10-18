#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include "pl0_lex.h"

const char * TOKEN_RESERVED_WORDS[NRW] = {"var", "const", "procedure", "begin", "end", "if", "then", "do", "while", "call", "odd"};
const char * TOKEN_SYMBOLS[NSYM] = { "+", "-", "*", "/", "=", "!=", "<", "<=", ">", ">=", "(", ")", ",", ";", ".", ":=" };

// 判断是否为数字
int number(char t){
    if(t >= '0' && t <= '9')
        return 1;
    return 0;
}

// 判断是否为字母
int character(char t){
    if((t >= 'A' && t <= 'Z') || (t >= 'a' && t <= 'z'))
        return 1;
    return 0;
}

// 判断是否为空格、标点等
int space_judge(char t){
    if(t < '0' || (t < 'A' && t > '9') || (t > 'Z' && t < 'a') || t > 'z')
        if(t != '_')
            return 1;
    return 0;
}

// 判断字符类型
int char_type_judge(char t){
    if(number(t) == 1)
        return 1;
    if(character(t) == 1)
        return 2;
    if(t == '_')
        return 3;
    if(t == ' ' || t == '\t')
        return 4;
    if(t == '/')
        return 5;
    if(t == '\n')
        return 6;
    if(t == '!' || t == '>' || t == '<' || t == ':')
        return 7;
    return 0;
}

// 判断是否为标识符
int identifier_judge(char token[11]){
    if(character(token[0]) == 1 || token[0] == '_'){
        for(int i = 1; token[i] != '\0'; ++i)
            if((space_judge(token[i]) == 1))
                return 0;
        return 1;
    }
    return 0;
}

// 判断是否为整数
int number_judge(char token[11]){
    for(int i = 0; i < strlen(token) ; ++i)
        if(number(token[i]) == 0)
            return 0;
    return 1;
}

// 判断是否为符号
int symbol_judge(char token[11]){
    for(int i = 0; i < 16; ++i)
        if(strcmp(token, TOKEN_SYMBOLS[i]) == 0)
            return i + 1;
    return 0;
}

// 判断是否为保留字
int reserved_word_judge(char token[11]){
    for(int i = 0; i < 11; ++i)
        if(strcmp(token, TOKEN_RESERVED_WORDS[i]) == 0)
            return i + 1;
    return 0;
}

// 字符串转换为整数
int str_toint(char token[11]){
    int num = 0;
    for(int i = 0; i < strlen(token); ++i)
        num = num * 10 + (token[i] - '0');
    return num;
}

// 判断 token 类型
void token_type(char token[11]){
    compiler->lex->last_token_type = TOKEN_NULL;
    if(token[0] != '_'){
        switch(reserved_word_judge(token)){
            case 1:
                compiler->lex->last_token_type = TOKEN_VAR;
                break;
            case 2:
                compiler->lex->last_token_type = TOKEN_CONST;
                break;
            case 3:
                compiler->lex->last_token_type = TOKEN_PROCEDURE;
                break;
            case 4:
                compiler->lex->last_token_type = TOKEN_BEGIN;
                break;
            case 5:
                compiler->lex->last_token_type = TOKEN_END;
                break;
            case 6:
                compiler->lex->last_token_type = TOKEN_IF;
                break;
            case 7:
                compiler->lex->last_token_type = TOKEN_THEN;
                break;
            case 8:
                compiler->lex->last_token_type = TOKEN_DO;
                break;
            case 9:
                compiler->lex->last_token_type = TOKEN_WHILE;
                break;
            case 10:
                compiler->lex->last_token_type = TOKEN_CALL;
                break;
            case 11:
                compiler->lex->last_token_type = TOKEN_ODD;
                break;
            default:
                compiler->lex->last_token_type = TOKEN_NULL;
                break;
        }
    }
    if(compiler->lex->last_token_type == TOKEN_NULL){
        if(identifier_judge(token)){
            compiler->lex->last_token_type = TOKEN_IDENTIFIER;
            strcpy(compiler->lex->last_id ,token);
        }
        else if(number_judge(token)){
            compiler->lex->last_token_type = TOKEN_NUMBER;
            compiler->lex->last_num = str_toint(token);
        }
        else{
            switch(symbol_judge(token)){
                case 1:
                    compiler->lex->last_token_type = TOKEN_PLUS;
                    break;
                case 2:
                    compiler->lex->last_token_type = TOKEN_MINUS;
                    break;
                case 3:
                    compiler->lex->last_token_type = TOKEN_TIMES;
                    break;
                case 4:
                    compiler->lex->last_token_type = TOKEN_SLASH;
                    break;
                case 5:
                    compiler->lex->last_token_type = TOKEN_EQU;
                    break;
                case 6:
                    compiler->lex->last_token_type = TOKEN_NEQ;
                    break;
                case 7:
                    compiler->lex->last_token_type = TOKEN_LES;
                    break;
                case 8:
                    compiler->lex->last_token_type = TOKEN_LEQ;
                    break;
                case 9:
                    compiler->lex->last_token_type = TOKEN_GTR;
                    break;
                case 10:
                    compiler->lex->last_token_type = TOKEN_GEQ;
                    break;
                case 11:
                    compiler->lex->last_token_type = TOKEN_LPAREN;
                    break;
                case 12:
                    compiler->lex->last_token_type = TOKEN_RPAREN;
                    break;
                case 13:
                    compiler->lex->last_token_type = TOKEN_COMMA;
                    break;
                case 14:
                    compiler->lex->last_token_type = TOKEN_SEMICOLON;
                    break;
                case 15:
                    compiler->lex->last_token_type = TOKEN_PERIOD;
                    break;
                case 16:
                    compiler->lex->last_token_type = TOKEN_BECOMES;
                    break;
                default:
                    compiler->lex->last_token_type = TOKEN_NULL;
                    break;
            }
        }
    }
}

// 状态机
int station(char c, int state){
    switch(state % 20){
        case 0: // 初判断字符可能所属的类型
            switch(char_type_judge(c)){
                // 预计为数字
                case 1:
                    state = 1;
                    break;
                // 预计为标识符
                case 2:
                case 3:
                    state = 2;
                    break;
                // 空格、tab 直接跳过
                case 4:
                    state = 12;
                    break;
                // 判断是否为注释
                case 5:
                    state = 3;
                    break;
                // 换行
                case 6:
                    state = 4;
                    break;
                case 7:
                    state = 10;
                    break;
                default:
                    state = 11;
                    break;
            }
            break;
        case 1: // 数字
            switch(char_type_judge(c)){
                case 1:
                    state = 1;
                    break;
                default:
                    state = 9;
                    break;
            }
            break;
        case 2: // 标识符、保留字
            switch(char_type_judge(c)){
                case 1:
                case 2:
                case 3:
                    state = 2;
                    break;
                default:
                    state = 9;
                    break;
            }
            break;
        case 3: // 是否为注释
            switch(c){
                case '/':
                    state = 5;
                    break;
                case '*':
                    state = 6;
                    break;
                default:
                    state = 9;
                    break;
            }
            break;
        case 5: // 行注释跳过
            switch(c){
                case '\n':
                    state = 4;
                    break;
                default:
                    state = 5;
                    break;
            }
            break;
        case 6: // 块注释跳过
            switch(c){
                case '*':
                    state = 7;
                    break;
                case '\n':
                    state = 8;
                    break;
                default:
                    state = 6;
                    break;
            }
            break;
        case 7: // 块注释中出现 *，判断注释是否结束
            switch(c){
                case '/':
                    state = 13;
                    break;
                default:
                    state = 6;
                    break;
            }
            break;
        case 10:
            switch(c){
                case '=':
                    state = 11;
                    break;
                default:
                    state = 9;
                    break;
            }
            break;
        default:
            state = 11;
            break;
    }
    return state;
}

PL0Lex * PL0Lex_create(PL0Compiler * parent)
{
	PL0Lex * lex = (PL0Lex *) calloc(1, sizeof(PL0Lex));
	lex->compiler = parent;

	return lex;
}

BOOL PL0Lex_destroy(PL0Lex * lex)
{
	free(lex);
	return TRUE;
}

BOOL PL0Lex_get_token(PL0Lex * lex)
{
	char c;
    char token[11];
    int state = 0;
    int num_long = 0;
    int t_num = 0;
    lex->line_begin = 0;
    lex->line_end = 0;
    memset(token, 0, sizeof(token));
    if((c = getc(fin)) != EOF){
        state = station(c, state);
        lex->line_begin = char_count;
        while(1){
            if(state == 0){
                t_num = 0;
                // memset(token, 0, sizeof(token));
                lex->line_begin = char_count;
                lex->line_end = char_count;
                lex->line_end++;
                token[t_num++] = c;
                if((c = getc(fin)) != EOF){
                        char_count++;
                        memset(token, 0, sizeof(token));
                    state = station(c,state);
                }
            }
            else if(state == 1 || state == 2 || state == 3){
                lex->line_end = char_count;
                lex->line_end++;
                token[t_num++] = c;
                // 判断数字长度是否符合要求
                if(state == 1){
                    if(num_long < MAX_NUM_LEN)
                        num_long ++;
                    else
                        return FALSE;
                }
                if((c = getc(fin)) != EOF){
                    char_count++;
                    state = station(c,state);
                }
            }
            else if(state == 4){
                state = 0;
                if((c = getc(fin)) != EOF){
                    l_num++;
                    char_count = 0;
                    lex->line_begin = char_count;
                    t_num = 0;
                    state = station(c,state);
                }
                // break;
            }
            else if(state == 5){
                // token[t_num] = '\0';
                memset(token, 0, sizeof(token));
                if((c = getc(fin)) != EOF)
                    state = station(c,state);
                else
                    return FALSE;
            }
            else if(state == 6){
                memset(token, 0, sizeof(token));
                if((c = getc(fin)) != EOF)
                    char_count++;
                    state = station(c,state);
            }
            else if(state == 7){
                memset(token, 0, sizeof(token));
                if((c = getc(fin)) != EOF){
                    char_count++;
                    state = station(c,state);
                }
            }
            else if(state == 8){
                state = 6;
                memset(token, 0, sizeof(token));
                char_count = 0;
                l_num++;
                if((c = getc(fin)) != EOF)
                    state = station(c,state);
            }
            else if(state == 9){
                fseek(fin, -1, SEEK_CUR);
                break;
            }
            else if(state == 10){
                lex->line_end = char_count;
                lex->line_end++;
                token[t_num++] = c;
                if((c = getc(fin)) != EOF){
                    char_count++;
                    state = station(c,state);
                }
            }
            else if(state == 11){
                lex->line_end = char_count;
                lex->line_end++;
                token[t_num++] = c;
                if((c = getc(fin)) != EOF){
                    char_count++;
                    fseek(fin, -1, SEEK_CUR);
                }
                break;
            }
            else if(state == 12){
                memset(token, 0, sizeof(token));
                t_num = 0;
                state = 0;
                if((c = getc(fin)) != EOF){
                    char_count++;
                    lex->line_begin = char_count;
                    //lex->line_end = char_count;
                    //lex->line_end++;
                    //token[t_num++] = c;
                    state = station(c,state);
                }
            }
            else if(state == 13) {
                state = 0;
                if ((c = getc(fin)) != EOF) {
                    char_count++;
                    lex->line_begin = char_count;
                    t_num = 0;
                    state = station(c, state);
                }
            }
        }
        if(token != '\0'){
            lex->line_end--;
            token_type(token);
            lex->line_num = l_num;
        }
        return TRUE;
    }
	return FALSE;
}
