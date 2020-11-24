#include "Node.h"
template <class T>
bool Node<T>::operator>(Node& NextData)
{
	if (IData->lTotal > NextData.IData.lTotal)
	{
		return 1;
	}
	return 0;
}
template <class T>
bool Node<T>::operator<(Node& NextData)
{
	if (IData->lTotal < NextData.IData.lTotal)
	{
		return 1;
	}
	return 0;
}
template <class T>
bool Node<T>::operator==(Node& NextData)
{
	if (IData->lTotal == NextData.IData.lTotal)
	{
		return 1;
	}
	return 0;
}