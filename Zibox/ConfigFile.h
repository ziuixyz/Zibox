#pragma once
//�� ConfigFile��
class CConfigFile
{
//˽�з���
private:

//��������
public:
	//��̬���� Init ��ʼ��
	static void Init();
	//��̬���� IsConfigFile �Ƿ���������ļ�
	static bool IsConfigFile();
	//��̬���� RemoveConfig �Ƴ������ļ�
	static int RemoveConfig();
	//�Ǿ�̬���� GetConfigInt ��ȡ�����ļ� ������������
	int GetConfigInt(LPCTSTR Title, LPCTSTR key, int def = -1);
	//�Ǿ�̬���� GetConfigString ��ȡ�����ļ� �����ַ�������
	CDuiString GetConfigString(LPCTSTR Title, LPCTSTR key, LPCTSTR def = NULL);
	//�Ǿ�̬���� SetConfigString ���������ļ� д���ַ�������
	int SetConfigString(LPCTSTR Title, LPCTSTR key, LPCTSTR value);


	UINT GetHotKeyMod(LPCTSTR key, UINT Def);
	UINT GetHotKeyVk(LPCTSTR key, UINT Def);
//˽�б���
private:
	//��̬���� �ļ�·��
	static CDuiString m_path;
//��������
public:

};

