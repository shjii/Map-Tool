#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

#define d_getprint 	void getprint() {
#define d_getprintend }


class Item
{
public:
	string name;
	string description;
	int Quantity;

	Item() {}
	Item(const Item& _rhs) {}
	virtual ~Item() {}

	virtual void getprint() { cout << ".�ƹ��͵� �����ϴ�." << endl; }

	virtual bool getQuantity(string _a) { return false; }

	virtual bool delQuantity() { return false; }

	virtual Item* clone()
	{
		return new Item(*this);
	}

};

class Weapon :public Item
{
public:
	Weapon() {}
	~Weapon() {}

	bool getQuantity(string _a) { return false; }
	bool delQuantity() { return false; }

};

class Sword:public Weapon
{
public:
	int offensivePower;

	Sword() {}
	~Sword() {}
	
	d_getprint;
		cout << "�̸� :" << name << "���� : " << description << "���ݷ� :" << offensivePower << endl;
	d_getprintend;

};

class SwordC1:public Sword
{
public:
	SwordC1()
	{
		name = "����1";
		description = "���� 1�� �Դϴ�.";
		offensivePower = 1;
	}
	SwordC1(const SwordC1& _rhs)
	{
		name = _rhs.name;
		description = _rhs.description;
		offensivePower = _rhs.offensivePower;
	}
	~SwordC1() {}

	Item* clone()
	{
		return new SwordC1(*this);
	}
};

class SwordC2 :public Sword
{
public:
	SwordC2()
	{
		name = "����2";
		description = "���� 2�� �Դϴ�.";
		offensivePower = 100;
	}
	SwordC2(const SwordC2& _rhs)
	{
		name = _rhs.name;
		description = _rhs.description;
		offensivePower = _rhs.offensivePower;
	}
	~SwordC2() {}

	Item* clone()
	{
		return new SwordC2(*this);
	}
};

class Armor :public Weapon
{
public:
	string wearingPart;
	int Defense;

	Armor() {}
	~Armor() {}

	d_getprint;
	cout << "�̸� :" << name << "���� : " << description << "���� :" << Defense << "������� :"<< wearingPart << endl;
	d_getprintend;
};

class Shield :public Armor
{
public:
	Shield()
	{
		name = "����";
		description = "���� �Դϴ�.";
		Defense = 10;
		wearingPart = "��";
	}
	Shield(const Shield& _rhs)
	{
		name = _rhs.name;
		description = _rhs.description;
		Defense = _rhs.Defense;
		wearingPart = _rhs.wearingPart;
	}
	~Shield() {}

	Item* clone()
	{
		return new Shield(*this);
	}
};

class Helmet :public Armor
{
public:
	Helmet()
	{
		name = "���";
		description = "��� �Դϴ�.";
		Defense = 5;
		wearingPart = "�Ӹ�";
	}
	Helmet(const Helmet& _rhs)
	{
		name = _rhs.name;
		description = _rhs.description;
		Defense = _rhs.Defense;
		wearingPart = _rhs.wearingPart;
	}
	~Helmet() {}

	Item* clone()
	{
		return new Helmet(*this);
	}
};

class consumable :public Item
{
public:
	

	consumable() {}
	~consumable() {}

	 bool getQuantity(string _a)
	{
		if (Quantity < 3 && _a == name)
		{
			Quantity++;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool delQuantity()
	{
		if (Quantity <= 1)
		{
			return false;
		}
		Quantity--;
		return true;
	}

};

class Potion :public consumable
{
public:
	int Recovery;

	Potion() {}
	~Potion() {}

	void getprint()
	{
		cout << " �̸� :" << name << "���� : " << description << "ȸ�� : " << Recovery << "���� :" << Quantity << endl;
	}
};

class REDPotion :public Potion
{
public:
	REDPotion()
	{
		name = "��������";
		description = "ü���� 10 �÷��ݴϴ�.";
		Recovery = 10;
		Quantity = 1;
	}
	REDPotion(const REDPotion& _rhs)
	{
		name = _rhs.name;
		description = _rhs.description;
		Recovery = _rhs.Recovery;
		Quantity = _rhs.Quantity;
	}
	~REDPotion() {}

	Item* clone()
	{
		return new REDPotion(*this);
	}
};

class MagicBook :public consumable
{
public:
	string Skill;

	MagicBook() {}
	~MagicBook() {}

	void getprint()
	{
		cout << " �̸� :" << name << "���� : " << description << "��ų : " << Skill << "���� :" << Quantity << endl;
	}
};

class Fire :public MagicBook
{
public:
	Fire()
	{
		name = "�Ҹ���";
		description = "��ų �� ���.";
		Skill = "�ҽ��";
		Quantity = 1;
	}
	Fire(const Fire& _rhs)
	{
		name = _rhs.name;
		description = _rhs.description;
		Skill = _rhs.Skill;
		Quantity = _rhs.Quantity;
	}
	~Fire() {}

	Item* clone()
	{
		return new Fire(*this);
	}
};

void reSetitem(map<string, Item*> *_iTem)
{
	_iTem->insert(map<string, Item*>::value_type("����1", new SwordC1()));
	_iTem->insert(map<string, Item*>::value_type("����2", new SwordC2()));
	_iTem->insert(map<string, Item*>::value_type("����", new Shield()));
	_iTem->insert(map<string, Item*>::value_type("���", new Helmet()));
	_iTem->insert(map<string, Item*>::value_type("��������", new REDPotion()));
	_iTem->insert(map<string, Item*>::value_type("�Ҹ����ֹ���", new Fire()));
}