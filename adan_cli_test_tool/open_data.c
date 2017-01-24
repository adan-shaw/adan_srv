/*
 * open_data.c
 *
 *  Created on: Nov 17, 2016
 *      Author: adan
 */
#include "data_comm_list.h"
#include "static_val.h"

//部分长定字符串
const char *uinfo_sclean = "call uinfo_clean ();";
const char *upicture_sclean = "call upicture_clean ();";

//解析数据帧
//
void open_data(struct data_frame* pdata_frame)
{
  switch(pdata_frame->comm)
  {
  //uinfo
  case remyself_uinfo_clean:
    printf("~!@#$%^&*(clean uinfo table success\n");
    break;
  case remyself_uinfo_add:
    printf("~!@#$%^&*(add uinfo table success, register success!\n");
    break;
  case remyself_uinfo_select_by_uid:
    printf("~!@#$%^&*(select uinfo table success, login success!\n");
    break;
  case remyself_uinfo_change:
    printf("~!@#$%^&*(change uinfo table success\n");
    break;
  //upicture
  case remyself_upicture_clean:
    printf("~!@#$%^&*(clean upicture table success\n");
    break;
  case remyself_upicture_add:
    printf("~!@#$%^&*(add upicture table success !\n");
    break;
  case remyself_upicture_select_by_uid:
    printf("~!@#$%^&*(select upicture table success !\n");
    break;
  case remyself_upicture_change:
    printf("~!@#$%^&*(change upicture table success\n");
    break;
  case remyself_open_error:
    printf("~!@#$%^&*(the server cant open the package from us!\n");
    break;
  case remyself_query_error:
    printf("~!@#$%^&*(the server query db fail\n");
    break;
  default:
    printf("~!@#$%^&*(server return a unknow package\n");
    break;
}

}
//数据帧参考
//
/*

  pdata_frame->comm = 1;
  pdata_frame->uid = 2;
  pdata_frame->ip = 3;
  pdata_frame->port = 4;
  pdata_frame->backup = 5;
  sscanf("654321","%s",pdata_frame->uname);
  sscanf("654321","%s",pdata_frame->upw);
  sscanf("hello-client,laid-with-me","%s",pdata_frame->buf);

 */
