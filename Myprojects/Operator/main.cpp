// 14��. C++�����ڿ����ε�
#include "TPoint.h"
void (TPoint::*fp) ();
void Execute(TPoint* the)
{
	if (fp != NULL)
	{
		(the->*fp)();
	}
}
int main(void)
{
	TPoint p1(0, 0), p2, p3, p4;
	p2 = ++p1;	// ������. operator++() ȣ��
	p1.Show();
	p2.Show();

	p1->Show();
	(*p1).Show();
	TPoint* pData = new TPoint();
	pData->Show();

	p3 = p1++;	// ������. operator++(int) ȣ��
	p1.Show();
	p3.Show();

	p4 = p1 + p2;
	p4.Show();

	TPoint p5(10, 20), p6, p7, p8;
	p6 = p5 * 2;		// ����Լ� ȣ�� 
	p6.Show();
	p6 = 2 * p5;		// friend �Լ� ȣ��
	p6.Show();

	p7 = p5 / 2;		// ����Լ� ȣ�� 
	p7.Show();
	p8 = 2 / p5;		// friend �Լ� ȣ��
	p8.Show();

	p5 += p4;
	p5.Show();
	p6 -= p5;
	p6.Show();

	p5 *= 2;
	p5.Show();
	p6 /= 2;
	p6.Show();

	p5 *= p4;
	p5.Show();
	p6 /= p4;
	p6.Show();

	if (p5 == p6) cout << "����.";
	if (p5 != p6) cout << "�ٸ���.";

	cout << p6[0] << endl;		// x��ǥ
	cout << p6[1] << endl;		// y��ǥ
	cout << p6(0) << endl;		// x��ǥ
	cout << p6(1) << endl;		// y��ǥ


	TPoint a, b, c;
	cout << "ù��°  �Է�: ";
	cin >> a;					// >> ������ ȣ��
	cout << "�ι�°  �Է�: ";
	cin >> b;					// >> ������ ȣ��

	cout << "a = " << a << endl;		// << ������ ȣ��
	cout << "b = " << b << endl;		// << ������ ȣ��
	cout << "a+b = " << a + b << endl;	// + ������ ȣ��
	cout << "a-b = " << a - b << endl;	// - ������ ȣ��
	cout << "a*b = " << a * b << endl;	// * ������ ȣ��
	cout << "a/b = " << a / b << endl;	// / ������ ȣ��

	TPoint* d = new TPoint();
	d->SetPosirion(10, 10);
	cout << *d;

	// ���� �Լ�������
	fp = &TPoint::Show;
	Execute(d); // �Լ��� ����

	//Ŭ��������Լ� �Լ�������
	d->m_fp = &TPoint::Show;
	(*d) > &a; // ������ �����ε����� ����
	d->Execute(); // Ŭ�����Լ��� ����
	delete d;
	return 0;
}
