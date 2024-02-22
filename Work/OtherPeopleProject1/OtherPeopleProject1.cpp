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

	static HWND hTaskbarWnd;
	hTaskbarWnd = FindWindowW(L"Shell_TrayWnd", NULL);
	if (!hTaskbarWnd)
		MessageBoxA(NULL, "FindWindowW 失败", "OK", MB_OK);

	HWND hTaskSwWnd = (HWND)GetPropW(hTaskbarWnd, L"TaskbandHWND");
	if (!hTaskSwWnd)
		MessageBoxA(NULL, "GetPropW 失败", "OK", MB_OK);


	unsigned int x = 0;
	unsigned int* y = &x;
	_POINTL z;
	z.x = 0x12F;
	z.y = 0x38A;
	LONG_PTR lpTaskSwLongPtr = GetWindowLongPtr(hTaskSwWnd, 0);
	if (!lpTaskSwLongPtr)
		MessageBoxA(NULL, "GetWindowLongPtr 失败", "OK", MB_OK);
	//MessageBoxA(NULL, "EV_TASK_SW_TASK_GROUPS_HDPA 开始", "OK", MB_OK);

	//LONG_PTR* plp = (LONG_PTR*)*(HDPA*)(lpTaskSwLongPtr + 0x158);	// 一半可以一半不行
	//LONG_PTR* plp = (LONG_PTR*)*(HDPA*)(lpTaskSwLongPtr + 0x160); // 完全不行
	//LONG_PTR* plp = (LONG_PTR*)*(HDPA*)(lpTaskSwLongPtr + 0x150); // 完全不行
	//LONG_PTR* plp = (LONG_PTR*)*(HDPA*)(lpTaskSwLongPtr + 0x148); // 注入了, 但是不崩溃
	//LONG_PTR* plp = (LONG_PTR*)*(HDPA*)(lpTaskSwLongPtr + 0x140); // 完全不行
	//LONG_PTR* plp = (LONG_PTR*)*(HDPA*)(lpTaskSwLongPtr + 0x120); // 完全不行
	//LONG_PTR* plp = (LONG_PTR*)*(HDPA*)(lpTaskSwLongPtr + 0x118); // 完全不行


	CoInitialize(NULL);

	// 获取桌面的 IShellFolder 接口
	LPSHELLFOLDER pDesktopFolder;
	SHGetDesktopFolder(&pDesktopFolder);

	// 文件路径
	LPCWSTR filePath = L"C:\\path\\to\\your\\file.txt";

	// 使用 IShellFolder 接口的 ParseDisplayName 方法来解析文件路径并获取 FolderItem 对象
	FolderItem* pFolderItem;
	ULONG pchEaten;
	ULONG pdwAttributes;
	// 定义一个 ITEMIDLIST 来表示要获取的项目
	LPITEMIDLIST pidlItem;
	HRESULT hr = pDesktopFolder->ParseDisplayName(NULL, NULL, (LPWSTR)filePath, &pchEaten, &pidlItem, &pdwAttributes);
	if (SUCCEEDED(hr)) {
		// 获取 FolderItem 对象
		pDesktopFolder->GetUIObjectOf(NULL, 1, &pidlItem, IID_FolderItem, NULL, (void**)&pFolderItem);

		// 使用 FolderItem 对象进行操作，比如获取文件路径、属性等

		// 释放资源
		pFolderItem->Release();
		CoTaskMemFree(pidlItem); // 释放 ITEMIDLIST 对象
	}
	else {
		// 处理解析文件路径失败的情况
	}

	//if (true)
	//{
	//	HANDLE hProp = GetPropW(hTaskSwWnd, L"OleDropTargetInterface");
	//	if (!hProp)
	//		MessageBoxA(NULL, "GetPropW 失败", "OK", MB_OK);
	//	__int64 v12;
	//	v12 = *((uintptr_t*)hProp + 139);
	//	char buffer3[17]; // 16 个字符的缓冲区，用于存储地址的字符串表示
	//	sprintf_s(buffer3, "%p", (void*)v12); // 将地址格式化成字符串
	//	MessageBoxA(NULL, buffer3, "OK", MB_OK);
	//	//int task_groups_count = (int)plp[0];
	//	//LONG_PTR** task_groups = (LONG_PTR**)plp[1];
	//	//LONG_PTR* task_group = task_groups[0];
	//	////int x = (((*(int*)(baseAddress + 0x5D)) / 8) + 3);

	//	//char* x = ((char*)(absoluteAddress)+0x5D + 0x3);
	//	////MessageBoxA(NULL, AddressToAnsiString((void*)x).data(), "OK", MB_OK);
	//	//char buffer3[17]; // 16 个字符的缓冲区，用于存储地址的字符串表示
	//	//sprintf_s(buffer3, "%p", (void*)*(short*)x); // 将地址格式化成字符串
	//	////MessageBoxA(NULL, buffer3, "OK", MB_OK);

	//	* ((unsigned long long**)hProp + (int)((int)*(short*)x / 8)) = (unsigned long long*)(struct ITaskGroup*)(*(__int64(__fastcall**)(__int64))(*(uintptr_t*)v12 + 48i64))(v12);;
	//	((RealCTaskListWnd__Drop)absoluteAddress)(hProp, IDataObject1, 0, z, y);
	//}
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

int main()
{
	HWND hProgman = FindWindowW(L"Progman", NULL);
	if (hProgman == NULL) {
		(L"Can't get file attributes");
		WriteMsg2Console(L"Can't find progman");
	}

	DWORD 	dwProcessId = 0;
	GetWindowThreadProcessId(hProgman, &dwProcessId);
	if (dwProcessId)
	{
		HANDLE hExplorer = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, 0, dwProcessId);
		if (hExplorer != INVALID_HANDLE_VALUE)
		{
			InjectFun2Explorer(NULL, hExplorer, thread_func);
		}
		else
		{
			WriteMsg2Console(L"OpenProcess error");
		}
	}
}
