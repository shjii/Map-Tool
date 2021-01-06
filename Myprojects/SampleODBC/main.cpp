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
SQLLEN IID, IUserID, IUSerPS;
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

bool GetDiagRec()
{
	SQLRETURN ret;
	SQLSMALLINT iCurrentRecord = 1, MsgLen;
	SQLTCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;
	SQLLEN numRecs = 0;

	SQLGetDiagField(SQL_HANDLE_STMT, g_hUserIDStmt, 0, SQL_DIAG_NUMBER, &numRecs, 0, 0);

	while (iCurrentRecord <= numRecs)
	{
		if (ret = SQLGetDiagRec(SQL_HANDLE_STMT, g_hUserIDStmt, iCurrentRecord, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
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

	ret = SQLBindCol(g_hUserIDStmt, 1, SQL_INTEGER, &g_iID, 0, &IID);
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

void main()
{
	setlocale(LC_ALL, "KOREAN");

	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv);
}