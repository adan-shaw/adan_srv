/*
 * open_data.c
 *
 *  Created on: Nov 17, 2016
 *      Author: adan
 */

#include "static_val.h"

//部分长定字符串
const char *uinfo_sclean = "call uinfo_clean ();";
const char *upicture_sclean = "call upicture_clean ();";
//
//解析数据帧

/*
int open_data(struct static_val *s_val, struct data_frame* pdata_frame)
{
  char sql_buf[sql_str_max];	//sql 查询字符缓冲区
  MYSQL_RES *g_res; 		// mysql 记录集
  int re_val;			//解析数据后分辨客户端选择的请求，这里保存辨析，以返回
  switch(pdata_frame->comm)
  { //uinfo
    case uinfo_clean:
      if(s_val->fp_set.pmysql_real_query(&s_val->conn_io, uinfo_sclean, strlen(uinfo_sclean)) == 1)
	{printf("open_data() mysql_real_query fail--%s\n",s_val->fp_set.pmysql_error (&s_val->conn_io));return -1;}//返回错误操作
      printf("*******the server has upload sql string %s********\n",uinfo_sclean);
      return remyself_uinfo_clean;
    case uinfo_add://city默认了，日期默认今天--除非你修改数据帧
      sprintf (&sql_buf, "call uinfo_add (%d,\"%s\",\"%s\",%d,%d,\"%s\",\"china\",0);", pdata_frame->uid, pdata_frame->uname, pdata_frame->upw, pdata_frame->ip, pdata_frame->port,pdata_frame->buf);
      re_val = remyself_uinfo_add;
      printf("*******the server has upload sql string %s********\n",sql_buf);
      break;
    case uinfo_select_by_uid:
      sprintf (&sql_buf, "call uinfo_select_by_uid (%d);", pdata_frame->uid);
      re_val = remyself_uinfo_select_by_uid;
      printf("*******the server has upload sql string %s********\n",sql_buf);
      break;
    case uinfo_change:
      sprintf (&sql_buf, "call uinfo_change (%d,\"%s\",\"%s\",%d,%d,\"%s\",\"china\");", pdata_frame->uid, pdata_frame->uname, pdata_frame->upw, pdata_frame->ip, pdata_frame->port,pdata_frame->buf);
      re_val = remyself_uinfo_change;
      printf("*******the server has upload sql string %s********\n",sql_buf);
      break;
    //upicture
    case upicture_clean:
      if(s_val->fp_set.pmysql_real_query(&s_val->conn_io, upicture_sclean, strlen(upicture_sclean)) == 1)
	{printf("open_data() mysql_real_query fail--%s\n",s_val->fp_set.pmysql_error (&s_val->conn_io));return -1;}//返回错误操作
      printf("*******the server has upload sql string %s********\n",upicture_sclean);
      return remyself_upicture_clean;
    case upicture_add:
      sprintf (&sql_buf, "call uinfo_add (%d,\"%s\");", pdata_frame->uid, pdata_frame->buf);
      re_val = remyself_upicture_add;
      printf("*******the server has upload sql string %s********\n",sql_buf);
      break;
    case upicture_select_by_uid:
      sprintf (&sql_buf, "call uinfo_add (%d);", pdata_frame->uid);
      re_val = remyself_upicture_select_by_uid;
      printf("*******the server has upload sql string %s********\n",sql_buf);
      break;
    case upicture_change:
      sprintf (&sql_buf, "call uinfo_add (%d,\"%s\");", pdata_frame->uid, pdata_frame->buf);
      re_val = remyself_upicture_change;
      printf("*******the server has upload sql string %s********\n",sql_buf);
      break;
    default:
      return remyself_open_error;//返回错误操作
  }
  //
  //提交查询语句
  if(s_val->fp_set.pmysql_real_query(&s_val->conn_io, &sql_buf, sql_str_max) == 1)
    {printf("open_data() mysql_real_query fail--%s\n",s_val->fp_set.pmysql_error (&s_val->conn_io));return remyself_query_error;}//返回错误操作
  //处理结果集--暂时都没有统一做数据库结果集处理
  do
  { g_res = s_val->fp_set.pmysql_store_result(&s_val->conn_io);
    s_val->fp_set.pmysql_free_result(g_res);
  }while( !s_val->fp_set.pmysql_next_result( &s_val->conn_io ) );

  return re_val;//返回正确操作
}
*/
