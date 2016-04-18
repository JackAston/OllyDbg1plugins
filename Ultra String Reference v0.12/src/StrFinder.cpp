////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   StrFinder.cpp
//  Version     :   1.0
//  Creater     :   Luo Cong
//  Date        :   2006-7-28 10:31:48
//  Comment     :   
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StrFinder.h"
#include "Plugin.h"
#include "ustrref.h"

#ifdef _DEBUG
#   define DEBUG_NEW new (_NORMAL_BLOCK, THIS_FILE, __LINE__)
#   define new DEBUG_NEW
#   undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

CStrFinder::CStrFinder()
{
}

CStrFinder::~CStrFinder()
{
}

BOOL CStrFinder::IsPrintAble(const BYTE ch)
{
    BOOL bRet;

    if (ch >= 0x20 && ch < 0x7f)
        bRet = TRUE;
    else if (0xff == ch)
        bRet = FALSE;
    else if (0x80 == (ch & 0x80))
        bRet = TRUE;
    else
        bRet = FALSE;

    return bRet;
}

BOOL CStrFinder::IsAlpha(const BYTE ch)
{
    if (ch >= 0x20 && ch <= 0x7f)
        return TRUE;
    else
        return FALSE;
}

BOOL CStrFinder::IsSimplifiedCH(const USHORT dch)
{
    BYTE highbyte;
    BYTE lowbyte;

    highbyte = dch & 0x00ff;
    lowbyte  = dch / 0x0100;

    //    区名    码位范围   码位数  字符数 字符类型
    // 双字节2区 B0A1—F7FE   6768    6763    汉字
    if (
        (highbyte >= 0xb0 && highbyte <= 0xf7) &&
        (lowbyte  >= 0xa1 && lowbyte  <= 0xfe)
    )
        return TRUE;
    else
        return FALSE;
}

BOOL CStrFinder::IsGraphicCH(const USHORT dch)
{
    BOOL bRet;
    BYTE highbyte;
    BYTE lowbyte;

    highbyte = dch & 0x00ff;
    lowbyte  = dch / 0x0100;

    //    区名    码位范围   码位数  字符数 字符类型
    // 双字节1区 A1A1—A9FE    846     718  图形符号
    if (
        (highbyte >= 0xa1 && highbyte <= 0xa9) &&
        (lowbyte  >= 0xa1 && lowbyte  <= 0xfe)
    )
        bRet = TRUE;
    //    区名    码位范围   码位数  字符数 字符类型
    // 双字节5区 A840—A9A0    192     166  图形符号
    else if (
        (highbyte >= 0xa8 && highbyte <= 0xa9) &&
        (lowbyte  >= 0x40 && lowbyte  <= 0xa0)
    )
        bRet = TRUE;
    else
        bRet = FALSE;

    return bRet;
}

BOOL CStrFinder::GetStr(
    const STR_FINDER_STRING_TYPE StrType,
    const BYTE *pbyBuf,
    BYTE *pszStr,
    int *pnLen
)
{
    BOOL bFound = TRUE;

    int i = 0;
    int nLen = 0;

    while (i < TEXTLEN)
    {
        if (enumSFST_Ascii == StrType)
        {
            if ('\0' == pbyBuf[i])
                break;
        }
        else if (enumSFST_Unicode == StrType)
        {
            if (i + 1 >= TEXTLEN || '\0' == pbyBuf[i] && '\0' == pbyBuf[i + 1])
                break;
        }

        if ('\r' == pbyBuf[i])
        {
            *pszStr++ = '\\';
            *pszStr++ = 'r';
            nLen += 2;
        }
        else if ('\n' == pbyBuf[i])
        {
            *pszStr++ = '\\';
            *pszStr++ = 'n';
            nLen += 2;
        }
        else if ('\t' == pbyBuf[i])
        {
            *pszStr++ = '\\';
            *pszStr++ = 't';
            nLen += 2;
        }
        else if (IsAlpha(pbyBuf[i]))
        {
            *pszStr++ = pbyBuf[i];
            ++nLen;
        }
        else if (IsGraphicCH(
            *(USHORT *)&pbyBuf[i]) ||
            IsSimplifiedCH(*(USHORT *)&pbyBuf[i])
        )
        {
            *pszStr++ = pbyBuf[i];
            ++i;
            *pszStr++ = pbyBuf[i];
            nLen += 2;
        }
        else if ('\0' == pbyBuf[i])
        {
            if (enumSFST_Unicode == StrType)
            {
                // Skip '\0'
            }
            else
            {
                bFound = FALSE;
                break;
            }
        }
        else
        {
            bFound = FALSE;
            break;
        }
        ++i;
    }

    if (0 == nLen)
        bFound = FALSE;

    *pszStr = '\0';

    if (bFound)
    {
        if (i >= TEXTLEN)
        {
            strcpy((char *)&pszStr[TEXTLEN - 4], " ...");
            nLen = TEXTLEN; // cut length to TEXTLEN
        }
    }

    if (pnLen)
        *pnLen = nLen;

    return bFound;
}

BOOL CStrFinder::FollowImmediateAddr(
    DWORD dwImmAddr,
    BYTE *pszStr,
    BOOL *pbFound
)
{
    BOOL bRetResult = FALSE;
    int nRetCode;

    t_memory *pMem;
    BYTE szBuf[TEXTLEN];
    int nStrLen;

    ASSERT(pbFound);
    *pbFound = FALSE;

    pMem = Findmemory(dwImmAddr);
    PROCESS_ERROR(pMem && pMem->sect[0]);

    nRetCode = Readmemory(
        &dwImmAddr, dwImmAddr, sizeof(dwImmAddr), MM_RESTORE | MM_SILENT
    );
    PROCESS_ERROR(nRetCode);

    pMem = Findmemory(dwImmAddr);
    PROCESS_ERROR(pMem && pMem->sect[0]);

    nRetCode = Readmemory(
        szBuf, dwImmAddr, sizeof(szBuf), MM_RESTORE | MM_SILENT
    );
    PROCESS_ERROR(nRetCode);

    *pbFound = GetStr(enumSFST_Ascii, szBuf, pszStr, &nStrLen);
    if (FALSE == *pbFound)
        *pbFound = GetStr(enumSFST_Unicode, szBuf, pszStr, &nStrLen);

    bRetResult = TRUE;
Exit0:
    return bRetResult;
}

BOOL CStrFinder::Find(
    const STR_FINDER_STRING_TYPE StrType,
    const DWORD dwBase,
    const DWORD dwSize,
    STR_FINDER_CALLBACK *pCallBack
)
{
    BOOL bRetResult = FALSE;
    int nRetCode;

    int i;
    DWORD dwOffset;
    DWORD dwAddr;
    BYTE szCmd[MAXCMDSIZE];
    BYTE szBuf[TEXTLEN];
    DWORD dwCmdSize;
    t_disasm da;
    t_memory *pMem;
    int nStrIndex = 0;
    BYTE pszStr[TEXTLEN * 2];
    int nStrLen;
    DWORD dwImmAddr;
    BOOL bFound;

    ASSERT(pCallBack);

    for (dwOffset = 0; dwOffset < dwSize; dwOffset += dwCmdSize)
    {
        dwAddr = dwBase + dwOffset;

        nRetCode = Readcommand(dwAddr, (char *)szCmd);
        PROCESS_ERROR(nRetCode);

        dwCmdSize = Disasm(
            szCmd, MAXCMDSIZE, dwAddr, NULL, &da, DISASM_CODE, 0
        );

        if (g_dwCurEip == dwAddr)
        {
            pCallBack(nStrIndex++, dwBase, dwOffset, dwSize, StrType, defINITCPUSEL);
            continue;
        }

        if (
            (0 != memicmp(da.result, "push", 4)) &&
            (0 != memicmp(da.result, "mov", 3)) &&
            (0 != memicmp(da.result, "lea", 3))
        )
            continue;

        pMem = Findmemory(da.immconst);
        if ((NULL == pMem) || ('\0' == pMem->sect[0]))
            continue;

        for (
            dwImmAddr = da.immconst, bFound = TRUE, i = 0;
            bFound && i < 8;    // loop 8 times at most!
            dwImmAddr += sizeof(DWORD), ++i
        )
        {
            nRetCode = FollowImmediateAddr(dwImmAddr, pszStr, &bFound);
            if (!nRetCode)
                break;

            if (bFound)
                pCallBack(nStrIndex++, dwBase, dwOffset, dwSize, StrType, (char *)pszStr);
        }

        nRetCode = Readmemory(
            szBuf, da.immconst, sizeof(szBuf), MM_RESTORE | MM_SILENT
        );
        PROCESS_ERROR(nRetCode);

        bFound = GetStr(StrType, szBuf, pszStr, &nStrLen);
        if (bFound)
            pCallBack(nStrIndex++, dwBase, dwOffset, dwSize, StrType, (char *)pszStr);
    }

    bRetResult = TRUE;
Exit0:
    return bRetResult;
}