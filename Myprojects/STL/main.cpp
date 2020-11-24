#include "obj.h"

vector< Weapon> a;

void DB() {
	a = {Weapon("a",1,1), Weapon("b",1,1) ,Weapon("c",1,1) , Weapon("d",1,1) , Weapon("e",1,1) , Weapon("f",1,1) };
}

Weapon* getItem(string _name)
{
	for (int i = 0; i < a.size(); ++i)
	{
		if (_name == a[i].getname())
		{
			return &a[i];
		}
	}
	cout << " 없는 아이템" << endl;
	return nullptr;
}

int main()
{
	DB();
	a.resize(6);
	// 초기화
	vector<list<CItem*>*> vecInven;
	list<CItem*>* pListItem;
	list<CItem*>::iterator iter;
	pListItem = new list<CItem*>;

	// 슬롯 갯수
	vecInven.resize(10);
	pListItem->resize(10);

	for (int i = 0; i < vecInven.size(); ++i)
		vecInven[i] = pListItem;

	int iCtrNum = 0;
	int iSlot = 0;
	int iKind = 0;
	char ItemName[20];
	

	while (10 != iCtrNum)
	{
		iter = pListItem->begin();

		system("cls");
		cout << " 1 추가" << endl;
		cout << " 2 삭제" << endl;
		cout << " 3 출력" << endl;
		cout << " 10 종료" << endl;
		cin >> iCtrNum;

		switch (iCtrNum)
		{
		case 1:
			system("cls");
			cout << "추가할 슬롯 : ";
			cin >> iSlot;
			for (int i = 0; i < iSlot - 1; ++i)
				iter++;

			cout << "추가할 아이템 : ";
			cin >> ItemName;

			if (*iter != nullptr)
			{
				cout << "아이템이 있습니다." << endl;
				break;
			}

			if (!getItem(ItemName) == NULL)
			{
				(*iter) = new CItem(*(getItem(ItemName)));
			}
			break;

		case 2:
			system("cls");
			cout << "삭제할 슬롯 : ";
			cin >> iSlot;
			for (int i = 0; i < iSlot - 1; ++i)
				iter++;

			if (*iter != nullptr)
			{
				(*iter) = nullptr;
			}
			else
			{
				cout << "아이템이 없습니다." << endl;
			}
			break;

		case 3:
			system("cls");
			cout << "인벤토리 목록 출력 " << endl;
			for (int i = 0; i < vecInven.size(); ++i)
			{
				if (NULL == (*iter))
				{
					cout << i + 1 << " 번째 : 없음 " << endl;
					iter++;
				}
				else
				{
					cout << i + 1 << " 번째 : " << (*(iter))->GetName() << endl;
					iter++;
				}
			}
			break;
		default:
			if (iCtrNum == 10)
			{
				cout << "프로그램 종료" << endl;
				break;
			}
			cout << " 잘못된 수 " << endl;
			break;
		}
		system("pause");

	}

	iter = pListItem->begin();

	for (; iter != pListItem->end(); ++iter)
		delete *iter;

	delete pListItem;
}