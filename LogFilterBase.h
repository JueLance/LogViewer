#ifndef LOG_FILTER_BASE_H
#define LOG_FILTER_BASE_H
#pragma once

#include <Windows.h>


#ifdef LOG_FILTER_EXPORTS
#define LOG_FILTER_API __declspec (dllexport)
#else
#define LOG_FILTER_API __declspec (dllimport)
#endif

#include "LogViewerDefine.h"

#include <ftlbase.h>

class LOG_FILTER_API ILogFilterBase
{
public:
    //���ع����ַ��� "LogViewer Files (*.FTL)|*.FTL|"
    virtual BOOL GetFilterString(LPWSTR pszBuf, LONG* pLen) = 0;

    //�����ļ���·����Ϣ(����չ��)���жϵ�ǰFilter�Ƿ��ܴ�������ֵԽ��˵��Խ�ܴ�������0˵�����ܴ���
    virtual DWORD GetHandleScore(LPCWSTR pszLogPath) = 0;

    //
    virtual BOOL ReadLog(LPCWSTR pszLogPath, LogItemsContainer& logItems) = 0;
    virtual BOOL WriteLog(LPCWSTR pszLogPath, const LogItemsContainer& logItems) = 0;

    virtual LONG Release() = 0;
protected:
    virtual ~ILogFilterBase() {};
};

typedef ILogFilterBase* (*GetLogFilterFunc)();

struct LogFilterRelease : public std::unary_function<ILogFilterBase*, void>
{
    void operator()(ILogFilterBase* ptr) const
    {
        ptr->Release();
    }
};

#endif //LOG_FILTER_BASE_H