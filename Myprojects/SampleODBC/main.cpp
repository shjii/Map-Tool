#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <vector>
#pragma comment(lib, "odbc32.lib")

SQLHENV g_hEnv = SQL_NULL_HENV;
SQLHENV g_hDbc = SQL_NULL_HDBC;
SQLHENV g_hStmt = SQL_NULL_HSTMT;

TCHAR g_szInputUserID[32] = { 0, };
SQLHENV g_hUserIDStmt = SQL_NULL_HSTMT;
SQLTCHAR g_userid[32] = { 0, };
SQLTCHAR g_userps[32] = { 0, };
SQLINTEGER g_iID;
SQLLEN lID, IUserID, IUSerPS;
SQLRETURN ret;

int iNameLength = 0;
SQLLEN cbColumn = SQL_NTS;
typedef std::vector<std::wstring> RECORD;

struct ColDescription
{
	SQLUSMALLINT icol;
	SQLWCHAR szColName[80];
	SQLSMALLINT cchColNameMax;
	SQLSMALLINT pcchColName;
	SQLSMALLINT pfSqlType;
	SQLULEN pcbColDef;
	SQLSMALLINT pibScale;
	SQLSMALLINT pfNullable;
	SQLWCHAR bindData[80];
	SQLLEN byteSize;
};

std::vector<ColDescription> m_ColumnList;
RECORD m_ColumnResultList;
std::vector<RECORD> m_RecordResultList;
void Check(SQLHSTMT stmt)
{
	if (ret == SQL_SUCCESS) return;
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH +1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(g_hEnv, g_hDbc, stmt, szSQLState, &iSQLCode, errorBuffer,sizeof(errorBuffer), &length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}

bool GetDiagRec(SQLHSTMT stmt)
{
	SQLRETURN ret;
	SQLSMALLINT iCurrentRecord = 1, MsgLen;
	SQLTCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;
	SQLLEN numRecs = 0;

	SQLGetDiagField(SQL_HANDLE_STMT, stmt, 0, SQL_DIAG_NUMBER, &numRecs, 0, 0);

	while (iCurrentRecord <= numRecs)
	{
		if (ret = SQLGetDiagRec(SQL_HANDLE_STMT, stmt, iCurrentRecord, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
		{
			if (_tcscmp(SqlState, L"01004") == 0)
			{
				return true;
			}
			MessageBox(NULL, Msg, SqlState, MB_OK);
			iCurrentRecord++;
		}
	}
	return false;
}

void CreatePrepare()
{
	SQLRETURN ret;
	TCHAR sql[] = L"select * from DemoGame where userid=?";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hUserIDStmt);
	if (ret != SQL_SUCCESS)
	{
		return;
	}
	if (SQLPrepare(g_hUserIDStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}
	iNameLength = sizeof(g_szInputUserID);
	ZeroMemory(g_szInputUserID, iNameLength);
	cbColumn = SQL_NTS;

	ret = SQLBindParameter(g_hUserIDStmt, 1, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR, iNameLength, 0, g_szInputUserID
							, iNameLength, &cbColumn);
	if (ret != SQL_SUCCESS)
	{
		Check(g_hUserIDStmt);
		return;
	}

	ret = SQLBindCol(g_hUserIDStmt, 1, SQL_INTEGER, &g_iID, 0, &lID);
	ret = SQLBindCol(g_hUserIDStmt, 2, SQL_INTEGER, (SQLPOINTER)g_userid, sizeof(g_userid), &IUserID);
	ret = SQLBindCol(g_hUserIDStmt, 2, SQL_INTEGER, (SQLPOINTER)g_userps, sizeof(g_userps), &IUSerPS);
}

SQLRETURN GetDescribeCol(SQLHSTMT stmt)
{
	SQLLEN count = 0;
	SQLRETURN ret = SQLRowCount(stmt, &count);
	if (ret != SQL_SUCCESS)
	{
		Check(stmt);
	}
	SQLSMALLINT colCount;
	SQLNumResultCols(stmt, &colCount);
	m_ColumnList.clear();
	m_ColumnResultList.clear();
	m_RecordResultList.clear();

	ColDescription col;
	col.icol = 1;
	while (1)
	{
		ret = SQLDescribeCol(stmt, col.icol, col.szColName, sizeof(col.szColName),
							&col.pcchColName, &col.pfSqlType, &col.pcbColDef, &col.pibScale, &col.pfNullable);
		if (ret != SQL_SUCCESS)
		{
			break;
		}
		m_ColumnList.push_back(col);
		col.icol++;
	}
	return ret;
}

SQLRETURN GetColData(SQLHSTMT stmt, int column, std::wstring& rt)
{
	SQLRETURN ret;
	if (m_ColumnList[column - 1].pfSqlType != SQL_VARBINARY)
	{
		if (ret = SQLGetData(stmt,
			m_ColumnList[column - 1].icol,
			SQL_WCHAR,
			m_ColumnList[column - 1].bindData,
			sizeof(m_ColumnList[column - 1].bindData),
			NULL) == SQL_SUCCESS)
		{
			rt = m_ColumnList[column - 1].bindData;
		}
	}
	else
	{
		TCHAR buf[1000] = { 0, };
		SQLLEN cb;
		do {
			ZeroMemory(buf, sizeof(TCHAR) * 1000);
			ret = SQLGetData(stmt, m_ColumnList[column - 1].icol, SQL_WCHAR, buf, sizeof(buf), &cb);
			if (ret == SQL_NULL_DATA) break;
			rt += buf;
		} while (ret == SQL_SUCCESS_WITH_INFO);

	}
	return ret;
}

bool GetRecordData(SQLHSTMT	hStmt)
{
	GetDescribeCol(hStmt);
	// SQL데이터 형에 따른 바인딩으로 처리 할 수 있다.
	SQLRETURN ret;
	while (1)
	{
		ret = SQLFetch(hStmt);
		if (ret == SQL_NO_DATA) break;
		if (ret != SQL_SUCCESS)
		{
			if (GetDiagRec(hStmt) == false)
			{
				SQLCloseCursor(hStmt);
				return false;
			}
		}

		RECORD record;
		for (int iCol = 0; iCol < m_ColumnList.size(); iCol++)
		{
			std::wstring retString;
			ret = GetColData(hStmt, m_ColumnList[iCol].icol, retString);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			{
				record.push_back(retString);
			}
		}
		m_RecordResultList.push_back(record);
	}
	while (ret = SQLMoreResults(hStmt) != SQL_NO_DATA);

	SQLCloseCursor(hStmt);
	SQLFreeStmt(hStmt, SQL_CLOSE);
	return true;
}
void ExecPrepare(SQLHSTMT stmt)
{
	_tcscpy_s(g_szInputUserID,L"kgca");
	ret = SQLExecute(stmt);
	if (ret != SQL_SUCCESS)
	{
		Check(stmt);
		return;
	}
	if (GetRecordData(stmt) == false)
	{
		Check(stmt);
		return;
	}
}

void SelectQuery()
{
	SQLTCHAR  szUserID[30] = { 0, };
	SQLTCHAR  szUserPS[30] = { 0, };
	SQLINTEGER iID;
	SQLLEN  lID, lUserID, lUserPS;
	SQLBindCol(g_hStmt, 1, SQL_INTEGER, &iID, 0, &lID);
	SQLBindCol(g_hStmt, 2, SQL_UNICODE, (SQLPOINTER)szUserID, sizeof(szUserID), &lUserID);
	SQLBindCol(g_hStmt, 3, SQL_UNICODE, (SQLPOINTER)szUserPS, sizeof(szUserPS), &lUserPS);
	TCHAR sql[] = L"select * from DemoGame";
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS);
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(g_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(g_hStmt, &iNumCol); // 반환 컬럼 수

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << "g_iID=" << iID << " " << szUserID << " " << szUserPS << std::endl;
	}
	SQLCloseCursor(g_hStmt);
}

void InsertQuery()
{
	TCHAR sql[] =
		L"insert into DemoGame (userid, userps) VALUES ('5555','1111')";
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(g_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(g_hStmt, &iNumCol); // 반환 컬럼 수
	SQLCloseCursor(g_hStmt);
}

void DeleteQuery()
{
	TCHAR sql[] = L"delete from DemoGame where g_szUserID = '홍길동'";
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCOl;
	SQLRowCount(g_hStmt, &iNumRow);
	SQLNumResultCols(g_hStmt, &iNumCOl);
	SQLCloseCursor(g_hStmt);
}
void UpdateQuerty()
{
	TCHAR sql[] =
		L"update DemoGame set g_szUserPS='3333' where g_szUserID='코로나'";
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(g_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(g_hStmt, &iNumCol); // 반환 컬럼 수
	SQLCloseCursor(g_hStmt);
}
void main()
{
	setlocale(LC_ALL, "KOREAN");

	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv);
	ret = SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3_80, SQL_IS_INTEGER);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc);

	TCHAR InCon[256] = {0,};
	TCHAR Dir[MAX_PATH];
	SQLSMALLINT cbCon;
	GetCurrentDirectory(MAX_PATH, Dir);
	wsprintf(InCon, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s;"), Dir, L"DemoGame.accdb");
	ret = SQLDriverConnect(g_hDbc, NULL, InCon, _countof(InCon), NULL, 0,
		&cbCon, SQL_DRIVER_NOPROMPT);
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);
	if (ret != SQL_SUCCESS)
	{
		Check(g_hStmt);
		return;
	}

	CreatePrepare();

	_tcscpy_s(g_szInputUserID, L"kgca");
	ExecPrepare(g_hUserIDStmt);
	std::wcout << "id=" << g_iID << " userid=" << g_userid << " userps=" << g_userps << std::endl;


	{
		_tcscpy_s(g_szInputUserID, L"코로나");
		ret = SQLExecute(g_hUserIDStmt);
		while (1)
		{
			ret = SQLFetch(g_hUserIDStmt);
			GetDiagRec(g_hUserIDStmt);
			if (ret == SQL_NO_DATA)
			{
				break;
			}
			std::wcout << "id=" << g_iID << " userid=" << g_userid << " userps=" << g_userps << std::endl;
		}
		while (ret = SQLMoreResults(g_hUserIDStmt) != SQL_NO_DATA);
		SQLCloseCursor(g_hUserIDStmt);
		SQLFreeStmt(g_hUserIDStmt, SQL_CLOSE);
	}
	SelectQuery();
	InsertQuery();
	DeleteQuery();
	UpdateQuerty();

	SQLFreeHandle(SQL_HANDLE_STMT, g_hUserIDStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);

}