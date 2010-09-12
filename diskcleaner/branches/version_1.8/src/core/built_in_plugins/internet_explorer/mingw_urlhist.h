/*
** Copyright (C) 2009 Robert Moerland
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation version 2.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef MINGW_URLHIST_H
#define MINGW_URLHIST_H


//This file is a partial fixup for the missing SDK header file urlhist.h
//It is in no way complete, but just suffices for what disk cleaner uses.

#include <rpc.h>
#include <rpcndr.h>
#include <ole2.h>

#if defined(__cplusplus)

const GUID CLSID_CUrlHistory =  {0x3C374A40L, 0xBAE4, 0x11CF, {0xBF, 0x7D, 0x00, 0xAA, 0x00, 0x69, 0x46, 0xEE}};

const IID IID_IUrlHistoryStg2 = {0xAFA0DC11L, 0xC313, 0x11d0, {0x83, 0x1A, 0x00, 0xC0, 0x4F, 0xD5, 0xAE, 0x38}};

struct __declspec(uuid("B722BCCB-4E68-101B-A2BC-00AA00404770")) IOleCommandTarget;

struct IEnumSTATURL;

typedef struct _STATURL
{
    DWORD cbSize;
    LPWSTR pwcsUrl;
    LPWSTR pwcsTitle;
    FILETIME ftLastVisited;
    FILETIME ftLastUpdated;
    FILETIME ftExpires;
    DWORD dwFlags;
} 	STATURL;

typedef struct _STATURL *LPSTATURL;
MIDL_INTERFACE("3C374A41-BAE4-11CF-BF7D-00AA006946EE")
IUrlHistoryStg :
public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE AddUrl(
        LPCOLESTR pocsUrl,
        LPCOLESTR pocsTitle,
        DWORD dwFlags) = 0;

    virtual HRESULT STDMETHODCALLTYPE DeleteUrl(
        LPCOLESTR pocsUrl,
        DWORD dwFlags) = 0;

    virtual HRESULT STDMETHODCALLTYPE QueryUrl(
        LPCOLESTR pocsUrl,
        DWORD dwFlags,
        LPSTATURL lpSTATURL) = 0;

    virtual HRESULT STDMETHODCALLTYPE BindToObject(
        LPCOLESTR pocsUrl,
        REFIID riid,
        void **ppvOut) = 0;

    virtual HRESULT STDMETHODCALLTYPE EnumUrls(
        IEnumSTATURL **ppEnum) = 0;

};

MIDL_INTERFACE("3C374A42-BAE4-11CF-BF7D-00AA006946EE")
IEnumSTATURL :
public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Next(
        ULONG celt,
        LPSTATURL rgelt,
        ULONG *pceltFetched) = 0;

    virtual HRESULT STDMETHODCALLTYPE Skip(
        ULONG celt) = 0;

    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;

    virtual HRESULT STDMETHODCALLTYPE Clone(
        IEnumSTATURL **ppenum) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetFilter(
        LPCOLESTR poszFilter,
        DWORD dwFlags) = 0;

};

MIDL_INTERFACE("AFA0DC11-C313-11d0-831A-00C04FD5AE38")
IUrlHistoryStg2 :
public IUrlHistoryStg
{
public:
    virtual HRESULT STDMETHODCALLTYPE AddUrlAndNotify(
        LPCOLESTR pocsUrl,
        LPCOLESTR pocsTitle,
        DWORD dwFlags,
        BOOL fWriteHistory,
        IOleCommandTarget *poctNotify,
        IUnknown *punkISFolder) = 0;

    virtual HRESULT STDMETHODCALLTYPE ClearHistory( void) = 0;

};

//Missing in MinGW's wininet.h
#ifdef __GNUC__
extern "C"
{
    HANDLE WINAPI FindFirstUrlCacheEntryExW(
        LPCTSTR lpszUrlSearchPattern,
        DWORD dwFlags,
        DWORD dwFilter,
        GROUPID GroupId,
        LPINTERNET_CACHE_ENTRY_INFO lpFirstCacheEntryInfo,
        LPDWORD lpdwEntryInfo,
        LPVOID lpGroupAttributes,
        LPDWORD lpcbGroupAttributes,
        LPVOID lpReserved
    );

    BOOL WINAPI FindNextUrlCacheEntryExW(
        HANDLE hEnumHandle,
        LPINTERNET_CACHE_ENTRY_INFO lpNextCacheEntryInfo,
        LPDWORD lpcbEntryInfo,
        LPVOID lpGroupAttributes,
        LPDWORD lpcbGroupAttributes,
        LPVOID lpReserved
    );
}
#define FindFirstUrlCacheEntryEx  FindFirstUrlCacheEntryExW
#define FindNextUrlCacheEntryEx  FindNextUrlCacheEntryExW
#endif //__GNUC__

#else //__cplusplus

#error "C not supported"

#endif //__cplusplus

#endif //MINGW_URLHIST_H


