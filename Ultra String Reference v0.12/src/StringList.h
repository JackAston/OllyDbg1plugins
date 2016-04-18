////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   StringList.h
//  Version     :   1.0
//  Creater     :   Luo Cong
//  Date        :   2006-7-31 10:56:01
//  Comment     :   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __STRING_LIST_H__
#define __STRING_LIST_H__

typedef struct tagSTRING_LIST_NODE
{
    char pszStr[MAX_PATH];
    tagSTRING_LIST_NODE *pNext;
} STRING_LIST_NODE;

class CStringList
{
private:
    STRING_LIST_NODE *m_pHeadNode;

public:
    CStringList();
    virtual ~CStringList();

public:
    BOOL AddHeadNode(
        const char cszStr[]
    );

    char * GetNodeStrAt(const int nIndex);

    int GetCount(void);

    BOOL RemoveHeadNode(void);

    void RemoveAllNodes(void);
};

#endif  // __STRING_LIST_H__