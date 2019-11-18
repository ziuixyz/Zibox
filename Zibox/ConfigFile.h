#pragma once
//类 ConfigFile类
class CConfigFile
{
//私有方法
private:

//公共方法
public:
	//静态方法 Init 初始化
	static void Init();
	//静态方法 IsConfigFile 是否存在配置文件
	static bool IsConfigFile();
	//静态方法 RemoveConfig 移除配置文件
	static int RemoveConfig();
	//非静态方法 GetConfigInt 获取配置文件 返回整型类型
	int GetConfigInt(LPCTSTR Title, LPCTSTR key, int def = -1);
	//非静态方法 GetConfigString 获取配置文件 返回字符串类型
	CDuiString GetConfigString(LPCTSTR Title, LPCTSTR key, LPCTSTR def = NULL);
	//非静态方法 SetConfigString 设置配置文件 写入字符串类型
	int SetConfigString(LPCTSTR Title, LPCTSTR key, LPCTSTR value);


	UINT GetHotKeyMod(LPCTSTR key, UINT Def);
	UINT GetHotKeyVk(LPCTSTR key, UINT Def);
//私有变量
private:
	//静态变量 文件路径
	static CDuiString m_path;
//公共变量
public:

};

