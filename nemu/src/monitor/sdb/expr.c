/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string.h>
#include <stdbool.h>
enum {
  TK_NOTYPE = 256, 
  REG = 1, 
  HEX = 2, 
  NUM = 3,
  TK_EQ, 
  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces

  {"\\+", '+'},         // plus    
  {"-", '-'},           //sub
  {"\\*", '*'},         //mul
  {"/", '/'},           //div

  {"\\(", '('},             // LEFT
  {"\\)", ')'},             //RIGHT

  {"\\$[a-zA-Z0-9]+",REG},      //Reg
  {"0[xX][0-9a-fA-F]+",HEX},    //HEX
  {"[0-9]*",NUM},               //Num

  {"==", TK_EQ},        // equal


};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case '+':
            tokens[nr_token++].type = rules[i].token_type;
            break;
          case '-':
            tokens[nr_token++].type = rules[i].token_type;
            break;
          case '*':
            tokens[nr_token++].type = rules[i].token_type;
            break;
          case '/':
            tokens[nr_token++].type = rules[i].token_type;
            break;
          case '(':
            tokens[nr_token++].type = rules[i].token_type;
            break;
          case ')':
            tokens[nr_token++].type = rules[i].token_type;
            break;
          case REG:
            strncpy(tokens[nr_token].str, e + position - substr_len, substr_len);
            tokens[nr_token++].type = rules[i].token_type;
            break; 
          case HEX:
            strncpy(tokens[nr_token].str, e + position - substr_len, substr_len);
            tokens[nr_token++].type = rules[i].token_type;
            break;
          case NUM:
            strncpy(tokens[nr_token].str, e + position - substr_len, substr_len);
            tokens[nr_token++].type = rules[i].token_type;
            break;         
          default: TODO();
            break;
        }
      break;
        
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


bool check_parentheses(int p, int q){
  char stack_parentheses[32]={};
  int head =  0;
  int i ;
  if((tokens[p].type == '(') && (tokens[q].type == ')')){
    for(i = p; i <= q; i++){
      switch (tokens[i].type)
      {
      case '(':
        stack_parentheses[head] = '(';
        head ++;
        break;
      case ')':
        if(stack_parentheses[head-1] == '('){
        head --;
        }
        else{
        stack_parentheses[head] = ')';
        head ++;
        }
        break;
      default:
        break;
      }
      if(head == 0){break;}
    }
    if((i == q)&&(head == 0)){return true;}
    else{return false;}
  }
  else{
    return false;
  }
}

uint32_t eval(int p, int q){
  if (p > q) {
    /* Bad expression */
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    uint32_t num = 0;
    printf("!\n");
    sscanf(tokens[p].str,"%u",&num);
    // printf("%u\n",num);
    return num;
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }
  else {
    //op = the position of 主运算符 in the token expression;
    int op = 0;
    int op_type = 0;
    int prior ;
    int mini_prior = 3;
    int in_parentheses = 0;
    for(int i = p; i <= q; i++){
      switch (tokens[i].type)
      {
      case '+':
        prior = 1;
        break;
      case '-':
        prior = 1;
        break;
      case '*':
        prior = 2;
        break;
      case '/':
        prior = 2;
        break;
      case '(':
        in_parentheses = 1;
        prior = 3;
        break;
      case ')':
        in_parentheses = 0;
        prior = 3;
        break;
      default:
        prior = 3;
        break;
      }
      if(in_parentheses){
        prior = 3;
      }
      if(prior <= mini_prior){
        op = i;
        op_type = tokens[i].type;
      }
    }
    int val1 = eval(p, op - 1);
    int val2 = eval(op + 1, q);
    switch (op_type) {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
      //default: //assert(0);
    }
  }

  return 0;
}



word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  uint32_t result = eval(0,nr_token-1);
  printf("%u\n",result);
  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}
