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
	cout << " ���� ������" << endl;
	return nullptr;
}

int main()
{
	DB();
	a.resize(6);
	// �ʱ�ȭ
	vector<list<CItem*>*> vecInven;
	list<CItem*>* pListItem;
	list<CItem*>::iterator iter;
	pListItem = new list<CItem*>;

	// ���� ����
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
		cout << " 1 �߰�" << endl;
		cout << " 2 ����" << endl;
		cout << " 3 ���" << endl;
		cout << " 10 ����" << endl;
		cin >> iCtrNum;

		switch (iCtrNum)
		{
		case 1:
			system("cls");
			cout << "�߰��� ���� : ";
			cin >> iSlot;
			for (int i = 0; i < iSlot - 1; ++i)
				iter++;

			cout << "�߰��� ������ : ";
			cin >> ItemName;

			if (*iter != nullptr)
			{
				cout << "�������� �ֽ��ϴ�." << endl;
				break;
			}

			if (!getItem(ItemName) == NULL)
			{
				(*iter) = new CItem(*(getItem(ItemName)));
			}
			break;

		case 2:
			system("cls");
			cout << "������ ���� : ";
			cin >> iSlot;
			for (int i = 0; i < iSlot - 1; ++i)
				iter++;

			if (*iter != nullptr)
			{
				(*iter) = nullptr;
			}
			else
			{
				cout << "�������� �����ϴ�." << endl;
			}
			break;

		case 3:
			system("cls");
			cout << "�κ��丮 ��� ��� " << endl;
			for (int i = 0; i < vecInven.size(); ++i)
			{
				if (NULL == (*iter))
				{
					cout << i + 1 << " ��° : ���� " << endl;
					iter++;
				}
				else
				{
					cout << i + 1 << " ��° : " << (*(iter))->GetName() << endl;
					iter++;
				}
			}
			break;
		default:
			if (iCtrNum == 10)
			{
				cout << "���α׷� ����" << endl;
				break;
			}
			cout << " �߸��� �� " << endl;
			break;
		}
		system("pause");

	}

	iter = pListItem->begin();

	for (; iter != pListItem->end(); ++iter)
		delete *iter;

	delete pListItem;
}