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

	virtual void getprint() { cout << ".아무것도 없습니다." << endl; }

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
		cout << "이름 :" << name << "설명 : " << description << "공격력 :" << offensivePower << endl;
	d_getprintend;

};

class SwordC1:public Sword
{
public:
	SwordC1()
	{
		name = "무기1";
		description = "무기 1번 입니다.";
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
		name = "무기2";
		description = "무기 2번 입니다.";
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
	cout << "이름 :" << name << "설명 : " << description << "방어력 :" << Defense << "착용부위 :"<< wearingPart << endl;
	d_getprintend;
};

class Shield :public Armor
{
public:
	Shield()
	{
		name = "방패";
		description = "방패 입니다.";
		Defense = 10;
		wearingPart = "손";
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
		name = "헬멧";
		description = "헬멧 입니다.";
		Defense = 5;
		wearingPart = "머리";
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
		cout << " 이름 :" << name << "설명 : " << description << "회복 : " << Recovery << "수량 :" << Quantity << endl;
	}
};

class REDPotion :public Potion
{
public:
	REDPotion()
	{
		name = "빨간포션";
		description = "체력을 10 올려줍니다.";
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
		cout << " 이름 :" << name << "설명 : " << description << "스킬 : " << Skill << "수량 :" << Quantity << endl;
	}
};

class Fire :public MagicBook
{
public:
	Fire()
	{
		name = "불마법";
		description = "스킬 불 사용.";
		Skill = "불쏘기";
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
	_iTem->insert(map<string, Item*>::value_type("무기1", new SwordC1()));
	_iTem->insert(map<string, Item*>::value_type("무기2", new SwordC2()));
	_iTem->insert(map<string, Item*>::value_type("방패", new Shield()));
	_iTem->insert(map<string, Item*>::value_type("헬멧", new Helmet()));
	_iTem->insert(map<string, Item*>::value_type("빨강포션", new REDPotion()));
	_iTem->insert(map<string, Item*>::value_type("불마법주문서", new Fire()));
}