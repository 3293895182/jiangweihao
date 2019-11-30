/*************************************************************************
#	 FileName	: server.c
#	 Author		: fengjunhui 
#	 Email		: 18883765905@163.com 
#	 Created	: 2018年12月29日 星期六 13时44分59秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include<sqlite3.h>


#include "common.h"

void admin_menu(int sockfd,MSG *msg);

void user_menu(int sockfd,MSG *msg);

/**************************************
 *函数名：do_query
 *参   数：消息结构体
 *功   能：查询
 ****************************************/
void do_admin_query(int sockfd,MSG *msg)   //管理员查询
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	int n;
	char name[DATALEN];
	msg->msgtype  = ADMIN_QUERY;
		
	while(1)
	{ 		
		printf ("------------------------------------------------\n");
		printf ("----1.按人名查找  2.查找所有   3.退出-----------\n");
		printf ("------------------------------------------------\n");
		
		printf ("请输入数字:");
		scanf ("%d",&n);
		switch (n)
		{
			case 1:
				strcpy(msg->recvmsg,"1");
				printf ("请输入要查找的姓名：");
				scanf ("%s",msg->info.name);
				send(sockfd, msg, sizeof(MSG), 0);
				recv(sockfd, msg, sizeof(MSG), 0);
				printf ("工号 用户类型 姓名 密码 年龄 电话 地址 职位 入职年月 等级 工资\n");				
				printf ("%s\n",msg->recvmsg);
				break;
			case 2:
				strcpy(msg->recvmsg,"2");
				send(sockfd, msg, sizeof(MSG), 0);
				printf ("工号 用户类型 姓名 密码 年龄 电话 地址 职位 入职年月 等级 工资\n");	
				while (1)
				{
					recv(sockfd, msg, sizeof(MSG), 0);
					if (strcmp(msg->recvmsg,"ALL")==0)
					{
						break;
					}
					printf ("%s\n",msg->recvmsg);
				}
				break;
			case 3:
				admin_menu(sockfd,msg);
				break;
			default :
				printf ("输入错误！");				
		}	
		
	}	
			
	
}


/**************************************
 *函数名：admin_modification
 *参   数：消息结构体
 *功   能：管理员修改
 ****************************************/
void do_admin_modification(int sockfd,MSG *msg)//管理员修改
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	
	msg->msgtype = ADMIN_MODIFY;
	
	printf ("请输入员工的工号：");
		scanf("%d",&msg->info.no);	
	while (1)
	{
		int n=0;
		printf(" 0.退出 1.用户类型 2.姓名 3密码 4.年龄 5.电话 6.地址 7.职位 8.入职年月 9.等级 10.工资\n");
		
		printf ("请输入要修改的项：");
		scanf ("%d",&n);
	
		
		send(sockfd,msg,sizeof(MSG),0);
		
		recv(sockfd,msg,sizeof(MSG),0);
		printf ("%s\n",msg->recvmsg);
		if (strcmp(msg->recvmsg,"error")==0)
		{
			printf ("请重新输入工号");
			continue;
		}
		
		
		
				
		switch (n)
		{
			
				case 1:
					msg->flags=1;
					while (1)
					{
						printf ("请输入要修改的用户类型：");
						scanf("%d",&msg->info.usertype);
						if (msg->info.usertype>1)
						{
							printf ("类型错误,请重新输入：");
							continue ;
						}
						else
						{
							break;
						}
					}					
					send(sockfd,msg,sizeof(MSG),0);					
					break;
					
				case 2:
					msg->flags=2;
					printf ("请输入要修改的姓名：");
					scanf("%s",msg->info.name);
					send(sockfd,msg,sizeof(MSG),0);
					break;
				case 3:
					msg->flags=3;
					printf ("请输入要修改的密码：");
					scanf("%s",msg->info.passwd);
					send(sockfd,msg,sizeof(MSG),0);
					break;
				case 4:
					msg->flags=4;
					printf ("请输入要修改的年龄：");
					scanf("%d",&msg->info.age);
					send(sockfd,msg,sizeof(MSG),0);
					break;
				case 5:
					msg->flags=5;
					printf ("请输入要修改的电话：");
					scanf("%s",msg->info.phone);
					send(sockfd,msg,sizeof(MSG),0);
					break;
				case 6:
					msg->flags=6;
					printf ("请输入要修改的地址：");
					scanf("%s",msg->info.addr);
					send(sockfd,msg,sizeof(MSG),0);
					break;
				case 7:
					msg->flags=7;
					printf ("请输入要修改的职位：");
					scanf("%s",msg->info.work);
					send(sockfd,msg,sizeof(MSG),0);
					break;
				case 8:
					msg->flags=8;
					printf ("请输入要修改的入职年月：");
					scanf("%s",msg->info.date);
					send(sockfd,msg,sizeof(MSG),0);
					break;
				case 9:
					msg->flags=9;
					printf ("请输入要修改的等级：");
					scanf("%d",&msg->info.level);
					send(sockfd,msg,sizeof(MSG),0);
					break;
				case 10:
					msg->flags=10;
					printf ("请输入要修改的工资：");
					scanf("%lf",&msg->info.salary);
					send(sockfd,msg,sizeof(MSG),0);
					break;	
				case 0:
					msg->flags=0;
					return ;
				default :
					printf ("输入错误！");
					break;
		}
		
					
	}
		
	admin_menu(sockfd,msg);
}


/**************************************
 *函数名：admin_adduser
 *参   数：消息结构体
 *功   能：管理员创建用户
 ****************************************/
void do_admin_adduser(int sockfd,MSG *msg)//管理员添加用户
{		
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	
	msg->msgtype  = ADMIN_ADDUSER;
	
	printf ("请输入要添加的工号：");
	scanf("%d",&msg->info.no);
	
	printf ("用户类型  ");
	scanf("%d",&msg->info.usertype);
	printf ("姓名  ");
	scanf("%s",msg->info.name);
	printf ("密码  ");
	scanf("%s",msg->info.passwd);
	printf ("年龄  ");
	scanf("%d",&msg->info.age);
	printf ("电话  ");
	scanf("%s",msg->info.phone);
	printf ("地址  ");
	scanf("%s",msg->info.addr);
	printf ("职位  ");
	scanf("%s",msg->info.work);
	printf ("入职年月  ");
	scanf("%s",msg->info.date);
	printf ("等级  ");
	scanf("%d",&msg->info.level);
	printf ("工资  ");
	scanf("%lf",&msg->info.salary);
	send(sockfd, msg, sizeof(MSG), 0);
	recv(sockfd,msg,sizeof(MSG),0);
}			



/**************************************
 *函数名：admin_deluser
 *参   数：消息结构体
 *功   能：管理员删除用户
 ****************************************/
void do_admin_deluser(int sockfd,MSG *msg)//管理员删除用户
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	msg->msgtype  = ADMIN_DELUSER;
	
	printf ("请输入要删除的工号：");
	scanf("%d",&msg->info.no);
	send(sockfd, msg, sizeof(MSG), 0);
	
	
}



/**************************************
 *函数名：do_history
 *参   数：消息结构体
 *功   能：查看历史记录
 ****************************************/
void do_admin_history (int sockfd,MSG *msg)   //历史记录
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	msg->msgtype  = ADMIN_HISTORY;
	send(sockfd, msg, sizeof(MSG), 0);
	while (1)
	{
		recv(sockfd, msg, sizeof(MSG), 0);
		if (strcmp(msg->recvmsg,"ALL")==0)
		{
			break;
		}
		printf ("%s\n",msg->recvmsg);
	}
}


/**************************************
 *函数名：admin_menu
 *参   数：套接字、消息结构体
 *功   能：管理员菜单
 ****************************************/
void admin_menu(int sockfd,MSG *msg)    //管理员菜单
{
	int n;

	while(1)
	{
		printf("*************************************************************\n");
		printf("* 1：查询  2：修改 3：添加用户  4：删除用户  5：查询历史记录*\n");
		printf("* 6：退出													*\n");
		printf("*************************************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&n);
		getchar();

		switch(n)
		{
			case 1:
				do_admin_query(sockfd,msg);
				break;
			case 2:
				do_admin_modification(sockfd,msg);
				break;
			case 3:
				do_admin_adduser(sockfd,msg);
				break;
			case 4:
				do_admin_deluser(sockfd,msg);
				break;
			case 5:
				do_admin_history(sockfd,msg);
				break;
			case 6:
				msg->msgtype = QUIT;
				strcpy(msg->recvmsg,msg->username);
				send(sockfd, msg, sizeof(MSG), 0);
				close(sockfd);
				exit(0);
			default:
				printf("您输入有误，请重新输入！\n");
		}
	}
}



/**************************************
 *函数名：do_query
 *参   数：消息结构体
 *功   能：查找
 ****************************************/
void do_user_query(int sockfd,MSG *msg)   //用户查找
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	msg->msgtype  = USER_QUERY;
	
	strcpy(msg->info.name,msg->username);
	send(sockfd, msg, sizeof(MSG), 0);
	recv(sockfd, msg, sizeof(MSG), 0);
	printf ("工号 用户类型 姓名 密码 年龄 电话 地址 职位 入职年月 等级 工资\n");				
	printf ("%s\n",msg->recvmsg);
	
	
}



/**************************************
 *函数名：do_modification
 *参   数：消息结构体
 *功   能：修改
 ****************************************/
void do_user_modification(int sockfd,MSG *msg)   //用户修改
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	
	int n=0;
	msg->msgtype  = USER_MODIFY;
	printf(" 0.退出 1.密码 2.电话 3.地址\n");
	printf ("请输入要修改的项：");
	scanf ("%d",&n);
	switch(n)
	{
		case 0:
			return ;
		case 1:
			strcpy(msg->recvmsg,"1");
			printf ("请输入要修改的密码：");
			scanf("%s",msg->info.passwd);
			send(sockfd,msg,sizeof(MSG),0);
			break;	
		case 2:
			strcpy(msg->recvmsg,"2");
			printf ("请输入要修改的电话：");
			scanf("%s",msg->info.phone);
			send(sockfd,msg,sizeof(MSG),0);
			break;
		case 3:
			strcpy(msg->recvmsg,"3");
			printf ("请输入要修改的地址：");
			scanf("%s",msg->info.addr);
			send(sockfd,msg,sizeof(MSG),0);
			break;
		default	:
			printf ("输入错误！");
			break;			
	}	
	
	
	
}


/**************************************
 *函数名：user_menu
 *参   数：消息结构体
 *功   能：用户菜单
 ****************************************/
void user_menu(int sockfd,MSG *msg)   //用户菜单
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	int n;
	while(1)
	{
		printf("*************************************************************\n");
		printf("*************  1：查询  	2：修改		3：退出	 *************\n");
		printf("*************************************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&n);
		getchar();

		switch(n)
		{
			case 1:
				do_user_query(sockfd,msg);
				break;
			case 2:
				do_user_modification(sockfd,msg);
				break;
			case 3:
				msg->msgtype = QUIT;
				send(sockfd, msg, sizeof(MSG), 0);
				close(sockfd);
				exit(0);
			default:
				printf("您输入有误，请输入数字\n");
				break;
		}
	}
}



int admin_or_user_login(int sockfd,MSG *msg)  //管理员或用户是否登陆成功
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	//输入用户名和密码
	memset(msg->username, 0, NAMELEN);
	printf("请输入用户名：");
	scanf("%s",msg->username);
	getchar();

	memset(msg->passwd, 0, DATALEN);
	printf("请输入密码（6位）");
	scanf("%s",msg->passwd);
	getchar();

	//发送登陆请求
	send(sockfd, msg, sizeof(MSG), 0);
	//接受服务器响应
	recv(sockfd, msg, sizeof(MSG), 0);
	printf("msg->recvmsg :%s\n",msg->recvmsg);


	//判断是否登陆成功
	if(strncmp(msg->recvmsg, "OK", 2) == 0)
	{
		if(msg->usertype == ADMIN)
		{
			printf("亲爱的管理员，欢迎您登陆员工管理系统！\n");
			admin_menu(sockfd,msg);
			strcpy(msg->recvmsg,msg->username);
			send(sockfd, msg, sizeof(MSG), 0);
		}
		else if(msg->usertype == USER)
		{
			printf("亲爱的用户，欢迎您登陆员工管理系统！\n");
			user_menu(sockfd,msg);
			strcpy(msg->recvmsg,msg->username);
			send(sockfd, msg, sizeof(MSG), 0);
		}
	}
	else
	{
		printf("登陆失败！%s\n", msg->recvmsg);
		return -1;
	}

	return 0;
}


/************************************************
 *函数名：do_login
 *参   数：套接字、消息结构体
 *返回值：是否登陆成功
 *功   能：登陆
 *************************************************/
int do_login(int sockfd)   //谁登录
{	
	int n;
	MSG msg;

	while(1){
		printf("*************************************************************\n");
		printf("********  1：管理员模式    2：普通用户模式    3：退出********\n");
		printf("*************************************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&n);
		getchar();
		
		switch(n)
		{
			case 1:
				msg.msgtype  = ADMIN_LOGIN;
				msg.usertype = ADMIN;
				break;
			case 2:
				msg.msgtype =  USER_LOGIN;
				msg.usertype = USER;
				break;
			case 3:
				msg.msgtype = QUIT;
				if(send(sockfd, &msg, sizeof(MSG), 0)<0)
				{
					perror("do_login send");
					return -1;
				}
				close(sockfd);
				exit(0);
			default:
				printf("您的输入有误，请重新输入\n"); 
				break;
		}

		admin_or_user_login(sockfd,&msg);
	}

}


int main(int argc, const char *argv[])
{
	//socket->填充->绑定->监听->等待连接->数据交互->关闭 
	int sockfd;
	int acceptfd;
	ssize_t recvbytes,sendbytes;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	socklen_t cli_len = sizeof(clientaddr);

	//创建网络通信的套接字
	sockfd = socket(AF_INET,SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket failed.\n");
		exit(-1);
	}
	printf("sockfd :%d.\n",sockfd); 

	//填充网络结构体
	memset(&serveraddr,0,sizeof(serveraddr));
	memset(&clientaddr,0,sizeof(clientaddr));
	serveraddr.sin_family = AF_INET;
//	serveraddr.sin_port   = htons(atoi(argv[2]));
//	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port   = htons(5000);
	serveraddr.sin_addr.s_addr = inet_addr("192.168.1.188");

	if(connect(sockfd,(const struct sockaddr *)&serveraddr,addrlen) == -1){
		perror("connect failed.\n");
		exit(-1);
	}

	do_login(sockfd);

	close(sockfd);

	return 0;
}



