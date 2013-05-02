// IPIP.cpp : Defines the entry point for the console application.
//

#include <atlconv.h>
#include "atlbase.h"
#include "stdafx.h"

#include <AFX.H>
#include <WINDOWS.H>
#include <Winsock2.h>
#include <Winnetwk.h>
#pragma comment(lib,"Mpr.lib")
#pragma comment(lib,"Ws2_32.lib")


int main()
{
	struct   hostent   *host;   
	struct   in_addr   *ptr;   
	
	DWORD   dwScope   =   RESOURCE_CONTEXT;   
	NETRESOURCE   *NetResource   =   NULL;   
	HANDLE   hEnum;   
	WNetOpenEnum(   dwScope,   NULL,   NULL,   NULL,   &hEnum   );   
    
	WSADATA   wsaData;   
	WSAStartup(MAKEWORD(1,1),&wsaData);   
    
	if(hEnum)
	{   
		DWORD   Count  = 0xFFFFFFFF;   
		DWORD   BufferSize = 10240;   
		LPVOID   Buffer  =  new char[10240];   
		WNetEnumResource(hEnum, &Count,Buffer, &BufferSize);   
		NetResource  = (NETRESOURCE*)Buffer;   
		
		char szHostName[200];  
		
		for( unsigned int i =  0;i < BufferSize/sizeof(NETRESOURCE); i++, NetResource++)   
		{   
			if (NetResource->dwUsage  == RESOURCEUSAGE_CONTAINER && NetResource->dwType == RESOURCETYPE_ANY ) 
			{   
				if   (NetResource->lpRemoteName)   
				{   
					CString   strFullName   =   NetResource->lpRemoteName;   
					if   (   0 == strFullName.Left(2).Compare(_T("\\\\"))   )
						strFullName  =  strFullName.Right(strFullName.GetLength()-2);   
					
					
					gethostname(szHostName,strlen(szHostName));
				/*	USES_CONVERSION;*/
					char *pchar = strFullName.GetBuffer(30);
					//char *pchar;
					host   =   gethostbyname(pchar);   
					
					if(host   ==   NULL)   continue;     
					ptr   =   (struct in_addr *)   host->h_addr_list[0];  
					
					int   a   =   ptr->S_un.S_un_b.s_b1;     //   211   
					int   b   =   ptr->S_un.S_un_b.s_b2;     //   40   
					int   c   =   ptr->S_un.S_un_b.s_b3;     //   35   
					int   d   =   ptr->S_un.S_un_b.s_b4;     //   76   
					CString   strTemp;   
					strTemp.Format(_T("%d.%d.%d.%d"),a,b,c,d); 
					printf(strTemp.GetBuffer(strTemp.GetLength()));
					printf("\r\n");
					strTemp.ReleaseBuffer();
					strFullName.ReleaseBuffer();
				}
			}
		}
		delete   Buffer;   
		WNetCloseEnum(   hEnum   );   
	}    
	WSACleanup();     
	return 0;
}