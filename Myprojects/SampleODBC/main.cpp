#include <iostream>
#include <Windows.h>
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
SQLTCHAR g_userid[30] = { 0, };
SQLTCHAR g_userps[30] = { 0, };
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
		if (ret = SQLGetData(stmt, m_ColumnList[column - 1].icol, SQL_WCHAR, m_ColumnList[column - 1].bindData,
							sizeof(m_ColumnList[column - 1].bindData), NULL) == SQL_SUCCESS)
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
		} while (ret == SQL_SUCCESS);
	}
	return ret;
}

bool GetRecordData(SQLHSTMT hSmt)
{
	GetDescribeCol(hSmt);

	SQLRETURN ret;
	while (1)
	{
		ret = SQLFetch(hSmt);
		if (ret == SQL_NO_DATA) break;
		if (ret != SQL_SUCCESS)
		{
			if (GetDiagRec(hSmt) == false)
			{
				SQLCloseCursor(hSmt);
				return false;
			}
		}

		RECORD record;
		for (int iCol = 0; iCol < m_ColumnList.size(); iCol++)
		{
			std::wstring retString;
			ret = GetColData(hSmt, m_ColumnList[iCol].icol, retString);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			{
				record.push_back(retString);
			}
		}
		m_RecordResultList.push_back(record);
	}
	while (ret = SQLMoreResults(hSmt) != SQL_NO_DATA);
	// ???
	SQLCloseCursor(hSmt);
	SQLFreeStmt(hSmt, SQL_CLOSE);
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
	SQLTCHAR szUerID[30] = { 0, };
	SQLTCHAR szUerPS[30] = { 0, };
	SQLINTEGER iID;
	SQLLEN IID, IUserID, iUserPS;

	SQLBindCol(g_hStmt, 1, SQL_INTEGER, &iID, 0, &IID);
	SQLBindCol(g_hStmt, 2, SQL_INTEGER, (SQLPOINTER)szUerID, sizeof(szUerID), &IUserID);
	SQLBindCol(g_hStmt, 2, SQL_INTEGER, (SQLPOINTER)szUerPS, sizeof(szUerPS), &iUserPS);
	TCHAR sql[] = L"select * from DemoGame";
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS);
	SQLLEN iNumRow;
	SQLSMALLINT iNumCOl;
	SQLRowCount(g_hStmt, &iNumRow);
	SQLNumResultCols(g_hStmt, &iNumCOl);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << "g_iID" << iID << " " << szUerID << " " << szUerPS << std::endl;
	}
	SQLCloseCursor(g_hStmt);
}

void InsertQuery()
{
	TCHAR sql[] = L"insert into DemoGame (g_szUserID, g_szUserPS) VALUES ('5555' , '1111' )";
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(g_hStmt, &iNumRow);
	SQLNumResultCols(g_hStmt, &iNumCol);
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
	TCHAR sql[] = L"update DemoGame set g_szUer='3333' wher g_szUserID='코로나'";
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(g_hStmt, &iNumRow);
	SQLNumResultCols(g_hStmt, &iNumCol);
	SQLCloseCursor(g_hStmt);
}
void main()
{
	setlocale(LC_ALL, "KOREAN");

	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv);
	ret = SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc);

	TCHAR InCon[256] = {0,};
	TCHAR Dir[MAX_PATH];
	SQLSMALLINT cbCon;
	GetCurrentDirectory(MAX_PATH, Dir);
	wsprintf(InCon, _T("DRIVER=[Microsoft Access Driver (*.mdb *.accdb)};DBQ=%s\\%s;"), Dir, L"DemoGame.accdb");
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);
	if (ret != SQL_SUCCESS)
	{
		Check(g_hStmt);
		return;
	}

	CreatePrepare();

	_tcscpy_s(g_szInputUserID, L"kgca");
	ExecPrepare(g_hUserIDStmt);
	std::wcout << "id=" << g_iID << "userid = " << g_userid << "PS=" << g_userps << std::endl;
	{
		_tcscpy_s(g_szInputUserID, L"kgca");
		ret = SQLExecute(g_hUserIDStmt);
		while (1)
		{
			ret = SQLFetch(g_hUserIDStmt);
			GetDiagRec(g_hUserIDStmt);
			if (ret == SQL_NO_DATA)
			{
				break;
			}
			std::wcout << "id=" << g_iID << "userid = " << g_userid << "PS=" << g_userps << std::endl;
		}
		while (ret = SQLMoreResults(g_hUserIDStmt) != SQL_NO_DATA);
		//?
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