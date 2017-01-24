/*
 * array_stack.h
 *
 *  Created on: Nov 13, 2016
 *      Author: adan
 */

#define element_type int//int： 4个字节(固定)
//#define element_type void*//*(即指针变量): 32-4/64-8  个字节
//#define as_pbuf_max 4096*sizeof(element_type)
#define as_buf_max 4096
//*********************
//运行变量
//*********************
struct as_info{
  element_type as_buf[as_buf_max];
  short int as_cur_size;
  //*********************
  //运行常量
  //*********************
  short int as_buf_empty;
  short int as_buf_full;
  short int as_full;
  short int as_empty;
  short int as_push_ok;
};

//附：栈中没有负值

