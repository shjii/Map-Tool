#pragma once
#include "TObject.h"
#include "SCamera.h"

struct NodeMtrlInfo
{
	int		iMtrl;	// �ؽ��� �ε���
	int		iNumFaces; // ���̽� ����
	int		iNumVertex;// ������ ����
};

struct TPolygon
{
	int				iNumVertices;		// ���� ����
	int				iNumIndices;		// �ε��� ����	
	int				iTexIndex;			// �ؽ��� �ε���
	bool			bBeenUsedAsSplitter;// ���Ҹ� ��뿩�� �Ǵ�  	
	Vector3		vNormal;			// ��� �븻	
	PNCT_VERTEX*	pVertexList;		// ���� ������ 
	TCHAR			strTexName[30];
	DWORD			*pIndices;			// �ε��� ������	
	TPolygon		*pNext;				// ���� ����Ʈ	
	virtual ~TPolygon()
	{
		SAFE_DEL(pIndices);
		SAFE_DEL(pVertexList);
	}
};

class SNode
{
public:
	DWORD				m_dwDepth;
	BOOL				m_isLeaf;
	BOOL				m_isTile;
	DWORD				m_dwLodLevel;
	DWORD				m_dwLodType;
	// �ٿ�� �ڽ�
	S_BOX				S_BOX;
	S_SPHERE			m_Sphere;
	vector<DWORD>		m_CornerIndex;
	// ����� �ڳ� ���� �ε��� �� ��ġ
	vector<Vector3>	m_CornerList;
	vector<SNode*>		m_ChildList;
	vector<SNode*>		m_NeighborList;
	vector<TObject*>	m_ObjectList;
	// �߰�
	DWORD				m_dwPositionIndex[2];// X,Y	

	D3D11_BOX			m_BoxVB;
	D3D11_BOX			m_BoxIB;
	UINT				m_iBeginVB;
	UINT				m_iBeginIB;
	vector<DWORD>		m_IndexList;
	vector <PNCT_VERTEX>	m_VertexList;
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;

	// bsp
	DWORD				m_dwIndex;
	vector<ComPtr<ID3D11Buffer>>	m_pVBList;
	vector<ComPtr<ID3D11Buffer>>	m_pIBList;
	int					iNumPolygon;
	int					iNumMtrl;	// ������������ ��� ����ϴ� �� �ؽ����� ����
	vector<NodeMtrlInfo> m_MtrlList;
	TPolygon			*m_pSplitter; // Splitter poly for this node
public:
	SNode();
	virtual ~SNode();
};