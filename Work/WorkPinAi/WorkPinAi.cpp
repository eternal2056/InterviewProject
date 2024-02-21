#include <Windows.h>
#include <Shobjidl.h>
#include <ShlObj.h>
#include <iostream>

int main() {
	// 初始化 COM 环境
	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr)) {
		std::cerr << "Failed to initialize COM environment" << std::endl;
		return 1;
	}

	// 创建快捷方式对象
	IShellLink* pShellLink = nullptr;
	hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink, reinterpret_cast<void**>(&pShellLink));
	if (FAILED(hr)) {
		std::cerr << "Failed to create IShellLink instance" << std::endl;
		CoUninitialize();
		return 1;
	}

	// 设置快捷方式路径
	hr = pShellLink->SetPath(L"C:\\Users\\Administrator\\Desktop\\Xenos64.exe"); // 设置快捷方式指向的可执行文件路径
	if (FAILED(hr)) {
		std::cerr << "Failed to set shortcut path" << std::endl;
		pShellLink->Release();
		CoUninitialize();
		return 1;
	}

	// 获取 ITaskbarList3 接口
	ITaskbarList3* pTaskbarList = nullptr;
	hr = CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, reinterpret_cast<void**>(&pTaskbarList));
	if (FAILED(hr)) {
		std::cerr << "Failed to create ITaskbarList3 instance" << std::endl;
		pShellLink->Release();
		CoUninitialize();
		return 1;
	}

	// 将快捷方式添加到任务栏
	hr = pTaskbarList->AddTab(nullptr);
	if (FAILED(hr)) {
		std::cerr << "Failed to add shortcut to taskbar" << std::endl;
	}

	// 释放资源
	pShellLink->Release();
	pTaskbarList->Release();

	// 释放 COM 环境
	CoUninitialize();

	return 0;
}