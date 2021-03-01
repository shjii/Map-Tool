#include "main.h"

string solution(string new_id) {
	string answer = "";

	// 1�ܰ� ��� �빮�ڸ� �����Ǵ� �ҹ��ڷ� ġȯ
	transform(new_id.begin(), new_id.end(), new_id.begin(), ::tolower);
	// 2�ܰ� 3�ܰ� 4�ܰ�
	bool t = false;
	for (int a = 0; a < new_id.size(); a++)
	{
		if (answer.size() >= 15) break;

		if (islower(new_id[a]) || atoi(&new_id[a]) != 0 || new_id[a] == '-' ||
			new_id[a] == '_' || new_id[a] == '0')
		{
			answer += new_id[a];
		}

		if (new_id[a] == '.')
		{
			if (a != 0)
			{
				if (t == false)answer += new_id[a];
			}
			t = true;
		}
		else
		{
			t = false;
		}
	}
	//5�ܰ�
	if (answer.empty()) answer = "aaa";
	//6�ܰ�
	while (answer.back() == '.')
	{
		answer.pop_back();
	}


	while (answer.size() < 3)
	{
		answer.push_back(answer.back());
	}
	return answer;
}

int main()
{
	string ain = ".....a";
	solution(ain);
	vector<Item*> Inve(10);
	vector<Item*>::iterator inveIte;
	// �κ��丮 ����
	map<string, Item*> iTem;
	map<string, Item*>::iterator iteITem;
	reSetitem(&iTem);
	//Inve[0] = iTem["���� 1"]->clone();
	int a = 0;
	while (a != 10)
	{
		cout << "��ȣ �Է� " << endl;
		cout << "1�� : ������ �߰� , 2�� : ��ü ��� , 3�� ���� , 4�� �˻� , 10�� ����" << endl;
		cin >> a;
		switch (a)
		{
		case 1:
		{
			for (auto& pair : iTem)
			{
				cout << pair.first << endl;
			}
			cout << "�߰��Ͻ� �������� �Է��ϼ���(�̸�).";

			string str;

			cin >> str;

			bool tf = false;

			if (iTem.find(str) == iTem.end())
			{
				cout << "�߸� �Է� �ϼ̽��ϴ�." << endl;
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
						cout << "�������� �߰� " << endl;
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
				cout << "���� ������ �����ϴ�." << endl;
			}

		}break;

		case 2:
		{
			for (inveIte = Inve.begin(); inveIte != Inve.end(); inveIte++)
			{
				if (*inveIte == NULL)
				{
					cout << ".�ƹ��͵� �����ϴ�." << endl;
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
					cout << ".�ƹ��͵� �����ϴ�." << endl;
				}
				else
				{
					(*inveIte)->getprint();
				}
			}

			cout << "������ ������ �̸�." << endl;

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

			cout << "�߸� �Է� �Ͽ����ϴ�." << endl;

		}break;

		case 4:
		{
			cout << "�˻� �̸�" << endl;
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
				cout << " �߸� �Է� �Ͽ����ϴ�." << endl;
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