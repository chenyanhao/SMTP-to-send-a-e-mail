#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
   SendEmail(
	"haha@126.com",				//发件箱
	"*******",				//发件箱的密码
	"SMTP.126.com",				//发件箱的SMTP服务器
	"hehe@163.com",				//目标邮箱
	"我是标题.",				//邮件的标题
	"Have A Test!!!"			//邮件的内容
	);

   system("pause");
	return 0;
}

