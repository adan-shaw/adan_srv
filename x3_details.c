/*
 * io_details.c
 *
 *  Created on: Jan 2, 2017
 *      Author: adan
 */

/*
1.关于协议块控制,或许这都是操作流程细节,因为书本都是讲述TCP/IP 的详细过程的,所以并不会遗漏任何细节:
从 网络基础知识->封包流程->插口api->选路->协议块控制 的整个流程都详细讲述,这不是一本讲述网络编程的书.
协议块控制还包括：internet pcb->tcp 专用pcb: 区分UDP 和TCP 的真正区别和共同点internet pcb
tcp 专用pcb还包括：7个定时器,9个控制函数（内核API）等等

那么到底怎么保证网络IO 稳定和异常catch ??
*1.利用好定时器,在规定时间内操作IO 失败,那就就应该异常返回--这里可以修改sockopt 来实现

*2.利用好send recv 的返回值:
--1. >0 则对比返回 >0 的值是否跟源数据的长度一致
--2. =0 则表示
--3. <0 则判断是否是SOCKET_ERROR,如果是, 什么原因？？如果不是,又是什么原因


2.只能发送方检查数据大小吗？收发双方都应该检查数据大小才对,根据mbuf 的原理,recv 方从首个mbuf 中获取
这次数据发送的整个包大小,不是没有可能的,那么到底要怎么做？？

虽然我们还是立足于应用层,但是这些细节到底要怎么搞？？这是个问题～

不可能收到之后再发送一次确认包,这种怂逼做法是绝对不可能的

***************************************************************************

 linux send与recv函数详解

1 #include <sys/socket.h>
2 ssize_t recv(int sockfd, void *buff, size_t nbytes, int flags);
3 ssize_t send(int sockfd, const void *buff, size_t nbytes, int flags);

recv 和send的前3个参数等同于read和write,等于0 都是信号被终端,接收/发送时,copy 数据到socket 缓冲区失败,
会返回SOCKET_ERROR,收到数据后对比大小,可以得到收发数据的正确性...

flags参数值为0或：
  flags 	说明 	         recv 	send
 MSG_DONTROUTE 	绕过路由表查找  	  	  •
 MSG_DONTWAIT 	仅本操作非阻塞  	  •       •
 MSG_OOB　　　　 	发送或接收带外数据 	  • 	  •
 MSG_PEEK　　 	窥看外来消息 	  •
 MSG_WAITALL　　 	等待所有数据  	  •

 1. send解析
 sockfd： 指定发送端套接字描述符。
   buff： 存放要发送数据的缓冲区
 nbytes:  实际要改善的数据的字节数
  flags： 一般设置为0

 1) send先比较发送数据的长度nbytes和套接字sockfd的发送缓冲区的长度，如果nbytes > 套接字sockfd的发送缓冲区的长度, 该函数返回SOCKET_ERROR;

 2) 如果nbtyes <= 套接字sockfd的发送缓冲区的长度，那么send先检查协议是否正在发送sockfd的发送缓冲区中的数据，如果是就等待协议把数据发送完，如果协议还没有开始发送sockfd的发送缓冲区中的数据或者sockfd的发送缓冲区中没有数据，那么send就比较sockfd的发送缓冲区的剩余空间和nbytes

 3) 如果 nbytes > 套接字sockfd的发送缓冲区剩余空间的长度，send就一起等待协议把套接字sockfd的发送缓冲区中的数据发送完

 4) 如果 nbytes < 套接字sockfd的发送缓冲区剩余空间大小，send就仅仅把buf中的数据copy到剩余空间里(注意并不是send把套接字sockfd的发送缓冲区中的数据传到连接的另一端的，而是协议传送的，send仅仅是把buf中的数据copy到套接字sockfd的发送缓冲区的剩余空间里)。

 5) 如果send函数copy成功，就返回实际copy的字节数，如果send在copy数据时出现错误，那么send就返回SOCKET_ERROR; 如果在等待协议传送数据时网络断开，send函数也返回SOCKET_ERROR。

 6) send函数把buff中的数据成功copy到sockfd的改善缓冲区的剩余空间后它就返回了，但是此时这些数据并不一定马上被传到连接的另一端。如果协议在后续的传送过程中出现网络错误的话，那么下一个socket函数就会返回SOCKET_ERROR。（每一个除send的socket函数在执行的最开始总要先等待套接字的发送缓冲区中的数据被协议传递完毕才能继续，如果在等待时出现网络错误那么该socket函数就返回SOCKET_ERROR）

 7) 在unix系统下，如果send在等待协议传送数据时网络断开，调用send的进程会接收到一个SIGPIPE信号，进程对该信号的处理是进程终止。

2.recv函数
sockfd: 接收端套接字描述符
  buff：用来存放recv函数接收到的数据的缓冲区
nbytes: 指明buff的长度
 flags: 一般置为0

 1) recv先等待s的发送缓冲区的数据被协议传送完毕，如果协议在传送sock的发送缓冲区中的数据时出现网络错误，那么recv函数返回SOCKET_ERROR

 2) 如果套接字sockfd的发送缓冲区中没有数据或者数据被协议成功发送完毕后，recv先检查套接字sockfd的接收缓冲区，如果sockfd的接收缓冲区中没有数据或者协议正在接收数据，那么recv就一起等待，直到把数据接收完毕。当协议把数据接收完毕，recv函数就把s的接收缓冲区中的数据copy到buff中（注意协议接收到的数据可能大于buff的长度，所以在这种情况下要调用几次recv函数才能把sockfd的接收缓冲区中的数据copy完。recv函数仅仅是copy数据，真正的接收数据是协议来完成的）

 3) recv函数返回其实际copy的字节数，如果recv在copy时出错，那么它返回SOCKET_ERROR。如果recv函数在等待协议接收数据时网络中断了，那么它返回0。

 4) 在unix系统下，如果recv函数在等待协议接收数据时网络断开了，那么调用 recv的进程会接收到一个SIGPIPE信号，进程对该信号的默认处理是进程终止。





















 */
