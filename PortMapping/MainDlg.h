#pragma once
#include "UIMenu.h"
#include "LibuvAdapter.h"
#include "EditUIEx.h"
#include <regex>
#include "MyListItem.h"

using namespace DuiLib;
class CMainDlg : public WindowImplBase,
	public INotifyLoop
{
public:
	CMainDlg();
	virtual ~CMainDlg();
	
protected:
	//实现基类纯虚接口
	virtual CDuiString GetSkinFolder();//设置资源文件夹
	virtual CDuiString GetSkinFile();//设置资源文件
	virtual LPCTSTR GetWindowClassName(void) const;//注册用的窗口类名
	virtual CControlUI* CreateControl(LPCTSTR pstrClass)  override;
public:
	//重写基类虚函数
	virtual void InitWindow() override;//在OnCreate最后调用
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled) override;

	
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	//INotifyLoop
	virtual void NotifyConnectMessage(UINT nType, ConnectInfo* pInfo);
	virtual void NotifyMappingMessage(UINT nType, MappingInfo* pInfo);
	virtual void NotifyGetAllConnectByMapping(ConnectInfo** pInfo, size_t size);//获取某一映射的所有链接信息
private:
	void UpDataList();
	bool CheckNotify(void* p);
	bool RootNotify(void* p);	//用来处理菜单消息
	
	bool ButtonNotify(void* pNotify);//按钮Notify消息
	bool ListNotify(void* pNotify);
	void UpDataConnectList(CControlUI* p_sender);	//刷新连接列表
	//列表Notify消息
	bool ListItemNotify(void* p);	//处理列表项单击消息
	bool CheckPort(void* p);		//检测端口
	bool CheckIP(void* p);			//检测IP

	void GetLocalIP();				//获取本地IP
	void OnMenuItemInit(CMenuElementUI* pMenuItem, LPARAM l_param);


	
	//菜单初始化，在弹出之前调用
	void OnMenuItemClick(LPCWSTR pName, LPARAM l_param);			//菜单项单击
	
	void OnAddClick();												//添加按钮单击
	bool CheckAllInfo();											//检测所有输入的信息是否有效

	void DealWithConnectMsg(WPARAM w_param, ConnectInfo* connect_info);
	void DealWithMappingMsg(WPARAM w_param, MappingInfo* mapping_info);
	//菜单消息处理
	void StartMapping(bool bAllSelect = false);
	void StopMapping(bool bAllSelect = false);
	void DeleteMapping(bool bAllSelect = false);
	typedef bool (CLibuvAdapter::*Func)(MappingInfo *);
	bool MappingOperate(Func fun, bool bAllSelect = false);
	void DeleteConnect(bool bAllSelect = false);

	bool AnlySelect(CListUI* pList);
private:
	CButtonUI*	m_pLeft_hide;		//左边隐藏 显示按钮
	CButtonUI*  m_pBottom_hide;		//下列表隐藏  显示按钮
	CButtonUI*  m_pMenu_hide;		//设置按钮
	CControlUI* m_pLeft_layout;		//左边添加信息的区域

	CListUI* m_pMapping_List;		//映射关系列表
	CListUI* m_pConnect_List;		//链接信息列表

	CEditUIEx*	m_pEdit_agent_port;	//本地端口
	CEditUIEx*	m_pEdit_server_port;//映射端口

	CEditUIEx* m_pEdit_server_ip;	//映射IP

	CComboUI*	m_pCmb_protocol;	//协议类型
	CComboUI*  m_pCmb_agent_ip;		//本地ip

	CButtonUI*	m_pBtn_ADD;			//ADD按钮

	CCheckBoxUI*	m_pCheck_Mapping;
	CCheckBoxUI*	m_pCheck_Connect;

	vector<wstring>		m_vecLocalIP;//本地IP
	CLibuvAdapter*		m_pLibuv;	//调用libuv相关功能

	wregex*				m_pregex_IP;//ip判断的正则表达式

	CMappingListItem*			m_pCur_mapping;

	union
	{
		CMappingListItem*		m_pMapping;
		CConnectListItem*		m_pConnect;
	} m_u_cur;
};

