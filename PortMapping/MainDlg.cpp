﻿#include "stdafx.h"
#include "MainDlg.h"
#include "MyListItem.h"
#include "UIMenu.h"
#include "EditUIEx.h"
#include <regex>
CMainDlg::CMainDlg() :
	m_pLeft_hide(nullptr), m_pBottom_hide(nullptr), m_pLeft_layout(nullptr),
	m_pMapping_List(nullptr), m_pConnect_List(nullptr), m_pMenu_hide(nullptr),
	m_pEdit_agent_port(nullptr), m_pEdit_server_port(nullptr), m_pEdit_server_ip(nullptr),
	m_pCmb_protocol(nullptr)
{
}


CMainDlg::~CMainDlg()
{
}

CDuiString CMainDlg::GetSkinFolder()
{
	return L"";
}
//返回xml文件资源的资源ID
CDuiString CMainDlg::GetSkinFile()
{
	return L"skin.xml";
}

LPCTSTR CMainDlg::GetWindowClassName(void) const
{
	return L"CMainDlg_Duilib";
}

CControlUI* CMainDlg::CreateControl(LPCTSTR pstrClass)
{
	if (wcsncmp(pstrClass, DUI_CTR_EDITEX, wcslen(pstrClass)) == 0)
	{
		return new CEditUIEx;
	}
	return nullptr;
}

void CMainDlg::InitWindow()
{
	//获取控件指针
	CControlUI* pCur = nullptr;
	pCur = m_PaintManager.FindSubControlByName(nullptr, L"btn_left");
	if (pCur->GetInterface(DUI_CTR_BUTTON))
	{
		m_pLeft_hide = static_cast<CButtonUI*>(pCur);
		m_pLeft_hide->OnNotify += MakeDelegate(this, &CMainDlg::ButtonNotify);
	}

	pCur = m_PaintManager.FindSubControlByName(nullptr, L"btn_bottom");
	if (pCur->GetInterface(DUI_CTR_BUTTON))
	{
		m_pBottom_hide = static_cast<CButtonUI*>(pCur);
		m_pBottom_hide->OnNotify += MakeDelegate(this, &CMainDlg::ButtonNotify);
	}

	pCur = m_PaintManager.FindSubControlByName(nullptr, L"menubtn");
	if (pCur->GetInterface(DUI_CTR_BUTTON))
	{
		m_pMenu_hide = static_cast<CButtonUI*>(pCur);
		m_pMenu_hide->OnNotify += MakeDelegate(this, &CMainDlg::ButtonNotify);
	}

	pCur = m_PaintManager.FindSubControlByName(nullptr, L"mapping_list");
	if (pCur->GetInterface(DUI_CTR_LIST))
	{
		m_pMapping_List = static_cast<CListUI*>(pCur);
		m_pMapping_List->OnNotify += MakeDelegate(this, &CMainDlg::ListNotify);
	}

	pCur = m_PaintManager.FindSubControlByName(nullptr, L"connect_list");
	if (pCur->GetInterface(DUI_CTR_LIST))
	{
		m_pConnect_List = static_cast<CListUI*>(pCur);
		m_pConnect_List->OnNotify += MakeDelegate(this, &CMainDlg::ListNotify);
	}
	m_pLeft_layout = m_PaintManager.FindSubControlByName(nullptr, L"left_layout");
	//端口编辑控件
	pCur = m_PaintManager.FindSubControlByName(m_pLeft_layout, L"local_port");
	if (pCur->GetInterface(DUI_CTR_EDITEX))
	{
		m_pEdit_agent_port = static_cast<CEditUIEx*>(pCur);
		m_pEdit_agent_port->OnCheck += MakeDelegate(this, &CMainDlg::CheckPort);
		m_pEdit_agent_port->SetText(m_pEdit_agent_port->GetText());
	}
	pCur = m_PaintManager.FindSubControlByName(m_pLeft_layout, L"mapping_port");
	if (pCur->GetInterface(DUI_CTR_EDITEX))
	{
		m_pEdit_server_port = static_cast<CEditUIEx*>(pCur);
		m_pEdit_server_port->OnCheck += MakeDelegate(this, &CMainDlg::CheckPort);
		m_pEdit_server_port->SetText(m_pEdit_server_port->GetText());
	}
	//IP
	pCur = m_PaintManager.FindSubControlByName(m_pLeft_layout, L"mapping_ip");
	if (pCur->GetInterface(DUI_CTR_EDITEX))
	{
		m_pEdit_server_ip = static_cast<CEditUIEx*>(pCur);
		m_pEdit_server_ip->OnCheck += MakeDelegate(this, &CMainDlg::CheckIP);
		m_pEdit_server_ip->SetText(m_pEdit_server_ip->GetText());
	}

	pCur = m_PaintManager.FindSubControlByName(m_pLeft_layout, L"protocol");
	if (pCur->GetInterface(DUI_CTR_COMBO))
	{
		m_pCmb_protocol = static_cast<CComboUI*>(pCur);
		m_pCmb_protocol->SelectItem(0);
	}
	pCur = m_PaintManager.FindSubControlByName(m_pLeft_layout, L"local_ip");
	if (pCur->GetInterface(DUI_CTR_COMBO))
	{
		m_pCmb_agent_ip = static_cast<CComboUI*>(pCur);
		m_pCmb_agent_ip->SelectItem(0);
	}
	
	if (m_PaintManager.GetRoot())
	{
		m_PaintManager.GetRoot()->OnNotify += MakeDelegate(this, &CMainDlg::RootNotify);
	}
	Test();
}
bool CMainDlg::CheckPort(void* p)
{
	CheckInfo* pInfo = (CheckInfo*)p;
	if (!pInfo)
		return true;
	for (size_t i = 0; i < pInfo->m_content.GetLength(); i++)
	{
		if (pInfo->m_content[i] < '0' || pInfo->m_content[i] > '9')
		{
			pInfo->m_waring_info = L"端口应该为1-65535";
			return false;
		}
			
	}
	int nport = _wtoi(pInfo->m_content);
	if (nport < 1 || nport > 65535)
	{
		pInfo->m_waring_info = L"端口应该为1-65535";
		return false;
	}
	return true;
}

bool CMainDlg::CheckIP(void* p)
{
	CheckInfo* pInfo = (CheckInfo*)p;
	if (!pInfo)
		return true;
	wregex pattern(L"^((25[0-5]|2[0-4]\d|[01]?\d\d?)($|(?!\.$)\.)){4}$");

	if (!regex_match(pInfo->m_content.GetData(), pattern))
	{
		pInfo->m_waring_info = L"IP格式错误！";
		return false;
	}
	return true;
}

bool CMainDlg::RootNotify(void* p)
{
	TNotifyUI* pNotify = (TNotifyUI*)p;
	if (!pNotify)
		return false;
	if (pNotify->sType == DUI_MSGTYPE_MENUITEM_INIT)
	{
		OnMenuItemInit((CMenuElementUI*)pNotify->wParam, pNotify->lParam);
	}
	else if (pNotify->sType == DUI_MSGTYPE_MENUITEM_CLICK)
	{
		OnMenuItemClick(LPCWSTR(pNotify->wParam), pNotify->lParam);
	}
	return false;
}
LRESULT CMainDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	//屏蔽escape键
	if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE)
	{
		bHandled = true;
		return S_OK;
	}
	return WindowImplBase::MessageHandler(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_USER_MENUITEM_CLICK)
	{
		m_PaintManager.SendNotify(m_PaintManager.GetRoot(), DUI_MSGTYPE_MENUITEM_CLICK, wParam, lParam);
		bHandled = true;
		return S_OK;
	}
	bHandled = false;
	return 0;
}

void CMainDlg::OnMenuItemInit(CMenuElementUI* pMenuItem, LPARAM l_param)
{
	if (!pMenuItem)
		return;
	int a = 10;
}

void CMainDlg::OnMenuItemClick(LPCWSTR pName, LPARAM l_param)
{
	if (!pName)
		return;
	MessageBox(m_hWnd, pName, L"菜单单击", MB_OK);
	int a = 10;
}

bool CMainDlg::ButtonNotify(void* pNotify)
{
	TNotifyUI* pNotifyUI = (TNotifyUI*)pNotify;
	if (!pNotifyUI)
		return true;
	if (pNotifyUI->sType == DUI_MSGTYPE_CLICK)//按钮单击
	{
		bool bvisible;
		if (pNotifyUI->pSender == m_pLeft_hide)
		{
			bvisible = m_pLeft_layout->IsVisible();
			m_pLeft_layout->SetVisible(!bvisible);
			m_pLeft_hide->SetText(!bvisible ? L"◀" : L"▶");
		}
		else if (pNotifyUI->pSender == m_pBottom_hide)
		{
			bvisible = m_pConnect_List->IsVisible();
			m_pConnect_List->SetVisible(!bvisible);
			m_pBottom_hide->SetText(!bvisible ? L"▼" : L"▲");
		}
		else if (pNotifyUI->pSender == m_pMenu_hide)
		{

		}
	}
	return true;
}

bool CMainDlg::ListNotify(void* pNotify)
{
	TNotifyUI* pNotifyUI = (TNotifyUI*)pNotify;
	if (!pNotifyUI || !pNotifyUI->pSender)
		return true;
	if (pNotifyUI->sType == DUI_MSGTYPE_MENU)//菜单弹出消息
	{
		CListUI* pList = (CListUI*)pNotifyUI->pSender;
		RECT rcHead = pList->GetHeader()->GetPos();
		if (PtInRect(&rcHead, pNotifyUI->ptMouse))
			return true;
		POINT pt = pNotifyUI->ptMouse;
		ClientToScreen(m_hWnd, &pt);
		if (pList == m_pMapping_List)
		{
			STRINGorID xml(L"mappingmenu.xml");
			ShowMenu(&m_PaintManager, xml, pt);
		}
		else if (pList == m_pConnect_List)
		{
			STRINGorID xml(L"connectmenu.xml");
			ShowMenu(&m_PaintManager, xml, pt);
		}
	}
	return true;
}

void CMainDlg::Test()
{
	CMappingListItem* pItem = new CMappingListItem(nullptr);
	pItem->InitStringList(L"192.168.1.101", L"1234", L"192.168.1.101", L"3214");
	m_pMapping_List->Add(pItem);
}
bool CMainDlg::ListItemNotify(void* p)
{
	TNotifyUI* pNotify = (TNotifyUI*)p;
	if (!pNotify)
		return false;
	if (pNotify->wParam != -1 && pNotify->sType == DUI_MSGTYPE_ITEMCLICK)
	{
		m_pConnect_List->SetVisible(true);
		m_pBottom_hide->SetText(L"▼" );
	}
	return false;
}