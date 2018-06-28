#pragma once

#include <deque>
#include <ftlbase.h>
//#include <ftlThread.h>
#include <list>
#include <ftlSharePtr.h>

#define MIN_TIME_WITH_DAY_INFO      ((LONGLONG)24 * 3600 * 1000 * 10000) 
//time ���ڵĵ�λ�� FILETIME(100ns)
#define TIME_RESULT_TO_MILLISECOND  (10 * 1000)

typedef std::string THREAD_ID_TYPE;
typedef std::string PROCESS_ID_TYPE;

struct PIdTIdType{
	PIdTIdType(PROCESS_ID_TYPE pid, THREAD_ID_TYPE tid){
		this->pid = pid;
		this->tid = tid;
	}
	PROCESS_ID_TYPE pid;
	THREAD_ID_TYPE  tid;
	bool operator < (const PIdTIdType & other) const{
		//COMPARE_MEM_LESS(pid, other);
		//COMPARE_MEM_LESS(tid, other);
        int pidCompare = pid.compare(other.pid);
        if( pidCompare < 0){
            return true;
        }else if(pidCompare > 0){
            return false;
        }

		int tidCompare = tid.compare(other.tid);
		if( tidCompare < 0){
			return true;
		}else if(tidCompare > 0){
			return false;
		}
		return false;
	}
};

typedef std::list<THREAD_ID_TYPE> AllThreadIdContainer;
typedef AllThreadIdContainer::iterator AllThreadIdContainerIter;

typedef std::list<PROCESS_ID_TYPE> AllProcessIdContainer;
typedef AllProcessIdContainer::iterator AllProcessIdContainerIter;


struct LogItem
{
    LONG                size;               //LogItem�Ĵ�С sizeof
    LONG                seqNum;             //���к�
    LONG                moduleNameLen;      //ģ�����ֵĳ���
    LONG                traceInfoLen;       //pszTraceInfo �ĳ��ȣ�Ŀǰ������ pszTraceInfo �ַ�������+1(������β��NULL,���˷ѿռ�)
    LONG                srcFileline;        //��Դ�ļ��е��к�
    LONGLONG            time;               //FILETIME ��Ӧ��ֵ(100ns��
    LONGLONG            elapseTime;
    PROCESS_ID_TYPE     processId;
    THREAD_ID_TYPE		threadId;
    FTL::TraceLevel     level;
    //HMODULE           module;
	LPCWSTR				pszFunName;
    LPCWSTR             pszModuleName;
    LPCWSTR             pszTraceInfo;       //�����ַ�����ָ��ʼ����WCHAR������
    LPCTSTR             pszSrcFileName;     //Դ�ļ���·��
    LPCWSTR             pszFullLog;         //������־

    LogItem()
    {
        size = 0;
        seqNum = 0;
        moduleNameLen = 0;
        traceInfoLen = 0;
        srcFileline = 0;
        time = 0;
        elapseTime = 0;
        processId = "0";
        threadId = "0";
        level = FTL::tlEnd;
		pszFunName = NULL;
        pszModuleName = NULL;
        pszTraceInfo = NULL;
        pszSrcFileName = NULL;
        pszFullLog = NULL;
    }
    ~LogItem()
    {
		SAFE_DELETE_ARRAY(pszFunName);
        SAFE_DELETE_ARRAY(pszModuleName);
        SAFE_DELETE_ARRAY(pszTraceInfo);
        SAFE_DELETE_ARRAY(pszSrcFileName);
        SAFE_DELETE_ARRAY(pszFullLog);
    }
    DISABLE_COPY_AND_ASSIGNMENT(LogItem);
};

typedef CFSharePtr<LogItem>             LogItemPointer;
typedef std::deque< LogItemPointer >    LogItemsContainer;
typedef LogItemsContainer::iterator     LogItemsContainerIter;