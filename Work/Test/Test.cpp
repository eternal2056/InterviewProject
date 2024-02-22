#include <windows.h>
#include <stdio.h>
#include <windef.h>

/*
FARPROC 是一个4字节指针，指向一个函数的内存地址，例如
GetProcAddress 的返回类型就是一个FARPROC。
如果你要存放这个地址，可以声明以一个 FARPROC变量来存放。
*/

typedef struct _THREAD_PARAM
{
	FARPROC pFunc[3];               // LoadLibraryA(), GetProcAddress()，存放两个API函数的地址
	char    szBuf[4][128];          // "user32.dll", "MessageBoxA", "www.reversecore.com", "ReverseCore" 
} THREAD_PARAM, * PTHREAD_PARAM;

typedef HMODULE(WINAPI* PFLOADLIBRARYA)
(
	LPCSTR lpLibFileName//LoadLibraryA的函数指针,HMODULE的返回值，调用约定为WINAPI，指针名叫PFLOADLIBRARYA，参数为字符串指针lpLibFileName
	);

typedef FARPROC(WINAPI* PFGETPROCADDRESS)//同理GetProcAddress()的函数指针
(
	HMODULE hModule,//模块句柄
	LPCSTR lpProcName//查找的函数名字符串指针
	);
typedef FARPROC(WINAPI* MYPRINTF)//同理GetProcAddress()的函数指针
();

typedef int (WINAPI* PFMESSAGEBOXA)//MessageBoxA的函数指针
(
	HWND hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT uType
	);

DWORD WINAPI ThreadProc(LPVOID lParam)     //线程过程，用来存放我们的注入代码
{
	PTHREAD_PARAM   pParam = (PTHREAD_PARAM)lParam;   //将传入的指针强制类型转换为我们定义的线程参数结构体类型的指针，其实指针本质都是相同的，只是语言的语法要求
	HMODULE         hMod = NULL;                                              //定义一个hMod来存放模块句柄
	FARPROC         pFunc = NULL;                                                  //定义一个FARPROC类型的变量来存放GetProcAddress的返回值
	//MessageBoxA(NULL, "获取成功", "OK", MB_OK);

	hMod = ((PFLOADLIBRARYA)pParam->pFunc[0])(pParam->szBuf[0]);   // LoadLibrary("user32.dll")
	if (!hMod)
		return 1;

	//pFunc = (FARPROC)((MYPRINTF)pParam->pFunc[2])();  // GetProcAddress(hMod, "MessageBoxA");
	pFunc = (FARPROC)((PFGETPROCADDRESS)pParam->pFunc[1])(hMod, pParam->szBuf[1]);  // GetProcAddress(hMod, "MessageBoxA");
	if (!pFunc)
		return 1;

	((PFMESSAGEBOXA)pFunc)(NULL, pParam->szBuf[2], pParam->szBuf[3], MB_OK);    // MessageBoxA(NULL, "www.reversecore.com", "ReverseCore", MB_OK);

	return 0;
}
void WINAPI myPrintf();
void WINAPI myPrintf() {
	MessageBoxA(NULL, "获取成功", "OK", MB_OK);
}
void* GetProgmanProcess() {
	unsigned long procId_ul;
	GetWindowThreadProcessId(FindWindowA("Progman", NULL), &procId_ul);
	return OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, 0, procId_ul);
}
BOOL InjectCode()
{
	HMODULE         hMod = NULL;
	THREAD_PARAM    param = { 0, };
	HANDLE          hProcess = NULL;
	HANDLE          hThread = NULL;
	LPVOID          pRemoteBuf[2] = { 0, }; //void型的长指针，用来指向分配的内存
	DWORD           dwSize = 0;

	hMod = GetModuleHandleA("kernel32.dll");

	param.pFunc[0] = GetProcAddress(hMod, "LoadLibraryA");  //给线程参数结构体赋值
	param.pFunc[1] = GetProcAddress(hMod, "GetProcAddress");
	param.pFunc[2] = (FARPROC)myPrintf;
	strcpy_s(param.szBuf[0], "user32.dll");
	strcpy_s(param.szBuf[1], "MessageBoxA");
	strcpy_s(param.szBuf[2], "www.reversecore.com");
	strcpy_s(param.szBuf[3], "ReverseCore");

	if (!(hProcess = GetProgmanProcess()))//根据PID获取进程句柄
	{
		printf("OpenProcess() fail : err_code = %d\n", GetLastError());
		return FALSE;
	}
	dwSize = sizeof(THREAD_PARAM);
	if (!(pRemoteBuf[0] = VirtualAllocEx(hProcess,
		NULL,
		dwSize,
		MEM_COMMIT,
		PAGE_READWRITE))) //在对应的进程内部给线程参数，即注入代码要用到的数据分配空间
	{
		printf("VirtualAllocEx() fail : err_code = %d\n", GetLastError());
		return FALSE;
	}

	if (!WriteProcessMemory(hProcess,//将线程参数(注入代码要使用的数据)结构体写入分配的内存
		pRemoteBuf[0],
		(LPVOID)&param,
		dwSize,
		NULL))
	{
		printf("WriteProcessMemory() fail : err_code = %d\n", GetLastError());
		return FALSE;
	}

	//就是因为这个的原因，生成的时候必须用release
	dwSize = (DWORD)InjectCode - (DWORD)ThreadProc;//函数名就是函数的起始地址，在内存中紧跟着ThreadProc函数的就是InjectCode,所以首地址相减就能够得到ThreadProc代码占用的空间

	//dwSize += 100000; // 没事, 但是别这么写

	if (!(pRemoteBuf[1] = VirtualAllocEx(hProcess,
		NULL,
		dwSize,
		MEM_COMMIT,
		PAGE_EXECUTE_READWRITE)))//为注入代码在对应进程分配空间
	{
		printf("VirtualAllocEx() fail : err_code = %d\n", GetLastError());
		return FALSE;
	}

	if (!WriteProcessMemory(hProcess,
		pRemoteBuf[1],
		(LPVOID)ThreadProc,
		dwSize,
		NULL))//在对应空间中写入代码
	{
		printf("WriteProcessMemory() fail : err_code = %d\n", GetLastError());
		return FALSE;
	}

	if (!(hThread = CreateRemoteThread(hProcess, //创建远程线程执行注入代码
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)pRemoteBuf[1],
		pRemoteBuf[0],
		0,
		NULL)))
	{
		printf("CreateRemoteThread() fail : err_code = %d\n", GetLastError());
		return FALSE;
	}

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}

BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege) //设置权限
{
	TOKEN_PRIVILEGES tp;
	HANDLE hToken;
	LUID luid;

	if (!OpenProcessToken(GetCurrentProcess(),//打开一个与进程相关联的访问token
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken))
	{
		printf("OpenProcessToken error: %u\n", GetLastError());
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL,//查找系统对应的权限值并储存到luid中
		lpszPrivilege,
		&luid))
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;//将这些都存入到TOKEN_PRIVILEGES结构体中
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;//并设置权限属性
	else
		tp.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(hToken,//启用提权
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}

int main()
{
	if (!SetPrivilege(SE_DEBUG_NAME, TRUE))
		return 1;

	InjectCode();

	return 0;
}
