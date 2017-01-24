/*
 * pthread_recv.c
 *
 *  Created on: Oct 17, 2016
 *      Author: adan
 */

#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include "static_val.h"
#include "array_queue.h"

//互交一次
int recv_once(struct static_val *s_val, int _pid, int sock_tmp)
{ //客户端IO 临时变量
  int data_size;
  struct data_frame buf;
  //接收数据--epoll 水平触发,这里本来就已经recv 完毕,一次recv 肯定能马上获取全部数据
  //因为已经被提出了epoll, 所以这里用非异步接收, 保证数据接受安全性--其实MSG_DONTWAIT 也是可以的
  memset(&buf,0,sizeof(struct data_frame));
  data_size = recv(sock_tmp, &buf, sizeof(struct data_frame), MSG_DONTWAIT);
  if(data_size < 0)//接收数据错误?
    { printf("io pthread.%d socket %d recv an data from client fail, errno:%d\n",_pid,sock_tmp,errno);return -1; }
  //解密数据
  encode_open(&buf);
  //显示收到的数据
  printf("io pthread.%d recv '%d-%d-%d-%s-%s-%d-%d-%d-%d-%d-%s-%s' from client\n",_pid,buf.encode,buf.comm,buf.uid,buf.uname,buf.upw,buf.ip,buf.age,buf.sex,buf.port,buf.u_backup,buf.s_backup,buf.buf);
  //*********************************************
  //open data 解析数据报
  int tmp;
  //tmp = open_data(s_val,_pid,&buf);//无论查询结果如何，直接交给pickup_data
  //pickup data 打包结果回发
  if(pickup_data(s_val,sock_tmp,tmp) == -1)
    { printf("re_send data fail, once touch error, unknow server fail\n");return -1; }
  return 0;//正确返回
}

//IO pthread 线程函数
void* fpthread_recv(struct static_val *s_val)
{ //继承任务队列
  struct aq_info *pm_recv = s_val->pm_recv;
  //继承pid
  int _pid = s_val->pid_tmp;
  //再开锁
  pthread_mutex_lock(&s_val->tmp_mutex);pthread_cond_signal(&s_val->tmp_cond);pthread_mutex_unlock(&s_val->tmp_mutex);
  //线程正常开始工作
  int tmp;
  for(;;)//for loop
  { if(s_val->exit_sig != 0) //quit?
      break;
    //不是退出,马上暂停,等待listen pthread 呼叫,listen pthread 没呼叫一次,检查任务队列一次
    //如果检查到任务队列非空,则执行任务--另外listen pthread 线程也要加入检查任务队列的模块
    //listen pthread 也要自动检查任务队列,当队列不为空时,无论epoll_wait 有没有事件都要进行检查
    s_val->sig_io[_pid] = 0;//标志停止
    pthread_mutex_lock(&s_val->mutex_io[_pid]);
    pthread_cond_wait(&s_val->cond_io[_pid],&s_val->mutex_io[_pid]);
    pthread_mutex_unlock(&s_val->mutex_io[_pid]);
    s_val->sig_io[_pid] = 1;//标志启动

    if(aq_is_empty(pm_recv) == 0)
    { //当队列不为空时,有IO 任务
      int mission;
      element_type aq_buf[AS_buf_max];//队列缓冲区copy
      mission = aq_pop(pm_recv);//只是计算出队列容量--你还需要copy buf 取出缓冲区
      if(mission == pm_recv->aq_empty)//取队列元素=空队列
      { pthread_mutex_unlock(&pm_recv->tmp_mutex);//解锁
	continue;}//捞到空队列不作为
      else
      { //正常取出队列元素,拷贝出pop缓冲区
	memcpy(aq_buf,pm_recv->AS_pop,mission);
	//解锁
	pthread_mutex_unlock(&pm_recv->tmp_mutex);
	//
	//执行互交--注意：互交时后会自动close(socket)
	for(tmp = 0;tmp < mission;tmp++)
	  { if(recv_once(s_val, _pid, aq_buf[tmp]) == -1)
	      { s_val->err_io[_pid]++;
		printf("io pthread %d once interaction error, err_count: %d\n",_pid,s_val->err_io[_pid]); }
	    //
	    //因为无论IO 是否正常,sock_fd 总是被关闭,所以有：
	    // ****非一次性流程操作的服务器,这里还可以做手脚****
	    close(aq_buf[tmp]);
	    s_val->test_io[_pid]++;//线程服务次数累加
	  }
      }
    }
  }//for end
  //退出将PID 清0
  s_val->pid_io[_pid] = 0;
  pthread_mutex_destroy(&s_val->mutex_io[_pid]);
  pthread_cond_destroy(&s_val->cond_io[_pid]);
  //IO 线程成功退出
  printf("+++++++++++io pthread had quit+++++++++++\n");
  return NULL;
}
//
//开始IO 处理线程池
int start_precv(struct static_val *s_val)
{ //初始化io 线程池
  pthread_t pthread;
  for(;s_val->pid_tmp < pool_max;)
    { //创建IO 处理线程
      if(pthread_create(&pthread,NULL,(void*)fpthread_recv,s_val) != 0)
	{ printf("create io pthread fail, errno: %d",errno);s_val->exit_sig++;return -1; }
      //detach 模式,线程退出内核会回收一切,不保留信息记录,不会成为僵尸线程
      //
      if(pthread_detach(pthread) != 0)
	{ printf("io pthread set pthread_detach fail, errno: %d",errno);s_val->exit_sig++;return -1; }
      //等待线程创建完毕
      //
      pthread_mutex_lock(&s_val->tmp_mutex);
      pthread_cond_wait(&s_val->tmp_cond,&s_val->tmp_mutex);
      pthread_mutex_unlock(&s_val->tmp_mutex);
      //记录线程PID
      s_val->pid_io[s_val->pid_tmp] = pthread;
      printf("precv-No.%d pthread has started\n",s_val->pid_tmp);
      s_val->pid_tmp++;//堆数组序号+1
    }
  //创建完毕后销毁tmp 锁和条件变量（io pthread 专属,也因为这样,它会被安排到最后才创建）
  //
  destroy_mutex_cond_tmp(s_val);
  s_val->pid_tmp = pool_max;//??
  //创建成功
  printf("[[[[[[ all precv pthread init success ]]]]]]]\n");
  return 0;
}
//
//摧毁tmp 锁和条件变量
void destroy_mutex_cond_tmp(struct static_val *s_val)
{ int s;
  s = pthread_mutex_destroy(&s_val->tmp_mutex);
  if (s != 0)
    printf("pthread_mutex_destroy fail\n");

  s = pthread_cond_destroy(&s_val->tmp_cond);
  if (s != 0)
    printf("pthread_cond_destroy fail\n");
  return;
}
