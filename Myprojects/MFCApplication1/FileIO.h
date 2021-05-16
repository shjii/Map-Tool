#pragma once
#include "SMiniMap.h"
#include "SText.h"
#include "SQuadTree.h"
#include "SModel.h"
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
};

class FileIO
{
public:
	bool Save(MapFileData data, ID3D11DeviceContext*   pImmediateContext);
	MapFileData Load();
};

