#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
   SendEmail(
	"haha@126.com",				//������
	"*******",				//�����������
	"SMTP.126.com",				//�������SMTP������
	"hehe@163.com",				//Ŀ������
	"���Ǳ���.",				//�ʼ��ı���
	"Have A Test!!!"			//�ʼ�������
	);

   system("pause");
	return 0;
}

