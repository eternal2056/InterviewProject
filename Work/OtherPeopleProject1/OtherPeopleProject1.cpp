// win10Pin2TB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <strsafe.h>
#include <Shlwapi.h>
#include <ShlDisp.h>
#include <comutil.h>
#include <iostream>
#include <fstream>
#include <shlobj.h>
#include <atlbase.h> // 包含 CComPtr 类的头文件


#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Shell32.lib")

BOOL WriteMsg2Console(LPCWSTR msg, ...)
{
	va_list va; // [rsp+258h] [rbp+10h]
	va_start(va, msg);
	WCHAR Str[MAX_PATH] = { 0 };
	StringCchVPrintf(Str, MAX_PATH, msg, va);
	size_t strLen = wcslen(Str);
	HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD NumberOfCharsWritten = 0;
	return WriteConsole(stdHandle, &Str, (DWORD)strLen, &NumberOfCharsWritten, 0i64);
}

#define BUFFER_SIZE  0x478
#define COMMAND_BUFFER_SIZE 0x64

static const void WINAPI replace_wchar(const wchar_t* srcStr, wchar_t replaceChar)
{
	if (srcStr == NULL)
		return;

	int strLen = 0;
	const wchar_t* sptr = srcStr;
	while (*sptr++) ++strLen;

	if (strLen >= 0)
	{
		int lastIndex = strLen;
		wchar_t* pSrcChar = (wchar_t*)&srcStr[lastIndex];
		do
		{
			if (*pSrcChar == replaceChar)
			{
				if (lastIndex == strLen)
				{
					*pSrcChar = 0;
				}
				else if (lastIndex < strLen)
				{
					int diff = strLen - lastIndex;
					wchar_t* pOldChar = pSrcChar + 1;
					wchar_t* pNewLocation = pSrcChar;
					while (diff)
					{
						*pNewLocation = *pOldChar;
						++pOldChar;
						++pNewLocation;
						--diff;
					}
				}
			}
			--lastIndex;
			--pSrcChar;
		} while (lastIndex >= 0);
	}
}
typedef __int64(*RealCTaskListWnd__Drop)(
	void* __this,
	struct IDataObject* a2,
	unsigned int a3,
	struct _POINTL a4,
	unsigned int* a5
	);
typedef DWORD(WINAPI* thread_callback)(void* pContextData);
static DWORD WINAPI thread_func(void* pContextData)
{
	uintptr_t absoluteAddress = NULL; // 获取模块基址
	uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)); // 获取模块基址
	uintptr_t relativeOffset = 0x2357E0; // 替换为函数在目标程序中的相对地址



	uintptr_t ulStartAddress = baseAddress;
	uintptr_t ulEndAddress = baseAddress + relativeOffset + 10000;
	uintptr_t* retFunAddr;
	CHAR szCodeFlag1[] = { 0x48,0x89 ,0x5C ,0x24 ,0x10 ,0x48, 0x89 ,0x6C ,0x24 ,0x18, 0x56, 0x57,
					   0x41, 0x54,
					   0x41, 0x56,
					   0x41, 0x57,
					   0x48, 0x83,0xEC,0x30,
					   0x4C, 0x8B,0xE2,
					   0x48, 0x8B,0xF1,
					   0x48, 0x83,0xC1, 0xE8,
					   0x33, 0xD2,
					   0x49, 0x8B, 0xD9,
					   0x45, 0x8B, 0xF8,
	};
	char buffer2[17]; // 16 个字符的缓冲区，用于存储地址的字符串表示
	sprintf_s(buffer2, "%p", (void*)*(int*)(ulStartAddress + 40)); // 将地址格式化成字符串
	//MessageBoxA(NULL, buffer2, "OK", MB_OK);
	for (size_t i = ulStartAddress; i < ulEndAddress; i++) {


		const unsigned char* byte_ptr1 = (const unsigned char*)i;
		const unsigned char* byte_ptr2 = (const unsigned char*)szCodeFlag1;
		bool isMatch = true;
		for (size_t i = 0; i < sizeof(szCodeFlag1); ++i)
		{
			if (byte_ptr1[i] < byte_ptr2[i])
				isMatch = false;
			else if (byte_ptr1[i] > byte_ptr2[i])
				isMatch = false;
		}

		if (isMatch) {
			//MessageBoxA(NULL, "获取成功", "OK", MB_OK);
			char buffer1[17]; // 16 个字符的缓冲区，用于存储地址的字符串表示
			sprintf_s(buffer1, "%p", (void*)(i)); // 将地址格式化成字符串
			//MessageBoxA(NULL, buffer1, "OK", MB_OK);
			absoluteAddress = i;
			//return 1;
			//if (memcmp(i + 0x23, szCodeFiag2, 7) == 0) {
			//	return 1;
			//}
		}
	}

	static HWND hTaskbarWnd;
	hTaskbarWnd = FindWindowW(L"Shell_TrayWnd", NULL);
	if (!hTaskbarWnd)
		MessageBoxA(NULL, "FindWindowW 失败", "OK", MB_OK);

	HWND hTaskSwWnd = (HWND)GetPropW(hTaskbarWnd, L"TaskbandHWND");
	if (!hTaskSwWnd)
		MessageBoxA(NULL, "GetPropW 失败", "OK", MB_OK);
	LONG_PTR lpTaskSwLongPtr = GetWindowLongPtr(hTaskSwWnd, 0);
	if (!lpTaskSwLongPtr)
		MessageBoxA(NULL, "GetWindowLongPtr 失败", "OK", MB_OK);

	HWND hTaskListWnd = FindWindowExW(hTaskSwWnd, NULL, L"MSTaskListWClass", NULL);
	if (!hTaskListWnd)
		MessageBoxA(NULL, "FindWindowExW 失败", "OK", MB_OK);

	HANDLE hProp = GetPropW(hTaskSwWnd, L"OleDropTargetInterface");
	if (!hProp)
		MessageBoxA(NULL, "GetPropW 失败", "OK", MB_OK);
	//CTaskListWnd x;
	//if (isFirst) {
	//	__this1 = __this;
	//	isFirst = false;
	//	return 0;
	//}


	const char* filePath = "C:\\Windows\\System32\\calc.exe";
	HWND hwnd = 0;
	CComPtr<IShellFolder> pDesktopFolder;
	HRESULT hr = SHGetDesktopFolder(&pDesktopFolder);

	// 计算转换后需要的缓冲区大小
	int nLength = MultiByteToWideChar(CP_UTF8, 0, filePath, -1, nullptr, 0);
	// 分配缓冲区
	WCHAR* wszTarget = new WCHAR[nLength];
	// 执行转换
	if (MultiByteToWideChar(CP_UTF8, 0, filePath, -1, wszTarget, nLength) == 0) {
		delete[] wszTarget; // 释放内存
	}

	WCHAR* lpWPath = wszTarget;
	LPITEMIDLIST pidl;
	hr = pDesktopFolder->ParseDisplayName(nullptr, nullptr, lpWPath, nullptr, &pidl, nullptr);
	if (FAILED(hr)) {
	}
	CComPtr<IShellFolder> pShellFolder;
	LPCITEMIDLIST pidlChild;
	hr = SHBindToParent(pidl, IID_IShellFolder, (void**)&pShellFolder, &pidlChild);
	CoTaskMemFree(pidl);
	if (FAILED(hr)) {
	}
	IDataObject* pDataObject = nullptr;
	hr = pShellFolder->GetUIObjectOf(hwnd, 1, &pidlChild, IID_IDataObject, nullptr, (void**)&pDataObject);
	if (FAILED(hr)) {
	}
	IDataObject* IDataObject1 = NULL;

	IDataObject1 = pDataObject;


	unsigned int x = 0;
	unsigned int* y = &x;
	_POINTL z;
	z.x = 0x12F;
	z.y = 0x38A;
	int nWinVersion = WIN_VERSION_10_19H1;
	//MessageBoxA(NULL, "EV_TASK_SW_TASK_GROUPS_HDPA 开始", "OK", MB_OK);
	LONG_PTR* plp = (LONG_PTR*)*(HDPA*)(lpTaskSwLongPtr +
		DO11_3264(0xA8, 0x120, 0xA4, 0x118, 0xA8, 0x120, 0xC0, 0x140, 0xC0, 0x148, 0xC4, 0x150, 0xC0, 0x158, 0xC8, 0x160, , , , , 0xC0, 0x158));;
	if (plp && (int)plp[0] > 0)
	{
		int task_groups_count = (int)plp[0];
		LONG_PTR** task_groups = (LONG_PTR**)plp[1];
		LONG_PTR* task_group = task_groups[0];
		//int x = (((*(int*)(baseAddress + 0x5D)) / 8) + 3);

		char* x = ((char*)(absoluteAddress)+0x5D + 0x3);
		//MessageBoxA(NULL, AddressToAnsiString((void*)x).data(), "OK", MB_OK);
		char buffer3[17]; // 16 个字符的缓冲区，用于存储地址的字符串表示
		sprintf_s(buffer3, "%p", (void*)*(short*)x); // 将地址格式化成字符串
		//MessageBoxA(NULL, buffer3, "OK", MB_OK);
		*((unsigned long long**)hProp + (int)((int)*(short*)x / 8)) = (unsigned long long*)task_group;
		((RealCTaskListWnd__Drop)absoluteAddress)(hProp, IDataObject1, 0, z, y);
	}
	return 1;
}

BOOL __fastcall InjectFun2Explorer(LPCVOID lpThreadArgs, HANDLE hProcess, thread_callback callBack)
{
	HMODULE hModule = GetModuleHandleW(NULL);
	printf("hModule: %p\n", hModule);
	printf("hModule: %p\n", InjectFun2Explorer);
	PIMAGE_NT_HEADERS pNTH = (PIMAGE_NT_HEADERS)((ULONG_PTR)hModule + ((IMAGE_DOS_HEADER*)hModule)->e_lfanew);

	BOOL ret = FALSE;
	if (pNTH->Signature == IMAGE_NT_SIGNATURE)
	{
		DWORD image_size = pNTH->OptionalHeader.SizeOfImage;
		ret = IsBadReadPtr(hModule, image_size);
		if (!ret)
		{
			LPVOID pProgman_mem = VirtualAllocEx(hProcess, NULL, image_size + BUFFER_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (pProgman_mem)
			{
				LPVOID pLocalMem = VirtualAlloc(NULL, image_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				if (pLocalMem)
				{
					memcpy(pLocalMem, (char*)hModule, image_size);
					PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)&pNTH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
					if (pReloc->SizeOfBlock)
					{
						if (pReloc->VirtualAddress)
						{
							PIMAGE_BASE_RELOCATION pRelocInLocalMem = (PIMAGE_BASE_RELOCATION) & ((char*)pLocalMem)[pReloc->VirtualAddress];
							LONG_PTR delta_progman = (ULONG_PTR)((ULONGLONG)pProgman_mem - pNTH->OptionalHeader.ImageBase);
							LONG_PTR delta_localMem = (ULONG_PTR)((ULONGLONG)hModule - pNTH->OptionalHeader.ImageBase);
							while (pRelocInLocalMem && pRelocInLocalMem->SizeOfBlock >= sizeof(IMAGE_BASE_RELOCATION))
							{
								DWORD countOfBlock = (pRelocInLocalMem->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) >> 1;
								if (countOfBlock)
								{
									WORD* typeoffset = (WORD*)(pRelocInLocalMem + 1);
									do
									{
										if (*typeoffset)
										{
											PULONG_PTR address_to_reloc = (PULONG_PTR) & ((char*)pLocalMem)[pRelocInLocalMem->VirtualAddress + (*typeoffset & 0xFFF)];
											*address_to_reloc += delta_progman - delta_localMem;
										}
										++typeoffset;
										--countOfBlock;
									} while (countOfBlock);
								}
								pRelocInLocalMem = (PIMAGE_BASE_RELOCATION)((char*)pRelocInLocalMem + pRelocInLocalMem->SizeOfBlock);
							}
						}
					}

					if (WriteProcessMemory(hProcess, pProgman_mem, pLocalMem, image_size, 0i64))
					{
						SIZE_T NumberOfBytesWritten = 0;
						WriteProcessMemory(hProcess, (char*)pProgman_mem + image_size, lpThreadArgs, BUFFER_SIZE, &NumberOfBytesWritten);
						HANDLE hTread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)((char*)pProgman_mem + ((char*)callBack - (char*)hModule)),
							(char*)pProgman_mem + image_size, 0, 0i64);
						WaitForSingleObject(hTread, 15 * 1000);
						TerminateThread(hTread, 0);
						CloseHandle(hTread);
					}
					VirtualFree(pLocalMem, 0i64, MEM_RELEASE);
				}
				ret = VirtualFreeEx(hProcess, pProgman_mem, 0i64, MEM_RELEASE);
			}
		}
	}
	return ret;
}

//Uncomment this for dbgviewer in release build mode 
//#define _DEBUG 1 
// Win10Pin2TB  "C:\HOME_NGQ\DriverMonitor\monitor.exe" "pin to taskbar"
int main()
{
	HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	LPWSTR commandline = GetCommandLine();
	int pNumArgs = 0;
	LPWSTR* szArglist = CommandLineToArgvW(commandline, &pNumArgs);
	if (pNumArgs < 3)
	{
		WriteMsg2Console(commandline);
		OutputDebugString(commandline);
		return 1i64;
	}

	WriteMsg2Console(commandline);

	WCHAR fileName[MAX_PATH] = { 0 };
	memset(fileName, 0, 2 * MAX_PATH);
	WCHAR folder[MAX_PATH] = { 0 };
	memset(folder, 0, 2 * MAX_PATH);
	wcscpy_s(folder, szArglist[1]);
	wcscpy_s(fileName, szArglist[1]);

	DWORD fileAttributes = GetFileAttributes(folder);
	if ((fileAttributes == INVALID_FILE_ATTRIBUTES || fileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		WriteMsg2Console(L"Can't get file attributes");
		SetConsoleTextAttribute(stdHandle, 0xAu); //BACKGROUND_BLUE
		ExitProcess(0);
	}

	char buffer[BUFFER_SIZE] = { 0 };
	memset(buffer, 0, BUFFER_SIZE);
	PathStripPath(fileName);
	memcpy_s(buffer, BUFFER_SIZE, fileName, MAX_PATH * 2);

	PathRemoveFileSpec(folder);
	memcpy_s(buffer + MAX_PATH * 2, BUFFER_SIZE, folder, MAX_PATH * 2);

	WCHAR command[COMMAND_BUFFER_SIZE] = { 0 };
	wcscpy_s(command, szArglist[2]);
	memcpy_s(buffer + MAX_PATH * 4, BUFFER_SIZE, command, COMMAND_BUFFER_SIZE);

	int opCode = 0;
	if (iswdigit((wint_t)*command))
	{
		StrToIntEx(command, 0, &opCode);
		memcpy_s(buffer + MAX_PATH * 4 + COMMAND_BUFFER_SIZE, BUFFER_SIZE, &opCode, sizeof(int));
	}
	else
	{
		_wcslwr_s(command);
	}

	HWND hProgman = FindWindowW(L"Progman", NULL);
	if (hProgman == NULL) {
		(L"Can't get file attributes");
		WriteMsg2Console(L"Can't find progman");
	}

	printf(buffer);

	DWORD 	dwProcessId = 0;
	GetWindowThreadProcessId(hProgman, &dwProcessId);
	if (dwProcessId)
	{
		HANDLE hExplorer = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, 0, dwProcessId);
		if (hExplorer != INVALID_HANDLE_VALUE)
		{
			InjectFun2Explorer(buffer, hExplorer, thread_func);
		}
		else
		{
			WriteMsg2Console(L"OpenProcess error");
		}
	}
}
