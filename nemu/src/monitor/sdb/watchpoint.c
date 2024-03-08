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
  for(WP* p = free_; p -> next != NULL; p = p -> next){
    if(!p -> used){
      p -> used = true;
      if(head == NULL){
        head = p;
      }
    }
    return p;
  }
  printf("No free point\n");
  assert(0);
  return NULL;
  
}

//将wp归还到free_链表中
void free_wp(WP *wp){
  if(head -> NO == wp -> NO){
    head -> used = false;
    head = NULL;
    return;
  }
  for(WP* p = head; p -> next != NULL; p = p -> next){
    if(p -> next -> NO == wp -> NO){
      p -> next = p -> next -> next;
      p -> next -> used = false;

    }
  }
  return;
}



void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
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
    if(wp_pool[i].NO == NO){
      free_wp(&wp_pool[i]);
      printf("Delete watchpoint No.%d success.\n", NO);
      return;
    }
  }
  printf("Failure : Cannot Delete watchpoint No.%d.\n", NO);
  return;
}

//查看监视器
void display_watchpoint(){
  int i = 0;
  for(int i = 0; i < NR_WP; i++){
    if(wp_pool[i].used){
      printf("Watchpoint No.%d --- expr : %s , value : %u\n", wp_pool[i].NO, wp_pool[i].expr, wp_pool[i].value);
    }
  }
  
  if(i == NR_WP){
    printf("No Watchpoint now\n");
  }
  return;
}

