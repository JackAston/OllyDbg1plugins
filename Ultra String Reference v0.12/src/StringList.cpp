////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   StringList.cpp
//  Version     :   1.0
//  Creater     :   Luo Cong
//  Date        :   2006-7-31 10:56:51
//  Comment     :   
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StringList.h"

#ifdef _DEBUG
#   define DEBUG_NEW new (_NORMAL_BLOCK, THIS_FILE, __LINE__)
#   define new DEBUG_NEW
#   undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

CStringList::CStringList() : m_pHeadNode(NULL)
{
}

CStringList::~CStringList()
{
    RemoveAllNodes();
}

BOOL CStringList::AddHeadNode(
    const char cszStr[]
)
{
    BOOL bRetResult = FALSE;

    STRING_LIST_NODE *pNew;

    pNew = new STRING_LIST_NODE;
    PROCESS_ERROR(pNew);

    strncpy(pNew->pszStr, cszStr, MAX_PATH);
    pNew->pNext = m_pHeadNode;

    m_pHeadNode = pNew;

    bRetResult = TRUE;
Exit0:
    return bRetResult;
}

char * CStringList::GetNodeStrAt(const int nIndex)
{
    int i;
    int nPos = (GetCount() - 1) - nIndex;
    STRING_LIST_NODE *pNode;

    for (pNode = m_pHeadNode, i = 0; pNode; pNode = pNode->pNext, ++i)
    {
        if (i == nPos)
            return pNode->pszStr;
    }

    return NULL;
}

int CStringList::GetCount(void)
{
    int nCount = 0;
    STRING_LIST_NODE *pNode;

    for (pNode = m_pHeadNode; pNode; pNode = pNode->pNext)
    {
        ++nCount;
    }

    return nCount;
}

BOOL CStringList::RemoveHeadNode(void)
{
    STRING_LIST_NODE *pHead;

    pHead = m_pHeadNode;
    if (pHead)
    {
        m_pHeadNode = pHead->pNext;
        delete pHead;
        pHead = NULL;
    }

    return TRUE;
}

void CStringList::RemoveAllNodes(void)
{
    STRING_LIST_NODE *pNode;

    while (m_pHeadNode)
    {
        pNode = m_pHeadNode;
        m_pHeadNode = pNode->pNext;

        delete pNode;
        pNode = NULL;
    }
}