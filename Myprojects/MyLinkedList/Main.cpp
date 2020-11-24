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
		printf("생성 = (0) 출력 = (1) 검색 = (2) 삭제 = (3) 수정 = (4) 삽입 = (5) 저장 = (6) 불러오기 = (7) 정렬 = (8)(9) 종료 = (99) \n");
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
			printf("검색할 이름 : \n");
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
				printf("검색값 없음!!");
			}
		}break;

		case 3:
		{
			if (MyClass.LFindNode != NULL)
			{
				system("cls");
				printf("%s 삭제\n", &MyClass.LFindNode->IData->lName);
				MyClass.DelNode(MyClass.LFindNode);
				MyClass.LFindNode = 0;
			}
			else
			{
				system("cls");
				MyClass.AllPrintNode();
				printf("검색값 없음!!\n검색할 이름 : \n");
				char a[20] = { 0 };
				scanf("%s", &a);
				MyClass.LFindNode = MyClass.SearchNode(a);
				if (MyClass.LFindNode != NULL)
				{
					system("cls");
					printf("%s 삭제\n", &MyClass.LFindNode->IData->lName);
					MyClass.DelNode(MyClass.LFindNode);
					MyClass.LFindNode = 0;
				}
				else
				{
					printf("검색값 없음!!\n");
				}
			}

		}break;

		case 4:
		{
			if (MyClass.LFindNode != NULL)
			{
				system("cls");
				printf("%s 수정\n", &MyClass.LFindNode->IData->lName);
				MyClass.UpdateData(MyClass.LFindNode);
				MyClass.LFindNode = 0;
			}
			else
			{
				system("cls");
				MyClass.AllPrintNode();
				printf("검색값 없음!!\n검색할 이름 : \n");
				char a[20] = { 0 };
				scanf("%s", &a);
				MyClass.LFindNode = MyClass.SearchNode(a);
				if (MyClass.LFindNode != NULL)
				{
					system("cls");
					printf("%s 수정\n", &MyClass.LFindNode->IData->lName);
					MyClass.UpdateData(MyClass.LFindNode);
					MyClass.LFindNode = 0;
				}
				else
				{
					printf("검색값 없음!!\n");
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
				printf("검색값 없음!!\n검색할 이름 : \n");
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
					printf("검색값 없음!!\n");
				}
			}

		}break;

		case 6:
		{
			system("cls");
			printf("파일 저장 완료\n");
			MyClass.SaveData("TEST.txt");
		}break;

		case 7:
		{
			system("cls");
			printf("파일 불러오기 완료\n");
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
			printf("프로그램 종료\n");
			repeat = true;
		}break;

		default:
		{
			printf("잘못 입력 하셨습니다.\n");
		}
		break;
		}
	}
	MyClass.AllDelNode();
	delete MyClass.myListHede;
	delete MyClass.myListTail;
}