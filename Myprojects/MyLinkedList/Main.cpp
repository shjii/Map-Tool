#include "MyLinkedList.h"

int main()
{
	MyClass<LNode> MyClass;

	//FileIO FileIo;

	MyClass.reset();

	bool repeat = false;

	while (!repeat)
	{
		int step = 0;
		printf("���� = (0) ��� = (1) �˻� = (2) ���� = (3) ���� = (4) ���� = (5) ���� = (6) �ҷ����� = (7) ���� = (8)(9) ���� = (99) \n");
		scanf("%d", &step);
		switch (step) {
		case 0:
		{
			system("cls");
			MyClass.Produce(step);
		}break;

		case 1:
		{
			system("cls");
			MyClass.AllPrintNode();
		}break;

		case 2:
		{
			system("cls");
			MyClass.AllPrintNode();
			char a[20] = { 0 };
			printf("�˻��� �̸� : \n");
			scanf("%s", &a);
			MyClass.LFindNode = MyClass.SearchNode(a);
			if (MyClass.LFindNode != NULL)
			{
				system("cls");
				MyClass.PrintNode(MyClass.LFindNode);
			}
			else
			{
				system("cls");
				printf("�˻��� ����!!");
			}
		}break;

		case 3:
		{
			if (MyClass.LFindNode != NULL)
			{
				system("cls");
				printf("%s ����\n", &MyClass.LFindNode->IData->lName);
				MyClass.DelNode(MyClass.LFindNode);
				MyClass.LFindNode = 0;
			}
			else
			{
				system("cls");
				MyClass.AllPrintNode();
				printf("�˻��� ����!!\n�˻��� �̸� : \n");
				char a[20] = { 0 };
				scanf("%s", &a);
				MyClass.LFindNode = MyClass.SearchNode(a);
				if (MyClass.LFindNode != NULL)
				{
					system("cls");
					printf("%s ����\n", &MyClass.LFindNode->IData->lName);
					MyClass.DelNode(MyClass.LFindNode);
					MyClass.LFindNode = 0;
				}
				else
				{
					printf("�˻��� ����!!\n");
				}
			}

		}break;

		case 4:
		{
			if (MyClass.LFindNode != NULL)
			{
				system("cls");
				printf("%s ����\n", &MyClass.LFindNode->IData->lName);
				MyClass.UpdateData(MyClass.LFindNode);
				MyClass.LFindNode = 0;
			}
			else
			{
				system("cls");
				MyClass.AllPrintNode();
				printf("�˻��� ����!!\n�˻��� �̸� : \n");
				char a[20] = { 0 };
				scanf("%s", &a);
				MyClass.LFindNode = MyClass.SearchNode(a);
				if (MyClass.LFindNode != NULL)
				{
					system("cls");
					printf("%s ����\n", &MyClass.LFindNode->IData->lName);
					MyClass.UpdateData(MyClass.LFindNode);
					MyClass.LFindNode = 0;
				}
				else
				{
					printf("�˻��� ����!!\n");
				}
			}
		}break;

		case 5:
		{
			if (MyClass.LFindNode != NULL)
			{
				system("cls");
				MyClass.Produce(step);
				MyClass.LFindNode = 0;
			}
			else
			{
				system("cls");
				MyClass.AllPrintNode();
				printf("�˻��� ����!!\n�˻��� �̸� : \n");
				char a[20] = { 0 };
				scanf("%s", &a);
				MyClass.LFindNode = MyClass.SearchNode(a);
				if (MyClass.LFindNode != NULL)
				{
					system("cls");
					MyClass.Produce(step);
					MyClass.LFindNode = 0;
				}
				else
				{
					printf("�˻��� ����!!\n");
				}
			}

		}break;

		case 6:
		{
			system("cls");
			printf("���� ���� �Ϸ�\n");
			MyClass.SaveData("TEST.txt");
		}break;

		case 7:
		{
			system("cls");
			printf("���� �ҷ����� �Ϸ�\n");
			MyClass.LoadLink("TEST.txt");
		}break;

		case 8:
		{
			MyClass.Ascending();
		}break;

		case 9:
		{
			MyClass.Descending();
		}break;

		case 99:
		{
			system("cls");
			printf("���α׷� ����\n");
			repeat = true;
		}break;

		default:
		{
			printf("�߸� �Է� �ϼ̽��ϴ�.\n");
		}
		break;
		}
	}
	MyClass.AllDelNode();
	delete MyClass.myListHede;
	delete MyClass.myListTail;
}