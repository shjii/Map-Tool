#include "pch.h"
#include "FileIO.h"

bool FileIO::Save(MapFileData data, ID3D11DeviceContext*   pImmediateContext)
{
	ofstream fout("TEST.txt");

	fout << data.fTile << endl;
	fout << data.fCell << endl;
	fout << data.fCellSize << endl;
	fout << to_wm(data.fTexture) << "\n" ;
	fout << data.fyList.size() << endl;
	for (int i = 0; i < data.fyList.size(); i++)
	{
		fout << data.fyList[i]<< endl;
	}
	for (int i = 0; i < 4; i++)
	{
		fout << to_wm(data.m_LayerList[i]) << endl;
	}

	fout << data.btext;
	//D3D11_TEXTURE2D_DESC desc;
	//data.btext->GetDesc(&desc);

	//D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	//if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)data.btext,
	//	0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	//{
	//	BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;

	//	for (UINT y = 0; y < desc.Height; y++)
	//	{
	//		for (UINT x = 0; x < desc.Width; x++)
	//		{
	//			fout << *pDestBytes++ << endl;
	//			fout << *pDestBytes++ << endl;
	//			fout << *pDestBytes++ << endl;
	//			fout << *pDestBytes++ << endl;
	//		}
	//	}
	//}
	//pImmediateContext->Unmap(data.btext, 0);
	fout.close();
	return true;
}
MapFileData FileIO::Load()
{
	MapFileData data;

	ifstream fin("TEST.txt");
	char line[1000];
	float  iB;
		fin >> iB;
		data.fTile = iB;
		fin >> iB;
		data.fCell = iB;
		fin >> iB;
		data.fCellSize = iB;
		fin >> line;
		data.fTexture = to_mw(line);
		fin >> iB;
		int a = iB;
		for (int i = 0; i < a; i++)
		{
			fin >> iB;
			data.fyList.push_back(iB);
		}
		data.m_LayerList.resize(4);
		for (int j = 0; j < 4; j++)
		{
			fin >> line;
			data.m_LayerList[j] = to_mw(line);
		}
	fin.close();




	return data;
}