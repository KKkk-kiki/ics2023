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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  bool used;
  char expr[100];//大小多少比较合适？
  uint32_t value;
  /* TODO: Add more members if necessary */

} WP;
static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

//从free_链表中返回一个空闲的监视点结构
WP* new_wp(){
  WP* p = NULL;
  if(free_!= NULL){
    p = free_;
    free_ = free_ -> next;
    p -> used = true;
    if(head == NULL){
      head = p;
      p -> next = NULL;
    }
    else{
      p -> next = head -> next;
      head = p;
    }
    return p;
  }
  else{
    printf("No free watchpoint\n");
    //assert(0);
    return NULL;
  }
}

//将wp归还到free_链表中
void free_wp(WP *wp){
  
  wp -> used = false;
  memset(wp -> expr, '\0', sizeof(wp -> expr));
  if(head == wp){
    head = head -> next;
    return;
  }
  else{
    for(WP* p = head; p -> next != NULL; p = p -> next){
      if(p -> next == wp){
        p -> next = p -> next -> next;
      }
    }
    wp -> next = free_ -> next;
    free_ = wp;
    return;
  }

  
}



void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    wp_pool[i].used = false;
    wp_pool[i].value = 0;
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

//设置监视器
void set_watchpoint(char *args){
  WP* p = new_wp();
  strcpy(p -> expr, args);
  bool success;
  p -> value = expr(args, &success);
  printf("Set watchpoint No.%d success.\n", p -> NO);
  return;
}

//删除监视器
void del_watchpoint(int NO){
  for(int i = 0; i < NR_WP; i++){
    if((wp_pool[i].NO == NO) && (wp_pool[i].used)){
      free_wp(&wp_pool[i]);
      printf("Delete watchpoint No.%d success.\n", NO);
      return;
    }
  }
  printf("Failure : Cannot Delete watchpoint No.%d.\n", NO);
  return;
}

//查看监视器
void display_watchpoint(void){
  int i = 0;
  int flag = 1;
  for(i = 0; i < NR_WP; i++){
    if(wp_pool[i].used){
      flag = 0;
      printf("Watchpoint No.%d --- expr : %s , value : %u\n", wp_pool[i].NO, wp_pool[i].expr, wp_pool[i].value);
    }
  }
  if(flag){
    printf("No Watchpoint now\n");
  }
  return;
}

//扫描watchpoint，查看是否有值被改变
void check_watchpoint(void){
  int i;
  for(i = 0; i < NR_WP; i++){
    if(wp_pool[i].used){
      bool success = false;
      uint32_t temp = expr(wp_pool[i].expr, &success);
      if(success){
        if(temp != wp_pool[i].value){
          printf("Watchpoint No.%d --- expr : %s\nold_value : %u , new_value : %u\n", wp_pool[i].NO, wp_pool[i].expr, wp_pool[i].value, temp);
          nemu_state.state = NEMU_STOP;
          return;
        }
      }
      else{
        printf("expr error\n");
      }
    }
  }
}

