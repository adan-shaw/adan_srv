/*
 * data_comm_list.h
 *
 *  Created on: Nov 17, 2016
 *      Author: adan
 */

#define uinfo_clean              12345
#define uinfo_add 		 54321
#define uinfo_select_by_uid      98765
#define uinfo_change             56789

#define upicture_clean           741258
#define upicture_add             852147
#define upicture_select_by_uid   963258
#define upicture_change          852369

//sql 语句buf max
#define sql_str_max 260


//open data 操作结果解析return
#define remyself_uinfo_clean              1
#define remyself_uinfo_add 		  2
#define remyself_uinfo_select_by_uid      3
#define remyself_uinfo_change             4

#define remyself_upicture_clean           5
#define remyself_upicture_add             6
#define remyself_upicture_select_by_uid   7
#define remyself_upicture_change          8

#define remyself_open_error -10000
#define remyself_query_error -10001
#define remyself_pickup_error -10002
