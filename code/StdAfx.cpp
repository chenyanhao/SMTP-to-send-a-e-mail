/////////////////////////////////////////////////////////////////////
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// �����ʼ��Ĳ���.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������
void InitializeWSA()
{
	WSAData wsadata;
	WSAStartup(MAKEWORD(2,2),&wsadata); 
}

char* WebsiteToIPAddress(char* pWebsite)
{
	hostent* ht = gethostbyname( pWebsite);
	char* IpAddress = (char*)HeapAlloc(GetProcessHeap(),0,32);
	memset(IpAddress,0,32);
	sprintf(IpAddress,"%d.%d.%d.%d",*(byte*)(ht->h_addr_list[0]),*(byte*)(ht->h_addr_list[0]+1),*(byte*)(ht->h_addr_list[0]+2),*(byte*)(ht->h_addr_list[0]+3));
	return IpAddress;
}


SOCKET CreateConnection(char* pWebsite,int iPort)
{
    //Ϊ����socket������׼������ʼ������
 	SOCKET sockClient = socket(AF_INET,SOCK_STREAM,0);//����socket����
 	SOCKADDR_IN addrSrv;   
	char* IpAddress = WebsiteToIPAddress(pWebsite);
	//printf("%s\n",IpAddress);
	addrSrv.sin_addr.S_un.S_addr = inet_addr(IpAddress);//��U_LONG�������ֽ�˳��ת��ΪTCP/IP�����ֽ�˳��   
	addrSrv.sin_family = AF_INET;   
	addrSrv.sin_port = htons(iPort);   
	int tf = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));   //���������������  
	if(tf != 0)
	{
		return 0;
		//printf("����ʧ��\n");
	}
	return sockClient;
}


bool SendAndRecvMsg(SOCKET sockClient,
					char* pMessage,
					int Messagelen,
					int DoWhat,//�Ƿ��������
					char* recvBuf,
					int recvBufLen)
{
	char lpMessage[256] = {0};
	memcpy(lpMessage, pMessage, Messagelen);
	printf("\n\n%s \n", lpMessage);
     	if (DoWhat==0)
	 {
		send(sockClient, lpMessage, Messagelen, 0);
		memset(recvBuf, 0, recvBufLen);
		DWORD num=recv(sockClient, recvBuf, recvBufLen, 0);     //��������
		printf("%s \n", recvBuf); 
		int i=0;
		while(i != num)
		{
			  printf("%02X ", recvBuf[i++]); 
			  if((i)%16 == 0)
			  {
				  printf("\n"); 
			  }
		}
		printf("\n");

	 }else if (DoWhat==1)
	 {
		send(sockClient,lpMessage,Messagelen,0);

	 }else if (DoWhat==2)
	 {
		memset(recvBuf, 0, recvBufLen);
		DWORD num=recv(sockClient, recvBuf, recvBufLen,0);     //��������
		printf("%s \n", recvBuf); 
		int i=0;
		while(i<num)
		{
			  printf("%02X ", (byte)recvBuf[i++]); 
			  if((i)%16==0)
			  {
				  printf("\n"); 
			  }
		}
		printf("\n");
	 }
     
     return recvBuf;
}

void CloseSock(SOCKET sockClient)
{
	 //�ر��׽���   
     closesocket(sockClient);   
     WSACleanup();  
}

char* StringToBase64(char* bString, int len)//len��bString�ĳ��ȣ����������\0
{
	int Index=0;
	byte data[3]={0};
	char* Base64=(char*)HeapAlloc(GetProcessHeap(),0,(len*4)/3+12);
	memset(Base64,0,(len*4)/3+12);
	char* tmp=(char*)HeapAlloc(GetProcessHeap(),0,5);

	while((Index+3)<=len)
	{
		memset(data,0,3);
		memcpy(data,bString+Index,3);
		memset(tmp,0,5);
		tmp[0]=(data[0]>>2)&0x3F;
		tmp[1]=((data[0]<<4)+(data[1]>>4))&0x3F;
		tmp[2]=((data[1]<<2)+(data[2]>>6))&0x3F;
		tmp[3]=data[2]&0x3F;
		lstrcat(Base64,tmp);
		Index+=3;
	}
	memset(tmp,0,5);
	Index=len%3;
	if(Index!=0)
	{
		memset(data,0,3);
		memcpy(data,bString+len-Index,Index);
		memset(tmp,0,5);
		tmp[0]=(data[0]>>2)&0x3F;
		tmp[1]=((data[0]<<4)+(data[1]>>4))&0x3F;
		tmp[2]=((data[1]<<2)+(data[2]>>6))&0x3F;
		tmp[3]=data[2]&0x3F;
		lstrcat(Base64,tmp);

		memset(tmp,0,5);

		Index=3-Index;
		while(Index--)
		{
			lstrcat(tmp,"=");
		}
	}

	Index=0;
	while(Base64[Index]!=0)
	{
		//printf("%02d=",Base64[Index]);
		if(Base64[Index]<=25 && Base64[Index]>=0)
		{
			Base64[Index]+='A';

		}else if(Base64[Index]<=51 && Base64[Index]>=26)
		{
			Base64[Index]-=26;
			Base64[Index]+='a';

		}else if(Base64[Index]<=61 && Base64[Index]>=52)
		{
			Base64[Index]-=4;

		}else if(Base64[Index]==62)
		{
			Base64[Index]='+';

		}else if(Base64[Index]==63)
		{
			Base64[Index]='/';

		}else
		{
//			MessageBox(0,"ת����Base64����",&Base64[Index],0);
			//printf("%02d\n",Base64[Index]);
		}
//		//printf("%c\n",Base64[Index]);

		Index++;
	}
	lstrcat(Base64,tmp);

	HeapFree(GetProcessHeap(), 0, tmp);
	return Base64;
}

bool FormatEmail(char* pFrom,char* pTo,char* pSubject,char* pMessage,char* Email)
{
	lstrcat(Email,"From: ");
	lstrcat(Email,pFrom);
	lstrcat(Email,"\r\n");

	lstrcat(Email,"To: ");
	lstrcat(Email,pTo);
	lstrcat(Email,"\r\n");

	lstrcat(Email,"Subject: ");
	lstrcat(Email,pSubject);
	lstrcat(Email,"\r\n");

	lstrcat(Email,"MIME-Version: 1.0");
	lstrcat(Email,"\r\n");
	lstrcat(Email,"\r\n");

	lstrcat(Email,pMessage);

	lstrcat(Email,"\r\n.\r\n");
	return 1;
}

void WINAPI SendEmail(
	char* pUserNameToSendEmail,			//������
	char* pPassWordToSendEmail,			//�����������
	char* SMTPService,				//�������SMTP������
	char* pTargetEmail,				//Ŀ������
	char* pEmailTitle,				//�ʼ��ı���
	char* pContent					//�ʼ�������
	)
{
	char* base;
	char str[1024];
	
	InitializeWSA();
	;
	SOCKET sockClient = CreateConnection(SMTPService, 25);
	char recvBuf[1024];
	memset(recvBuf, 0, 1024);

	bool tf = SendAndRecvMsg(sockClient, 0, 0, 2, recvBuf, 1024);
	char UserNameToSendEmail[256] = {0};
	sprintf(UserNameToSendEmail, "EHLO %s", pUserNameToSendEmail);
	lstrcat(UserNameToSendEmail, "\r\n\0");
	tf = SendAndRecvMsg(sockClient, UserNameToSendEmail, GetAsc2Len(UserNameToSendEmail), 0, recvBuf, 1024);
	
	tf = SendAndRecvMsg(sockClient, "AUTH LOGIN\r\n", strlen("AUTH LOGIN\r\n"), 0, recvBuf, 1024);

	char pUerName[256] = {0};
	DWORD p=strstr(pUserNameToSendEmail, "@") - pUserNameToSendEmail;
	memcpy(pUerName, pUserNameToSendEmail, p);
	
	base=StringToBase64(pUerName, lstrlen(pUerName));
	memset(str, 0, 1024);
	sprintf(str, "%s\r\n", base);
	tf = SendAndRecvMsg(sockClient, str, lstrlen(str), 0, recvBuf, 1024);

	base=StringToBase64(pPassWordToSendEmail, lstrlen(pPassWordToSendEmail));
	memset(str,0,1024);
	sprintf(str, "%s\r\n", base);
	tf = SendAndRecvMsg(sockClient, str, lstrlen(str), 0, recvBuf, 1024);

	char MailFrom[256] = {0};
	sprintf(MailFrom, "MAIL FROM: <%s>\r\n", pUserNameToSendEmail);

	tf = SendAndRecvMsg(sockClient, MailFrom, lstrlen(MailFrom), 0, recvBuf, 1024);
	
	char RcptTo[256]={0};
	sprintf(RcptTo, "RCPT TO: <%s>\r\n", pTargetEmail);
	tf = SendAndRecvMsg(sockClient, RcptTo, lstrlen(RcptTo), 0, recvBuf, 1024);
	
	tf = SendAndRecvMsg(sockClient, "DATA\r\n", lstrlen("DATA\r\n"), 0, recvBuf, 1024);
	

	char Email[1024]={0};
	FormatEmail(pUserNameToSendEmail, pTargetEmail, pEmailTitle, pContent, Email);

	tf = SendAndRecvMsg(sockClient, Email, lstrlen(Email), 0, recvBuf, 1024);
	
	tf = SendAndRecvMsg(sockClient, "QUIT\r\n", lstrlen("QUIT\r\n"), 0, recvBuf, 1024);
	
	CloseSock(sockClient);

	return ;
}

int GetAsc2Len(char* pString)
{
	int i=0;
	while(pString[i++]!=0);
	return i-1;
}
////////////////////////////////////////////////////////////////////

