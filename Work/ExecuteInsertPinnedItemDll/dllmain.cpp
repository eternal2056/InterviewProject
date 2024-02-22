// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <atlbase.h> // 包含 CComPtr 类的头文件


int nWinVersion = WIN_VERSION_10_19H1;

__int64 __fastcall CTaskListWnd__DragEnter(
	void* _this,
	struct IDataObject* a2,
	unsigned int a3,
	struct _POINTL a4,
	IUnknown* punk);

WCHAR* ConvertToWideChar(const char* szSource) {
	// 计算转换后需要的缓冲区大小
	int nLength = MultiByteToWideChar(CP_UTF8, 0, szSource, -1, nullptr, 0);
	if (nLength == 0) {
		return nullptr;
	}

	// 分配缓冲区
	WCHAR* wszTarget = new WCHAR[nLength];
	if (!wszTarget) {
		return nullptr;
	}

	// 执行转换
	if (MultiByteToWideChar(CP_UTF8, 0, szSource, -1, wszTarget, nLength) == 0) {
		delete[] wszTarget; // 释放内存
		return nullptr;
	}

	return wszTarget;
}
IDataObject* IDataObject1 = NULL;
IDataObject* GetDataObjectForFile(const char* filePath, HWND hwnd) {
	CComPtr<IShellFolder> pDesktopFolder;
	HRESULT hr = SHGetDesktopFolder(&pDesktopFolder);
	if (FAILED(hr)) {
		return nullptr;
	}

	WCHAR* lpWPath = ConvertToWideChar(filePath);
	LPITEMIDLIST pidl;
	hr = pDesktopFolder->ParseDisplayName(nullptr, nullptr, lpWPath, nullptr, &pidl, nullptr);
	if (FAILED(hr)) {
		return nullptr;
	}
	CComPtr<IShellFolder> pShellFolder;
	LPCITEMIDLIST pidlChild;
	hr = SHBindToParent(pidl, IID_IShellFolder, (void**)&pShellFolder, &pidlChild);
	CoTaskMemFree(pidl);
	if (FAILED(hr)) {
		return nullptr;
	}
	IDataObject* pDataObject = nullptr;
	hr = pShellFolder->GetUIObjectOf(hwnd, 1, &pidlChild, IID_IDataObject, nullptr, (void**)&pDataObject);
	if (FAILED(hr)) {
		return nullptr;
	}
	return pDataObject;
}
void GetSelectedInternernal(CComQIPtr<IWebBrowserApp> pwba) {
	CComQIPtr<IServiceProvider> psp;
	if (FAILED(pwba->QueryInterface(IID_IServiceProvider, reinterpret_cast<void**>(&psp)))) {
		return;
	}
	CComPtr<IShellBrowser> psb;
	if (FAILED(psp->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, reinterpret_cast<LPVOID*>(&psb)))) {
		return;
	}
	CComPtr<IShellView> psv;
	if (FAILED(psb->QueryActiveShellView(&psv))) {
		return;
	}
	CComPtr<IDataObject> dao;
	if (FAILED(psv->GetItemObject(SVGIO_SELECTION, IID_IDataObject, reinterpret_cast<void**>(&dao)))) {
		return;
	}
	//IDataObject1 = dao;
	//ObtainFirstItem(dao);
}
uintptr_t absoluteAddress = NULL;
void getSelectedFromDesktop() {
	// 初始化
	CoInitialize(nullptr);
	CComPtr<IShellWindows> psw;
	CComQIPtr<IWebBrowserApp> pwba;
	// 获取 IShellWindows 实例
	if (FAILED(psw.CoCreateInstance(CLSID_ShellWindows)))
		return;
	VARIANT pvarLoc;
	VariantInit(&pvarLoc);
	long phwnd;
	// 获取 IWebBrowserApp 实例
	if (FAILED(psw->FindWindowSW(&pvarLoc, &pvarLoc, SWC_DESKTOP, &phwnd, SWFO_NEEDDISPATCH, reinterpret_cast<IDispatch**>(
		&pwba))))
		return;
	GetSelectedInternernal(pwba);
}

// 最终提取文件地址
//void ObtainFirstItem(CComPtr<IDataObject> dao) {
//	FORMATETC formatetc;
//	STGMEDIUM medium = { sizeof medium };
//	formatetc.cfFormat = CF_HDROP;
//	formatetc.ptd = nullptr;
//	formatetc.dwAspect = DVASPECT_CONTENT;
//	formatetc.lindex = -1;
//	formatetc.tymed = TYMED_HGLOBAL;
//	medium.tymed = TYMED_HGLOBAL;
//	if (FAILED(dao->GetData(&formatetc, &medium)))
//		return;
//	// 获取选中数量
//	int n = DragQueryFile(HDROP(medium.hGlobal), 0xFFFFFFFF, nullptr, 0);
//	if (n < 1)
//		return;
//	WCHAR buffer[260] = { '\0' };
//	// 选择第一个
//	DragQueryFile(HDROP(medium.hGlobal), 0, buffer, MAX_PATH - 1);
//	// 输出
//	std::wcout << buffer << std::endl;
//}

std::string AddressToAnsiString(void* address)
{
	char buffer[17]; // 16 个字符的缓冲区，用于存储地址的字符串表示
	sprintf_s(buffer, "%p", address); // 将地址格式化成字符串
	return std::string(buffer); // 将字符数组转换成 std::string 对象并返回
}
typedef __int64(*RealCTaskBand__InsertPinnedItem)(
	void* __this,
	struct ITaskListUI* a2,
	struct IDataObject* a3,
	struct ITaskGroup* a4,
	__int16 a5);

typedef __int64(*RealCTaskListWnd__Drop)(
	void* __this,
	struct IDataObject* a2,
	unsigned int a3,
	struct _POINTL a4,
	unsigned int* a5
	);

void* __this1 = NULL;
bool isFirst = true;
ITaskListUI* ITaskListUI1 = NULL;
ITaskGroup* ITaskGroup1 = NULL;
__int64 __fastcall CTaskBand__InsertPinnedItem(
	void* __this,
	struct ITaskListUI* a2,
	struct IDataObject* a3,
	struct ITaskGroup* a4,
	__int16 a5)
{
	//if (isFirst) {
	//	__this1 = __this;
	//	ITaskListUI1 = a2;
	//	IDataObject1 = a3;
	//	ITaskGroup1 = a4;
	//	isFirst = false;
	//}
	//else {
	//	((RealCTaskBand__InsertPinnedItem)absoluteAddress)(__this1, ITaskListUI1, IDataObject1, ITaskGroup1, 0);
	//getSelectedFromDesktop();
	//ObtainDataObjectFromPath(L"C:\\Users\\Administrator\\Desktop\\Xenos64.exe");
	IDataObject1 = GetDataObjectForFile("C:\\Users\\Administrator\\Desktop\\inst.exe", NULL);
	((RealCTaskBand__InsertPinnedItem)absoluteAddress)(__this, a2, IDataObject1, a4, 0);
	//}










	//hr = CoInitialize(nullptr);
	//if (FAILED(hr)) {
	//	MessageBoxA(NULL, "Failed to initialize COM environment", "OK", MB_OK);
	//	return 1;
	//}
	//// 从数据对象中检索数据
	//FORMATETC format;
	//STGMEDIUM medium;
	//format.cfFormat = CF_TEXT; // 数据格式为文本
	//format.ptd = nullptr;
	//format.dwAspect = DVASPECT_CONTENT;
	//format.lindex = -1;
	//format.tymed = TYMED_HGLOBAL;

	//hr = a3->GetData(&format, &medium);
	//if (SUCCEEDED(hr)) {
	//	// 成功获取数据，处理数据
	//	if (medium.hGlobal != nullptr) {
	//		// 从全局内存中获取数据
	//		char* pData = static_cast<char*>(GlobalLock(medium.hGlobal));
	//		if (pData != nullptr) {
	//			MessageBoxA(NULL, pData, "OK", MB_OK);
	//			GlobalUnlock(medium.hGlobal);
	//		}
	//		// 释放全局内存
	//		GlobalFree(medium.hGlobal);
	//	}
	//}
	//else {
	//	MessageBoxA(NULL, "no pData", "OK", MB_OK);
	//}
	//MessageBoxA(NULL, "CTaskBand__InsertPinnedItem", "OK", MB_OK);
	return 0;
}
__int64 CTaskListWnd__Drop(
	void* __this,
	struct IDataObject* a2,
	unsigned int a3,
	struct _POINTL a4,
	unsigned int* a5
)
{
	static HWND hTaskbarWnd;
	hTaskbarWnd = FindWindowW(L"Shell_TrayWnd", NULL);
	//if (!hTaskbarWnd)
	//	return EXE_ERR_NO_TASKBAR;

	HWND hTaskSwWnd = (HWND)GetPropW(hTaskbarWnd, L"TaskbandHWND");
	//if (!hTaskSwWnd)
	//	return EXE_ERR_NO_TASKLIST;

	HWND hTaskListWnd = FindWindowExW(hTaskSwWnd, NULL, L"MSTaskListWClass", NULL);
	//if (!hTaskListWnd)
	//	return EXE_ERR_NO_TASKLIST;

	HANDLE hProp = GetPropW(hTaskSwWnd, L"OleDropTargetInterface");
	//CTaskListWnd x;
	//if (isFirst) {
	//	__this1 = __this;
	//	isFirst = false;
	//	return 0;
	//}
	IDataObject1 = GetDataObjectForFile("C:\\Users\\Administrator\\Desktop\\inst.exe", NULL);
	unsigned int x = 4;
	unsigned int* y = &x;
	_POINTL z;
	z.x = 0x12F;
	z.y = 0x38A;
	((RealCTaskListWnd__Drop)absoluteAddress)(hProp, IDataObject1, 0, z, y);

	MessageBoxA(NULL, AddressToAnsiString((void*)a3).data(), "OK", MB_OK);
	MessageBoxA(NULL, AddressToAnsiString((void*)a4.x).data(), "OK", MB_OK);
	MessageBoxA(NULL, AddressToAnsiString((void*)a4.y).data(), "OK", MB_OK);
	MessageBoxA(NULL, AddressToAnsiString((void*)*a5).data(), "OK", MB_OK);

	MessageBoxA(NULL, "CTaskListWnd__Drop", "OK", MB_OK);
	return 0;
}
#include <iostream>
#include <fstream>
// 原始的DispatchMessageW函数指针
typedef BOOL(WINAPI* PFnDispatchMessageW)(const MSG*);

// Detour的DispatchMessageW函数指针
PFnDispatchMessageW TrueDispatchMessageW = NULL;

// 被Detour的DispatchMessageW函数
BOOL WINAPI MyDispatchMessageW(const MSG* lpMsg)
{


	//int xPos = *((int*)(lpMsg->lParam) + 0);
	//int yPos = *((int*)(lpMsg->lParam) + 1);
	//bool leftButtonDown = (GET_KEYSTATE_WPARAM(lpMsg->wParam) & MK_LBUTTON) != 0;

	// 写入消息的具体信息到文件中

	//if (lpMsg->message == 275) {
	//	//if (lpMsg->wParam == 1 && lpMsg->lParam == 0) {
	//	if (lpMsg->lParam != 0) {
	//		return true;
	//	}
	//}
	// 这是啥, 这居然可以
	//if (lpMsg->message == 275) { return true; } // 这是啥, 这居然可以
	IDataObject1 = GetDataObjectForFile("C:\\Users\\Administrator\\Desktop\\inst.exe", NULL);
	//lpMsg->wParam = 0;
	//lpMsg->lParam = 0;

	std::ofstream outputFile("C:\\Users\\Administrator\\Desktop\\my.txt", std::ios_base::app);
	if (!outputFile.is_open())
	{
		// 如果无法打开文件，则直接调用原始的DispatchMessageW函数
		return false;
	}
	outputFile << "---------------------------- " << std::endl;
	outputFile << "Message received:" << std::endl;
	outputFile << "hwnd: " << lpMsg->hwnd << std::endl;
	/*
	WM_MOUSEMOVE		512
	x		1024
	x		1025
	*/
	outputFile << "message: " << lpMsg->message << std::endl;
	outputFile << "wParam: " << lpMsg->wParam << std::endl;
	outputFile << "lParam: " << lpMsg->lParam << std::endl;
	//outputFile << "time: " << lpMsg->time << std::endl;
	//outputFile << "pt.x: " << lpMsg->pt.x << ", pt.y: " << lpMsg->pt.y << std::endl;
	outputFile << "---------------------------- " << std::endl;
	outputFile << std::endl;

	// 关闭文件
	outputFile.close();

	//if (lpMsg->message == 513) return true;
	//if (lpMsg->message == 514) return true;
	//if (lpMsg->message == 516) return true;
	//if (lpMsg->message == 517) return true;
	//if (lpMsg->message == 49351) return false;
	//if (lpMsg->message == 1098) return false;
	//if (lpMsg->message == 1025) return false;
	//if (lpMsg->message == 1028) return false;
	//if (lpMsg->message == 1098) return false;
	//if (lpMsg->message == 1120) return false;
	//if (lpMsg->message == 1187) return false;
	//if (lpMsg->message == 1197) return false;
	//if (lpMsg->message == 1205) return false;
	//if (lpMsg->message == 1267) return false;
	//if (lpMsg->message == 15) return false;
	//if (lpMsg->message == 49192) return false;
	//if (lpMsg->message == 675) return false; // 不行
	//if (lpMsg->message == 96) return false;	// 不行
	// 调用原始的DispatchMessageW函数
	return TrueDispatchMessageW(lpMsg);
}

void HookUser32() {
	//readAndPrintFile("C:\\D_Files\\Project_Driver\\KillHappy\\x64\\Debug\\AddrList.txt");
	//MessageBox(NULL, "This Is From Dll!\nInject Success!", "OK", MB_OK);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());


	// 计算函数的绝对地址
	//uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)); // 获取模块基址
	//uintptr_t relativeOffset = 0x20D1C0; // 替换为函数在目标程序中的相对地址
	//absoluteAddress = baseAddress + relativeOffset; // 计算绝对地址

	//// 将绝对地址转换为函数指针
	////FunctionPtr function = reinterpret_cast<FunctionPtr>(absoluteAddress);
	//MessageBoxA(NULL, AddressToAnsiString((void*)baseAddress).data(), "OK", MB_OK);
	//printf("absoluteAddress: %p", baseAddress);

	// 调用函数
	//function();

	// 获取原始 wsasend 函数的地址
	TrueDispatchMessageW = (PFnDispatchMessageW)GetProcAddress(GetModuleHandle("user32.dll"), "DispatchMessageW");

	//RealCTaskBand__InsertPinnedItem ptr = (RealCTaskBand__InsertPinnedItem)absoluteAddress;
	// 对 wsasend 函数进行 Hook，将其替换为自定义的 HookedWsaSend 函数
	DetourAttach(&(PVOID&)TrueDispatchMessageW, MyDispatchMessageW);
	//ptr(nullptr, nullptr, nullptr, nullptr, 3);

	// 完成 Hook 事务
	DetourTransactionCommit();
}
// 原始的 SendMessageW 函数指针
typedef LRESULT(WINAPI* PFnSendMessageW)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

// Detour 的 SendMessageW 函数指针
PFnSendMessageW TrueSendMessageW = NULL;

// 被 Detour 的 SendMessageW 函数
LRESULT WINAPI MySendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// 在这里可以添加你自己的逻辑
	std::ofstream outputFile("C:\\Users\\Administrator\\Desktop\\my.txt", std::ios_base::app);
	if (!outputFile.is_open())
	{
		// 如果无法打开文件，则直接调用原始的DispatchMessageW函数
		return false;
	}

	// 都出现好几百次
	if (Msg == 78) { return false; }
	if (Msg == 792) { return false; }
	if (Msg == 4619) { return false; }
	if (Msg == 4608) { return false; }
	if (Msg == 4127) { return false; }
	if (Msg == 20) { return false; }
	if (Msg == 1031) { return false; }

	// 都只出现几十次
	if (Msg == 4289) { return false; }
	if (Msg == 1052) { return false; }
	if (Msg == 1040) { return false; }
	if (Msg == 1184) { return false; }
	if (Msg == 32782) { return false; }
	if (Msg == 641) { return false; }
	if (Msg == 11) { return false; }

	// 都只出现两次
	if (Msg == 1401) { return false; }
	if (Msg == 32772) { return false; }
	if (Msg == 32784) { return false; }
	if (Msg == 4114) { return false; }
	if (Msg == 642) { return false; }

	// 都只出现一次
	//if (Msg == 516) { return false; }
	if (Msg == 123) { return false; }
	if (Msg == 295) { return false; }
	if (Msg == 1025) { return false; }
	if (Msg == 1082) { return false; }
	if (Msg == 1361) { return false; }
	if (Msg == 4137) { return false; }
	if (Msg == 4275) { return false; }


	outputFile << "---------------------------- " << std::endl;
	outputFile << "Message received:" << std::endl;
	outputFile << "hwnd: " << hWnd << std::endl;
	outputFile << "message: " << Msg << std::endl;
	outputFile << "wParam: " << wParam << std::endl;
	outputFile << "lParam: " << lParam << std::endl;
	outputFile << "---------------------------- " << std::endl;
	outputFile << std::endl;

	// 关闭文件
	outputFile.close();
	// 调用原始的 SendMessageW 函数
	return TrueSendMessageW(hWnd, Msg, wParam, lParam);
}
void HookUser32SendMessag() {
	//readAndPrintFile("C:\\D_Files\\Project_Driver\\KillHappy\\x64\\Debug\\AddrList.txt");
	//MessageBox(NULL, "This Is From Dll!\nInject Success!", "OK", MB_OK);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());


	// 计算函数的绝对地址
	//uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)); // 获取模块基址
	//uintptr_t relativeOffset = 0x20D1C0; // 替换为函数在目标程序中的相对地址
	//absoluteAddress = baseAddress + relativeOffset; // 计算绝对地址

	//// 将绝对地址转换为函数指针
	////FunctionPtr function = reinterpret_cast<FunctionPtr>(absoluteAddress);
	//MessageBoxA(NULL, AddressToAnsiString((void*)baseAddress).data(), "OK", MB_OK);
	//printf("absoluteAddress: %p", baseAddress);

	// 调用函数
	//function();

	// 获取原始 wsasend 函数的地址
	TrueSendMessageW = (PFnSendMessageW)GetProcAddress(GetModuleHandle("user32.dll"), "SendMessageW");

	//RealCTaskBand__InsertPinnedItem ptr = (RealCTaskBand__InsertPinnedItem)absoluteAddress;
	// 对 wsasend 函数进行 Hook，将其替换为自定义的 HookedWsaSend 函数
	DetourAttach(&(PVOID&)TrueSendMessageW, MySendMessageW);
	//ptr(nullptr, nullptr, nullptr, nullptr, 3);

	// 完成 Hook 事务
	DetourTransactionCommit();
}
void HookInsertPin() {
	//readAndPrintFile("C:\\D_Files\\Project_Driver\\KillHappy\\x64\\Debug\\AddrList.txt");
	//MessageBox(NULL, "This Is From Dll!\nInject Success!", "OK", MB_OK);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());


	// 计算函数的绝对地址
	uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)); // 获取模块基址
	uintptr_t relativeOffset = 0x20D1C0; // 替换为函数在目标程序中的相对地址
	absoluteAddress = baseAddress + relativeOffset; // 计算绝对地址

	// 将绝对地址转换为函数指针
	//FunctionPtr function = reinterpret_cast<FunctionPtr>(absoluteAddress);
	MessageBoxA(NULL, AddressToAnsiString((void*)baseAddress).data(), "OK", MB_OK);
	printf("absoluteAddress: %p", baseAddress);

	// 调用函数
	//function();

	// 获取原始 wsasend 函数的地址
	//RealWsaSend = (LPFN_WSASEND)GetProcAddress(GetModuleHandle("ws2_32.dll"), "WSASend");

	RealCTaskBand__InsertPinnedItem ptr = (RealCTaskBand__InsertPinnedItem)absoluteAddress;
	// 对 wsasend 函数进行 Hook，将其替换为自定义的 HookedWsaSend 函数
	DetourAttach(&(PVOID&)absoluteAddress, CTaskBand__InsertPinnedItem);
	//ptr(nullptr, nullptr, nullptr, nullptr, 3);

	// 完成 Hook 事务
	DetourTransactionCommit();
}
//typedef VOID(*FunMiProcessLoaderEntry)(ULONG ulEntry, LOGICAL lflag);
#define BLOCKMAXSIZE 409600//每次读取内存的最大大小
BYTE* MemoryData;//每次将读取的内存读入这里
short Next[260];

//特征码转字节集
WORD GetTzmArray(char* Tzm, WORD* TzmArray)
{
	int len = 0;
	WORD TzmLength = strlen(Tzm) / 3 + 1;

	for (int i = 0; i < strlen(Tzm); )//将十六进制特征码转为十进制
	{
		char num[2];
		num[0] = Tzm[i++];
		num[1] = Tzm[i++];
		i++;
		if (num[0] != '?' && num[1] != '?')
		{
			int sum = 0;
			WORD a[2];
			for (int i = 0; i < 2; i++)
			{
				if (num[i] >= '0' && num[i] <= '9')
				{
					a[i] = num[i] - '0';
				}
				else if (num[i] >= 'a' && num[i] <= 'z')
				{
					a[i] = num[i] - 87;
				}
				else if (num[i] >= 'A' && num[i] <= 'Z')
				{
					a[i] = num[i] - 55;
				}

			}
			sum = a[0] * 16 + a[1];
			TzmArray[len++] = sum;
		}
		else
		{
			TzmArray[len++] = 256;
		}
	}
	return TzmLength;
}

//获取Next数组
void GetNext(short* next, WORD* Tzm, WORD TzmLength)
{
	//特征码（字节集）的每个字节的范围在0-255（0-FF）之间，256用来表示问号，到260是为了防止越界
	for (int i = 0; i < 260; i++)
		next[i] = -1;
	for (int i = 0; i < TzmLength; i++)
		next[Tzm[i]] = i;
}
//搜索一块内存
void SearchMemoryBlock(HANDLE hProcess, WORD* Tzm, WORD TzmLength, unsigned __int64 StartAddress, unsigned long size, std::vector<unsigned __int64>& ResultArray)
{

	if (!ReadProcessMemory(hProcess, (LPCVOID)StartAddress, MemoryData, size, NULL))
	{
		//MessageBoxA(NULL, AddressToAnsiString((void*)(StartAddress)).data(), "OK", MB_OK);/
		return;
	}

	for (int i = 0, j, k; i < size;)
	{
		j = i; k = 0;

		for (; k < TzmLength && j < size && (Tzm[k] == MemoryData[j] || Tzm[k] == 256); k++, j++);

		if (k == TzmLength)
		{
			ResultArray.push_back(StartAddress + i);
		}

		if ((i + TzmLength) >= size)
		{
			return;
		}

		int num = Next[MemoryData[i + TzmLength]];
		if (num == -1)
			i += (TzmLength - Next[256]);//如果特征码有问号，就从问号处开始匹配，如果没有就i+=-1
		else
			i += (TzmLength - num);
	}
}

//搜索整个程序
int SearchMemory(HANDLE hProcess, char* Tzm, unsigned __int64 StartAddress, unsigned __int64 EndAddress, int InitSize, std::vector<unsigned __int64>& ResultArray)
{
	int i = 0;
	unsigned long BlockSize;
	MEMORY_BASIC_INFORMATION mbi;

	WORD TzmLength = strlen(Tzm) / 3 + 1;
	WORD* TzmArray = new WORD[TzmLength];

	GetTzmArray(Tzm, TzmArray);
	GetNext(Next, TzmArray, TzmLength);

	//初始化结果数组
	ResultArray.clear();
	ResultArray.reserve(InitSize);

	while (VirtualQueryEx(hProcess, (LPCVOID)StartAddress, &mbi, sizeof(mbi)) != 0)
	{

		//获取可读可写和可读可写可执行的内存块
		if (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_EXECUTE_READWRITE || mbi.Protect == PAGE_EXECUTE_READ)
		{
			//MessageBoxA(NULL, AddressToAnsiString((void*)(mbi.Protect)).data(), "OK", MB_OK);
			i = 0;
			BlockSize = mbi.RegionSize;
			//搜索这块内存
			while (BlockSize >= BLOCKMAXSIZE)
			{
				SearchMemoryBlock(hProcess, TzmArray, TzmLength, StartAddress + (BLOCKMAXSIZE * i), BLOCKMAXSIZE, ResultArray);
				BlockSize -= BLOCKMAXSIZE; i++;
			}
			SearchMemoryBlock(hProcess, TzmArray, TzmLength, StartAddress + (BLOCKMAXSIZE * i), BlockSize, ResultArray);

		}
		StartAddress += mbi.RegionSize;

		if (EndAddress != 0 && StartAddress > EndAddress)
		{
			return ResultArray.size();
		}
	}
	free(TzmArray);
	return ResultArray.size();
}
int GetMiProcessLoaderEntryAddr(uintptr_t ulStartAddress, uintptr_t ulEndAddress, uintptr_t* retFunAddr) {
	// 特征码
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
	//CHAR szCodeFiag2[] = { 0x8B,0xCE,0xF0,0x0F,0xBA,0x29,0x1F };
// 要修改的内存大小

	//BOOL result = 1;

	for (size_t i = ulStartAddress; i < ulEndAddress; i++) {
		//SIZE_T size = 4096; // 假设要修改的内存大小为 4096 字节（一页大小）
// 新的访问权限
		//DWORD newProtect = PAGE_EXECUTE_READWRITE; // 假设要修改为可读写权限

		// 保存原始权限
		//DWORD oldProtect;
		// 修改内存权限
		//BOOL result = VirtualProtect((LPVOID)ulStartAddress, size, newProtect, &oldProtect);
		//MessageBoxA(NULL, AddressToAnsiString((void*)((char*)ulStartAddress + i)).data(), "OK", MB_OK);
		//MessageBoxA(NULL, AddressToAnsiString((void*)*((char*)ulStartAddress + i)).data(), "OK", MB_OK);
		//MessageBoxA(NULL, AddressToAnsiString((void*)(szCodeFlag1)).data(), "OK", MB_OK);
		if (memcmp((void*)i, szCodeFlag1, sizeof(szCodeFlag1)) == 0) {
			MessageBoxA(NULL, "获取成功", "OK", MB_OK);
			MessageBoxA(NULL, AddressToAnsiString((void*)(i)).data(), "OK", MB_OK);
			*retFunAddr = i;
			//return 1;
			//if (memcmp(i + 0x23, szCodeFiag2, 7) == 0) {
			//	return 1;
			//}
		}
	}
	return -1;
}
LONG_PTR lpTaskSwLongPtr;
struct _DPA;
typedef struct _DPA* HDPA;
HDPA* EV_TASK_SW_TASK_GROUPS_HDPA(void)
{
	return (HDPA*)(lpTaskSwLongPtr +
		DO11_3264(0xA8, 0x120, 0xA4, 0x118, 0xA8, 0x120, 0xC0, 0x140, 0xC0, 0x148, 0xC4, 0x150, 0xC0, 0x158, 0xC8, 0x160, , , , , 0xC0, 0x158));
}
void HookDrop() {
	//readAndPrintFile("C:\\D_Files\\Project_Driver\\KillHappy\\x64\\Debug\\AddrList.txt");
	//MessageBox(NULL, "This Is From Dll!\nInject Success!", "OK", MB_OK);
	//DetourTransactionBegin();
	//DetourUpdateThread(GetCurrentThread());


	//// 获取当前线程句柄
	//HANDLE hThread = GetCurrentThread();
	//if (hThread == NULL) {
	//	std::cerr << "Failed to get handle to current thread." << std::endl;
	//}

	//// 获取当前线程所属进程的进程ID
	//DWORD processId = GetProcessIdOfThread(hThread);
	//if (processId == 0) {
	//	std::cerr << "Failed to get process ID of current thread." << std::endl;
	//}

	//// 打开当前线程所属进程的进程句柄
	//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	//if (hProcess == NULL) {
	//	std::cerr << "Failed to open process handle." << std::endl;
	//}
	//std::vector<unsigned __int64> ResultArray;

	// 计算函数的绝对地址
	uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)); // 获取模块基址
	//uintptr_t sizeBase = ((int*)baseAddress)[15]; // 获取模块基址
	//MessageBoxA(NULL, AddressToAnsiString((void*)(sizeBase)).data(), "OK", MB_OK);
	uintptr_t relativeOffset = 0x2357E0; // 替换为函数在目标程序中的相对地址
	GetMiProcessLoaderEntryAddr(baseAddress, baseAddress + relativeOffset + 10000, (uintptr_t*)&absoluteAddress);
	//absoluteAddress = baseAddress + relativeOffset; // 计算绝对地址
	//SearchMemory(hProcess, (char*)"48 89 6C 24 18", baseAddress, baseAddress + 0x260000, 30, ResultArray);
	//SearchMemory(hProcess, (char*)"48 89 6C 24 18", 0, 0x7fffffffffffffff, 30, ResultArray);
	//SearchMemory(hProcess, (char*)"48 89 6C 24 18", baseAddress + relativeOffset, 0x260000, 30, ResultArray);


	//MessageBoxA(NULL, AddressToAnsiString((void*)(hProcess)).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)(2)).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)baseAddress).data(), "OK", MB_OK);


	//for (std::vector<unsigned __int64>::iterator it = ResultArray.begin(); it != ResultArray.end(); it++)
	//{
	//	MessageBoxA(NULL, AddressToAnsiString((void*)(*it)).data(), "OK", MB_OK);
	//}
	// 将绝对地址转换为函数指针
	//FunctionPtr function = reinterpret_cast<FunctionPtr>(absoluteAddress);
	//printf("absoluteAddress: %p", baseAddress);

	// 调用函数
	//function();

	// 获取原始 wsasend 函数的地址
	//RealWsaSend = (LPFN_WSASEND)GetProcAddress(GetModuleHandle("ws2_32.dll"), "WSASend");

	RealCTaskListWnd__Drop ptr = (RealCTaskListWnd__Drop)absoluteAddress;

	static HWND hTaskbarWnd;
	hTaskbarWnd = FindWindowW(L"Shell_TrayWnd", NULL);
	//if (!hTaskbarWnd)
	//	return EXE_ERR_NO_TASKBAR;

	HWND hTaskSwWnd = (HWND)GetPropW(hTaskbarWnd, L"TaskbandHWND");
	//if (!hTaskSwWnd)
	//	return EXE_ERR_NO_TASKLIST;
	lpTaskSwLongPtr = GetWindowLongPtr(hTaskSwWnd, 0);

	HWND hTaskListWnd = FindWindowExW(hTaskSwWnd, NULL, L"MSTaskListWClass", NULL);
	//if (!hTaskListWnd)
	//	return EXE_ERR_NO_TASKLIST;

	HANDLE hProp = GetPropW(hTaskSwWnd, L"OleDropTargetInterface");
	//CTaskListWnd x;
	//if (isFirst) {
	//	__this1 = __this;
	//	isFirst = false;
	//	return 0;
	//}
	IDataObject1 = GetDataObjectForFile("C:\\Windows\\System32\\calc.exe", NULL);
	unsigned int x = 0;
	unsigned int* y = &x;
	_POINTL z;
	z.x = 0x12F;
	z.y = 0x38A;

	MessageBoxA(NULL, "EV_TASK_SW_TASK_GROUPS_HDPA 开始", "OK", MB_OK);


	LONG_PTR* plp = (LONG_PTR*)*EV_TASK_SW_TASK_GROUPS_HDPA();
	if (plp && (int)plp[0] > 0)
	{
		int task_groups_count = (int)plp[0];
		LONG_PTR** task_groups = (LONG_PTR**)plp[1];
		LONG_PTR* task_group = task_groups[0];
		//int x = (((*(int*)(baseAddress + 0x5D)) / 8) + 3);
		char* x = ((char*)(absoluteAddress)+0x5D + 0x3);
		//MessageBoxA(NULL, AddressToAnsiString((void*)x).data(), "OK", MB_OK);
		MessageBoxA(NULL, AddressToAnsiString((void*)*(short*)x).data(), "OK", MB_OK);
		*((unsigned long long**)hProp + (int)((int)*(short*)x / 8)) = (unsigned long long*)task_group;
	}
	((RealCTaskListWnd__Drop)absoluteAddress)(hProp, IDataObject1, 0, z, y);


	//MessageBoxA(NULL, AddressToAnsiString((void*)a3).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)z.x).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)z.y).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)*y).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)hProp).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)IDataObject1).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)absoluteAddress).data(), "OK", MB_OK);

	//MessageBoxA(NULL, "CTaskListWnd__Drop", "OK", MB_OK);

	// 对 wsasend 函数进行 Hook，将其替换为自定义的 HookedWsaSend 函数
	//DetourAttach(&(PVOID&)absoluteAddress, CTaskListWnd__Drop);
	////ptr(nullptr, nullptr, nullptr, nullptr, 3);

	//// 完成 Hook 事务
	//DetourTransactionCommit();
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		//MessageBoxA(NULL, "DLL_PROCESS_DETACH", "OK", MB_OK);s
		//HookInsertPin();
		HookDrop();
		//HookUser32();
		//HookUser32SendMessag();
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		//FreeInsertPin();
	}
	return TRUE;
}

