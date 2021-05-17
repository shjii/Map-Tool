#pragma once
#include "SMiniMap.h"
#include "SText.h"
#include "SQuadTree.h"
#include "SModel.h"
#include "DirectXTex.h"
#include <fstream>
struct MapFileData
{
	int fTile;
	int fCell;
	int fCellSize;
	wstring fTexture;
	vector<float> fyList;
	vector<wstring>	m_LayerList;
	ID3D11Texture2D* btext;
	ID3D11ShaderResourceView* ResourceView;
};

class FileIO
{
public:
	bool Save(MapFileData data, ID3D11Device* pDevice, ID3D11DeviceContext*   pImmediateContext);
	bool Load(MapFileData* data, ID3D11Device* pDevice, ID3D11DeviceContext*   pImmediateContext);
};

