#include "Sample.h"
int main()
{
	FileLoad();

	srand(time(NULL));
	int select;
	while (1)
	{
		printf("\n����:0,�ε�:1,�ű�:2,���:3,�˻���ȣ:4,�˻��̸�:5,�ʱ�:9,����(99)");
		scanf("%d", &select);
		if (select == Exit) break;
		system("cls");
		switch (select)
		{
		case DataSave: {
			if (FileSave() > 0)
				printf("\n������ ����Ǿ����ϴ�.");
			else
				printf("\n������ �������� ���߽��ϴ�.");
		}break;
		case DataLoad: {
			if (FileLoad() > 0)
				printf("\n������ �ε�Ǿ����ϴ�.");
			else
				printf("\n������ �ε����� ���߽��ϴ�.");
		}break;
		case DataNew: {
			NewData();
		}break;
		case DataPrint: {
			PrintData();
		}break;
		case DataFindIndex: {
			FindIndex();
		}break;
		case DataFindName: {
			FindName();
		}break;
		case SampleData: {
			// ��� g_pStudentData�� �����ǰ� ���ÿ� �����Ͱ� �����ȴ�.
			CreateSampleData();
		}break;
		}
	}
	free(g_pStudentData);

	printf("\n�ƹ�Ű�� ������ ����˴ϴ�. !");
	_getch();
	return 0;
}