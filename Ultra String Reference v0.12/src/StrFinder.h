////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   StrFinder.h
//  Version     :   1.0
//  Creater     :   Luo Cong
//  Date        :   2006-7-28 10:31:26
//  Comment     :   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __STR_FINDER_H__
#define __STR_FINDER_H__

typedef enum tagSTR_FINDER_STRING_TYPE
{
    enumSFST_Ascii  = 0,
    enumSFST_Unicode,
} STR_FINDER_STRING_TYPE;

static const char *g_szStrFinderStrType[] =
{
    "ASCII",
    "UNICODE",
};

typedef void STR_FINDER_CALLBACK(
    const int nStrIndex,
    const DWORD dwBase,
    const DWORD dwOffset,
    const DWORD dwSize,
    const STR_FINDER_STRING_TYPE StrType,
    const char cszStr[]
);

class CStrFinder
{
private:
    BOOL IsPrintAble(const BYTE ch);
    BOOL IsAlpha(const BYTE ch);
    BOOL IsSimplifiedCH(const USHORT dch);
    BOOL IsGraphicCH(const USHORT dch);
    BOOL GetStr(
        const STR_FINDER_STRING_TYPE StrType,
        const BYTE *pbyBuf,
        BYTE *pszStr,
        int *pnLen
    );
    BOOL FollowImmediateAddr(
        DWORD dwImmAddr,
        BYTE *pszStr,
        BOOL *pbFound
    );

public:
    CStrFinder();
    virtual ~CStrFinder();

    BOOL Find(
        const STR_FINDER_STRING_TYPE StrType,
        const DWORD dwBase,
        const DWORD dwSize,
        STR_FINDER_CALLBACK *pCallBack
    );
};

#endif  // __STR_FINDER_H__