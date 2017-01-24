/*
 * send_once.c
 *
 *  Created on: Nov 3, 2016
 *      Author: adan
 */
#include "static_val.h"
#include "data_comm_list.h"
#include <sys/types.h>
#include <sys/socket.h>

//循环测试所有数据帧--每一个socket 都遍历所有数据库操作
//
int test_once(int sock_tmp,int db_all)
{
  struct data_frame buf;
  memset(&buf,0,sizeof(struct data_frame));
  //选择性地打包数据
  //
  switch(db_all)
  {
    case 1:
      buf.comm = uinfo_clean;//平时不用，否则数据都清除了
      break;
    case 2:
      buf.comm = uinfo_add;//uid 直接用socket 值*7 代入入，增加活跃性
      buf.uid = sock_tmp*7;
      sscanf("123456","%s",&buf.uname);
      sscanf("654321","%s",&buf.upw);
      sprintf (&buf.buf, "iam_email-%d", buf.uid);
      buf.ip = 1;
      buf.port = 1;
      buf.u_backup = 1;
      break;
    case 3:
      buf.comm = uinfo_select_by_uid;
      buf.uid = 2;//永恒存在的测试数据，自己手动添加
      break;
    case 4:
      buf.comm = uinfo_change;
      buf.uid = 2;//永恒存在的测试数据，自己手动添加
      sscanf("123456","%s",&buf.uname);
      sscanf("654321","%s",&buf.upw);
      sprintf (&buf.buf, "iam_email-%d", buf.uid);
      buf.ip = 1;
      buf.port = 1;
      buf.u_backup = 1;
      break;
    //upicture
    case 8:
      buf.comm = upicture_clean;//平时不用，否则数据都清除了
      break;
    case 6:
      buf.comm = upicture_add;
      buf.uid = sock_tmp*7;
      sprintf (&buf.buf, "iam_picture-%d", buf.uid);
      break;
    case 7:
      buf.comm = upicture_select_by_uid;
      buf.uid = 2;//永恒存在的测试数据，自己手动添加
      break;
    case 5:
      buf.comm = upicture_change;
      buf.uid = 2;//永恒存在的测试数据，自己手动添加
      sprintf (&buf.buf, "iam_picture-%d", buf.uid);
      break;
    default:
      printf("input a error test command, cant pickup a data for test sending\n");
      return -1;//返回错误操作
  }
  //打包数据结束
  //
  //显示加密前的数据
  printf("send '||---%d-%d-%s-%s-%d-%d-%d-%s' to server\n",buf.comm,buf.uid,buf.uname,buf.upw,buf.ip,buf.port,buf.u_backup,buf.buf);
  //加密数据
  encode_pickup(&buf);
  //执行发送
  if(send(sock_tmp, &buf, sizeof(struct data_frame), 0) < 0)
  {
    close(sock_tmp);
    printf("\nsend() an data to server fail!\n");
    return -1;
  }
  //接受数据
  if(recv(sock_tmp, &buf, sizeof(struct data_frame), 0) < 0)
  {
    close(sock_tmp);
    printf("\nrecv() an data from server fail!\n");
    return -1;
  }
  //解密数据
  encode_open(&buf);
  //客户端解析数据--这个操作没有什么特别，只是显示正确与否

  //open_data(&buf);
  //显示解密后的数据
  printf("recv '%d---%d-%d-%s-%s-%d-%d-%d-%s' from server\n",buf.encode,buf.comm,buf.uid,buf.uname,buf.upw,buf.ip,buf.age,buf.sex,buf.port,buf.u_backup,buf.s_backup,buf.buf);

  close(sock_tmp);
  return 0;
}
