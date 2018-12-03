// MachinePidTidTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "LogViewer.h"
#include "MachinePidTidTreeView.h"


// CMachinePidTidTreeView

IMPLEMENT_DYNCREATE(CMachinePidTidTreeView, CTreeView)

CMachinePidTidTreeView::CMachinePidTidTreeView()
{
    m_bInited = FALSE;
}

CMachinePidTidTreeView::~CMachinePidTidTreeView()
{
}

BEGIN_MESSAGE_MAP(CMachinePidTidTreeView, CTreeView)
    ON_NOTIFY_REFLECT(NM_TVSTATEIMAGECHANGING, &CMachinePidTidTreeView::OnNMTVStateImageChanging)
END_MESSAGE_MAP()


// CMachinePidTidTreeView diagnostics

#ifdef _DEBUG
void CMachinePidTidTreeView::AssertValid() const
{
    CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CMachinePidTidTreeView::Dump(CDumpContext& dc) const
{
    CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CMachinePidTidTreeView::PreTranslateMessage(MSG* pMsg){
    //DUMP_WINDOWS_MSG(__FILE__LINE__, NULL, 0, pMsg->message, pMsg->wParam, pMsg->lParam);

    if( IsDialogMessage( pMsg ) )
    {
        return TRUE;
    }
    else
    {
        return __super::PreTranslateMessage( pMsg );
    }

}

// CMachinePidTidTreeView message handlers

void CMachinePidTidTreeView::OnInitialUpdate()
{
    BOOL bRet = FALSE;
    CTreeView::OnInitialUpdate();
    
    CTreeCtrl& treeCtrl = GetTreeCtrl();
    if (FALSE == m_bInited)
    {
        treeCtrl.ModifyStyle(0, TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT
            |TVS_DISABLEDRAGDROP|TVS_CHECKBOXES|TVS_SHOWSELALWAYS|TVS_TRACKSELECT
            |TVS_EX_PARTIALCHECKBOXES);
        //treeCtrl.SetExtendedStyle(treeCtrl.GetExtendedStyle() | TVS_EX_PARTIALCHECKBOXES);
        m_bInited = TRUE;
    }
    CLogViewerDoc* pDoc = GetDocument();

    _InitIdsTree(treeCtrl, pDoc->m_FTLogManager.GetAllMachinePidTidInfos());
    //AllThreadIdContainer allThreadIds;
    //pDoc->m_FTLogManager.GetThreadIds(allThreadIds);

    // TODO: Add your specialized code here and/or call the base class
}

void CMachinePidTidTreeView::_InitIdsTree(CTreeCtrl& treeCtrl, MachinePidTidContainer& allMachinePidTidInfos){
    treeCtrl.DeleteAllItems();

    BOOL fCheck = TRUE;
    TVINSERTSTRUCT tvInsertStruct = { 0 };
    tvInsertStruct.hParent = TVI_ROOT;
    tvInsertStruct.hInsertAfter = TVI_LAST;
    tvInsertStruct.itemex.mask = TVIF_TEXT|TVIF_STATE;
    tvInsertStruct.itemex.stateMask = TVIS_STATEIMAGEMASK;
    tvInsertStruct.itemex.state = INDEXTOSTATEIMAGEMASK((fCheck ? 2 : 1));


    for (MachinePidTidContainerIter iterMachine = allMachinePidTidInfos.begin();
        iterMachine != allMachinePidTidInfos.end();
        ++iterMachine){
            FTL::CFConversion conv;
            tvInsertStruct.hParent = TVI_ROOT;
            tvInsertStruct.itemex.pszText = conv.UTF8_TO_TCHAR(iterMachine->first.c_str());
            HTREEITEM hItemMachine = treeCtrl.InsertItem(&tvInsertStruct);

            PidTidContainer& pidTidContainer = iterMachine->second;
            for (PidTidContainer::iterator iterPid = pidTidContainer.begin(); 
                iterPid != pidTidContainer.end();
                ++iterPid)
            {
                tvInsertStruct.hParent = hItemMachine;
                tvInsertStruct.itemex.pszText = conv.UTF8_TO_TCHAR(iterPid->first.c_str());
                HTREEITEM hItemPid = treeCtrl.InsertItem(&tvInsertStruct);
                treeCtrl.Expand(hItemMachine,TVE_EXPAND);
                
                TidContainer& tidContainer = iterPid->second;

                for (TidContainerIter iterTid = tidContainer.begin();
                    iterTid != tidContainer.end();
                    ++iterTid)
                {
                    tvInsertStruct.hParent = hItemPid;
                    tvInsertStruct.itemex.pszText = conv.UTF8_TO_TCHAR(iterTid->first.c_str());

                    HTREEITEM hItemTid = treeCtrl.InsertItem(&tvInsertStruct);
                    treeCtrl.SetItemData(hItemTid, (DWORD_PTR)&(iterTid->second));
                    treeCtrl.Expand(hItemPid, TVE_EXPAND);
                }
            }
    }
}
void CMachinePidTidTreeView::OnNMTVStateImageChanging(NMHDR *pNMHDR, LRESULT *pResult)
{
    CTreeCtrl& treeCtrl = GetTreeCtrl();
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    TRACE("OnNMTVStateImageChanging, pNMTreeView: action=%d, itemOld.mask=0x%x, itemNew.mask=0x%x, ptDrag=(%d, %d)\n", 
        pNMTreeView->action, pNMTreeView->itemOld.mask, pNMTreeView->itemNew.mask, 
        pNMTreeView->ptDrag.x, pNMTreeView->ptDrag.y);
    
    CPoint pt;
    GetCursorPos(&pt);
    ScreenToClient(&pt);
    MapWindowPoints((CWnd*)&treeCtrl, &pt, 1);

    UINT flag = TVHT_ONITEM;
    HTREEITEM hCurrentItem = treeCtrl.HitTest(pt, &flag);
    if(hCurrentItem) {
        treeCtrl.SelectItem(hCurrentItem);
        if(flag&(TVHT_ONITEMSTATEICON)) {
            SetChildCheck(treeCtrl, hCurrentItem, !treeCtrl.GetCheck(hCurrentItem));
            SetParentCheck(treeCtrl, hCurrentItem, !treeCtrl.GetCheck(hCurrentItem));


            //pDoc->m_FTLogManager.SetThreadIdChecked(threadId,TRUE);
            CLogViewerDoc* pDoc = GetDocument();
            pDoc->m_FTLogManager.DoFilterLogItems();
            pDoc->UpdateAllViews(this);
        }
    }

    *pResult = 0;
}

//�ݹ����,���õ�ǰ�ڵ�������ӽڵ��״ֵ̬
void CMachinePidTidTreeView::SetChildCheck(CTreeCtrl& treeCtrl,  HTREEITEM hItem, BOOL bCheck )
{
    if (treeCtrl.ItemHasChildren(hItem))
    {
        HTREEITEM hChildItem = treeCtrl.GetChildItem(hItem);
        while(hChildItem) {
            treeCtrl.SetCheck(hChildItem, bCheck);
            SetChildCheck(treeCtrl, hChildItem, bCheck);
            hChildItem = treeCtrl.GetNextSiblingItem(hChildItem);
        }
    } else {
        //��ײ�� Tid �ڵ�
        ID_INFOS* pIdInfos = (ID_INFOS*)treeCtrl.GetItemData(hItem);
        pIdInfos->bChecked = bCheck;
    }

}

void CMachinePidTidTreeView::SetParentCheck(CTreeCtrl& treeCtrl, HTREEITEM hItem, BOOL bCheck )
{
    HTREEITEM hParent = treeCtrl.GetParentItem(hItem);
    if(hParent==NULL)
        return ;

    if(bCheck) {
        HTREEITEM hSlibing = treeCtrl.GetNextSiblingItem(hItem); //.GetNextItem(hItem, TVGN_NEXT);
        BOOL bAllCheck = TRUE;
        // ��ǰItem��ǰ���ֵܽڵ����Ƿ�ȫ��ѡ�У�
        while(hSlibing) {
            if(!treeCtrl.GetCheck(hSlibing)) {
                bAllCheck = FALSE; // ����ֵܽڵ�����һ��û��ѡ��
                break;
            }
            hSlibing = treeCtrl.GetNextSiblingItem(hSlibing); //GetNextItem(hSlibing, TVGN_NEXT);
        }

        if(bAllCheck) {
            hSlibing = treeCtrl.GetPrevSiblingItem(hItem); //GetNextItem(hItem, TVGN_PREVIOUS);
            while(hSlibing) {
                if(!treeCtrl.GetCheck(hSlibing)) {
                    bAllCheck = FALSE; // ǰ���ֵܽڵ�����һ��û��ѡ��
                    break;
                }
                hSlibing = treeCtrl.GetPrevSiblingItem(hSlibing); //GetNextItem(hSlibing, TVGN_PREVIOUS);
            }
        }

        // bFlagΪTRUE����ʾ��ǰ�ڵ������ǰ���ֵܽڵ㶼��ѡ�У���������丸�ڵ�ҲΪѡ��
        if(bAllCheck){
            treeCtrl.SetCheck(hParent, TRUE);
        }
    } else { // ��ǰ�ڵ���Ϊδѡ�У���Ȼ�丸�ڵ�ҲҪ����Ϊδѡ��
        treeCtrl.SetCheck(hParent, FALSE);
    }

    // �ݹ����
    SetParentCheck(treeCtrl, hParent, treeCtrl.GetCheck(hParent));
}
