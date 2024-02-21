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
	MessageBoxA(NULL, "CTaskBand__InsertPinnedItem", "OK", MB_OK);
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
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());


	// 计算函数的绝对地址
	uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)); // 获取模块基址
	uintptr_t relativeOffset = 0x2357E0; // 替换为函数在目标程序中的相对地址
	absoluteAddress = baseAddress + relativeOffset; // 计算绝对地址

	// 将绝对地址转换为函数指针
	//FunctionPtr function = reinterpret_cast<FunctionPtr>(absoluteAddress);
	MessageBoxA(NULL, AddressToAnsiString((void*)baseAddress).data(), "OK", MB_OK);
	printf("absoluteAddress: %p", baseAddress);

	// 调用函数
	//function();

	// 获取原始 wsasend 函数的地址
	//RealWsaSend = (LPFN_WSASEND)GetProcAddress(GetModuleHandle("ws2_32.dll"), "WSASend");

	RealCTaskListWnd__Drop ptr = (RealCTaskListWnd__Drop)absoluteAddress;
	// 对 wsasend 函数进行 Hook，将其替换为自定义的 HookedWsaSend 函数
	DetourAttach(&(PVOID&)absoluteAddress, CTaskListWnd__Drop);
	//ptr(nullptr, nullptr, nullptr, nullptr, 3);

	// 完成 Hook 事务
	DetourTransactionCommit();
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
	IDataObject1 = GetDataObjectForFile("C:\\Users\\Administrator\\Desktop\\inst.exe", NULL);
	unsigned int x = 7;
	unsigned int* y = &x;
	_POINTL z;
	z.x = 0x12F;
	z.y = 0x38A;


	LONG_PTR* plp = (LONG_PTR*)*EV_TASK_SW_TASK_GROUPS_HDPA();
	if (plp && (int)plp[0] > 0)
	{
		int task_groups_count = (int)plp[0];
		LONG_PTR** task_groups = (LONG_PTR**)plp[1];
		LONG_PTR* task_group = task_groups[0];
		*((unsigned long long**)hProp + 142) = (unsigned long long*)task_group;
	}
	((RealCTaskListWnd__Drop)absoluteAddress)(hProp, IDataObject1, 1, z, y);


	//MessageBoxA(NULL, AddressToAnsiString((void*)a3).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)z.x).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)z.y).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)*y).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)hProp).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)IDataObject1).data(), "OK", MB_OK);
	//MessageBoxA(NULL, AddressToAnsiString((void*)absoluteAddress).data(), "OK", MB_OK);

	MessageBoxA(NULL, "CTaskListWnd__Drop", "OK", MB_OK);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		MessageBoxA(NULL, "DLL_PROCESS_DETACH", "OK", MB_OK);
		//HookInsertPin();
		HookDrop();
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		//FreeInsertPin();
	}
	return TRUE;
}

