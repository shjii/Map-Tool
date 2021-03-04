#include "main.h"

string solution(string new_id) {
	string answer = "";

	int iVertexIndex = 0;
	for (int dwRow = 0; dwRow <= 16; dwRow++)
	{
		for (int dwCol = 48; dwCol <= 63; dwCol++)
		{
			iVertexIndex++;
		}
	}
	
	return answer;
}




int main()
{
	string ain = ".....a";
	solution(ain);
	vector<Item*> Inve(10);
	vector<Item*>::iterator inveIte;
	// 인벤토리 공간
	map<string, Item*> iTem;
	map<string, Item*>::iterator iteITem;
	reSetitem(&iTem);
	//Inve[0] = iTem["무기 1"]->clone();
	int a = 0;
	while (a != 10)
	{
		cout << "번호 입력 " << endl;
		cout << "1번 : 아이템 추가 , 2번 : 전체 출력 , 3번 삭제 , 4번 검색 , 10번 종료" << endl;
		cin >> a;
		switch (a)
		{
		case 1:
		{
			for (auto& pair : iTem)
			{
				cout << pair.first << endl;
			}
			cout << "추가하실 아이템을 입력하세요(이름).";

			string str;

			cin >> str;

			bool tf = false;

			if (iTem.find(str) == iTem.end())
			{
				cout << "잘못 입력 하셨습니다." << endl;
				break;
			}

			bool fild = false;
			for (inveIte = Inve.begin(); inveIte != Inve.end(); inveIte++)
			{
				if ((*inveIte) != NULL)
				{
					fild = (*inveIte)->getQuantity(str);
					if (fild)
					{
						cout << "아이템을 추가 " << endl;
						tf = true;
						break;
					}
				}
			}
			if (tf) break;

			for (inveIte = Inve.begin(); inveIte != Inve.end(); inveIte++)
			{
				if (*inveIte == NULL)
				{
					*inveIte = iTem[str]->clone();
					tf = true;
					break;
				}
			}

			if (!tf)
			{
				cout << "남은 슬롯이 없습니다." << endl;
			}

		}break;

		case 2:
		{
			for (inveIte = Inve.begin(); inveIte != Inve.end(); inveIte++)
			{
				if (*inveIte == NULL)
				{
					cout << ".아무것도 없습니다." << endl;
				}
				else
				{
					(*inveIte)->getprint();
				}
			}
		}break;

		case 3:
		{
			for (inveIte = Inve.begin(); inveIte != Inve.end(); inveIte++)
			{
				if (*inveIte == NULL)
				{
					cout << ".아무것도 없습니다." << endl;
				}
				else
				{
					(*inveIte)->getprint();
				}
			}

			cout << "삭제할 아이템 이름." << endl;

			string str;

			cin >> str;

			bool qf = false;

			for (inveIte = Inve.begin(); inveIte != Inve.end(); inveIte++)
			{
				if (*inveIte != NULL)
				{
					if ((*inveIte)->name == str)
					{
						qf = (*inveIte)->delQuantity();
						if (!qf)
						{
							delete (*inveIte);
							(*inveIte) = NULL;
							qf = true;
							break;
						}
						break;
					}
				}
			}
			if (qf)
				break;

			cout << "잘못 입력 하였습니다." << endl;

		}break;

		case 4:
		{
			cout << "검색 이름" << endl;
			string b;
			cin >> b;
			for (inveIte = Inve.begin(); inveIte != Inve.end(); inveIte++)
			{
				if (*inveIte != NULL && (*inveIte)->name == b)
				{
					(*inveIte)->getprint();
				}
			}
		}break;
		default:
		{
			if (a != 10)
				cout << " 잘못 입력 하였습니다." << endl;
		}
		break;
		}
	}

	for (auto& pair : iTem)
	{
		delete pair.second;
	}
	for (inveIte = Inve.begin(); inveIte != Inve.end(); inveIte++)
	{
		delete *inveIte;
	}
	Inve.clear();
}