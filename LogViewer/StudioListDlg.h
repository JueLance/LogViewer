#pragma once
#include "afxwin.h"


// CStudioListDlg �Ի���
#include "VsIdeHandler.h"

class CStudioListDlg : public CDialog
{
	DECLARE_DYNAMIC(CStudioListDlg)

public:
	CStudioListDlg(CVsIdeHandler& vsIdeHandler,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStudioListDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_STUDIO_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    IUnknown*  m_SelectedIDE;
    CListBox m_StudiosList;
    CVsIdeHandler& m_VsIdeHandler;
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    afx_msg void OnLbnSelchangeListStudios();
};
