#include "StdAfx.h"
#include "ConfigFile.h"
#include "shlobj.h"
#include "fstream"

//����˽�о�̬��Ա����
CDuiString CConfigFile::m_path;

//��̬���� Init ��ʼ��
void CConfigFile::Init()
{
	//������� TCHAR(�ַ�������)  my_documents(�ҵ��ĵ�����Ŀ¼)
	TCHAR my_documents[MAX_PATH];
	//SHGetFolderPath()���� WindowsAPI���� ��ȡ�ļ���Ŀ¼ CSIDL_MYDOCUMENTS(�ҵ��ĵ�) ��ȡ�ɹ���д��my_documents
	//�˺����μ� https://docs.microsoft.com/zh-cn/windows/win32/api/shlobj_core/nf-shlobj_core-shgetfolderpatha
	if (SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, my_documents) != S_OK) {
		//����Ҳ���Ŀ¼ֱ�ӷ��� �������ִ��
		return;
	}
	//������� CDuiString(�ַ������� ��ʵ��һ���� ֻ�Ǹ�������) path(·��)
	CDuiString path;
	//ƴ���ַ��� ʹ��my_documents�滻ռλ��%s �õ�������·�� C:\\User\\my\\Documents\\Zibox.ini
	path.Format(TEXT("%s\\Zibox.ini"), my_documents);
	//��·���洢��˽�б���m_path��
	m_path = path;
}

//��̬���� IsConfigFile �Ƿ���������ļ�
bool CConfigFile::IsConfigFile()
{
	//������� ifstream(Cܳ���ı��� ������ȡ�ļ�)
	ifstream file;
	//.open(·��,ģʽ) in�Ƕ�ȡ m_path���ǳ�ʼ��ʱ���õ��ļ�·��
	file.open(m_path, ios::in);
	//��� ���ļ����� �ͷ���true ���򷵻�false
	if (file) return true;
	return false;
}

//��̬���� RemoveConfig �Ƴ������ļ�
int CConfigFile::RemoveConfig()
{
	return DeleteFile(m_path);
}

//�Ǿ�̬���� GetConfigInt ��ȡ�����ļ� ������������
int CConfigFile::GetConfigInt(LPCTSTR Title, LPCTSTR key, int def)
{
	return GetPrivateProfileInt(Title, key, def, m_path);
}

//�Ǿ�̬���� GetConfigString ��ȡ�����ļ� �����ַ�������
CDuiString CConfigFile::GetConfigString(LPCTSTR Title, LPCTSTR key, LPCTSTR def)
{
	TCHAR str[MAX_PATH];
	GetPrivateProfileString(Title, key, def, str, MAX_PATH, m_path);
	return str;
}

//�Ǿ�̬���� SetConfigString ���������ļ� д���ַ�������
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
