#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <conio.h>

typedef struct TNodeData
{
	char m_szName[20];
	int m_iAge;
	char m_strAddress[50];
	struct TNodeData* pNext;
}TNode;
char* GetName(TNode* pNode, int ilndex) { return pNode[ilndex].m_szName; }
int GetAge(TNode* pNode, int ilndex) { return pNode[ilndex].m_iAge; }
char* GetAddress(TNode* pNode, int ilndex) { return pNode[ilndex].m_strAddress; }

int main()
{

	FILE *fp_src;
	FILE *fp_dest;
	char buff[4096];
	size_t n_size;

	fp_src = fopen("sample.cpp", "r");
	fp_dest = fopen("sample.bak", "w");


	while (0 < (n_size = fread(buff, 1, 4096, fp_src)))
	{
		fwrite(buff, 1, n_size, fp_src);
	}

	fclose(fp_src);
	fclose(fp_dest);

	TNode List[3] = {
		{"홍길동", 11, "안성"},
		{"홍동", 124589, "서울"},
		{"길동", 123, "도시"}
	};
	fp_dest = fopen("BlockRead.dat", "w");
	fwrite(List, 1, sizeof(TNode) * 3, fp_dest);
	fclose(fp_dest);

	TNode Load[3];
	fp_src = fopen("BlockRead.dat", "r");
	fread(Load, 1, sizeof(TNode) * 3, fp_src);
	fclose(fp_src);

	fp_dest = fopen("LineWrite.txt", "wt");
	for (int i = 0; i < 3; i++)
	{
		fprintf(fp_dest, "\n%s %d %s", GetName(List, i), GetAge(List, i), GetAddress(List, i));
	}
	fclose(fp_dest);

	TNode LineBuffer[3];
	char pBuffer[256];
	int iCount = 0;
	fp_src = fopen("LineWrite.txt", "rt");
	fgets(pBuffer, 256, fp_src);
	while (!feof(fp_src))
	{
		fgets(pBuffer, 256, fp_src);
		sscanf(pBuffer, "%s %d %s", LineBuffer[iCount].m_szName, &LineBuffer[iCount].m_iAge, LineBuffer[iCount].m_strAddress);
		iCount++;
	}
	fclose(fp_src);
}