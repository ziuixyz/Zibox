#include "StdAfx.h"
#include "ConfigFile.h"
#include "shlobj.h"
#include "fstream"

//定义私有静态成员变量
CDuiString CConfigFile::m_path;

//静态方法 Init 初始化
void CConfigFile::Init()
{
	//定义变量 TCHAR(字符串类型)  my_documents(我的文档所在目录)
	TCHAR my_documents[MAX_PATH];
	//SHGetFolderPath()函数 WindowsAPI函数 获取文件夹目录 CSIDL_MYDOCUMENTS(我的文档) 获取成功会写入my_documents
	//此函数参见 https://docs.microsoft.com/zh-cn/windows/win32/api/shlobj_core/nf-shlobj_core-shgetfolderpatha
	if (SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, my_documents) != S_OK) {
		//如果找不到目录直接返回 否则继续执行
		return;
	}
	//定义变量 CDuiString(字符串类型 其实是一个类 只是个工具类) path(路径)
	CDuiString path;
	//拼接字符串 使用my_documents替换占位符%s 得到完整的路径 C:\\User\\my\\Documents\\Zibox.ini
	path.Format(TEXT("%s\\Zibox.ini"), my_documents);
	//将路径存储到私有变量m_path内
	m_path = path;
}

//静态方法 IsConfigFile 是否存在配置文件
bool CConfigFile::IsConfigFile()
{
	//定义变量 ifstream(C艹的文本流 用来读取文件)
	ifstream file;
	//.open(路径,模式) in是读取 m_path就是初始化时设置的文件路径
	file.open(m_path, ios::in);
	//如果 此文件存在 就返回true 否则返回false
	if (file) return true;
	return false;
}

//静态方法 RemoveConfig 移除配置文件
int CConfigFile::RemoveConfig()
{
	return DeleteFile(m_path);
}

//非静态方法 GetConfigInt 获取配置文件 返回整型类型
int CConfigFile::GetConfigInt(LPCTSTR Title, LPCTSTR key, int def)
{
	return GetPrivateProfileInt(Title, key, def, m_path);
}

//非静态方法 GetConfigString 获取配置文件 返回字符串类型
CDuiString CConfigFile::GetConfigString(LPCTSTR Title, LPCTSTR key, LPCTSTR def)
{
	TCHAR str[MAX_PATH];
	GetPrivateProfileString(Title, key, def, str, MAX_PATH, m_path);
	return str;
}

//非静态方法 SetConfigString 设置配置文件 写入字符串类型
int CConfigFile::SetConfigString(LPCTSTR Title, LPCTSTR key, LPCTSTR value)
{
	return WritePrivateProfileString(Title, key, value, m_path);
}

UINT CConfigFile::GetHotKeyMod(LPCTSTR key, UINT Def)
{
	UINT Mod;
	int a = GetConfigInt(TEXT("HotKey"), key);
	switch (a) {
	case 0: Mod = NULL; break;
	case 1: Mod = MOD_CONTROL; break;
	case 2: Mod = MOD_SHIFT; break;
	case 3: Mod = MOD_ALT; break;
	case 4: Mod = MOD_CONTROL | MOD_SHIFT; break;
	case 5: Mod = MOD_CONTROL | MOD_ALT; break;
	case 6: Mod = MOD_SHIFT | MOD_ALT; break;
	case 7: Mod = MOD_CONTROL | MOD_SHIFT | MOD_ALT; break;
	default: Mod = Def;
	}
	return Mod;
}

UINT CConfigFile::GetHotKeyVk(LPCTSTR key, UINT Def)
{
	UINT Vk;
	int a = GetConfigInt(TEXT("HotKey"), key);
	switch (a) {
	case 0: Vk = VK_F1; break;
	case 1: Vk = VK_F2; break;
	case 2: Vk = VK_F3; break;
	case 3: Vk = VK_F4; break;
	case 4: Vk = VK_F5; break;
	case 5: Vk = VK_F6; break;
	case 6: Vk = VK_F7; break;
	case 7: Vk = VK_F8; break;
	case 8: Vk = VK_F9; break;
	case 9: Vk = VK_F10; break;
	case 10: Vk = VK_F11; break;
	case 11: Vk = VK_F12; break;
	default: Vk = Def;
	}
	return Vk;
}
