#include "pch.h"
#include "FileIO.h"

bool FileIO::Save(MapFileData data, ID3D11Device* pDevice, ID3D11DeviceContext*   pImmediateContext)
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
	HRESULT hr;
	ScratchImage Simage;
	if (FAILED(hr = DirectX::CaptureTexture(pDevice, pImmediateContext, data.btext, Simage)))
	{
		H(hr);
		return nullptr;
	}
	if (FAILED(hr = DirectX::SaveToDDSFile(Simage.GetImages(), Simage.GetImageCount(), Simage.GetMetadata(), DirectX::DDS_FLAGS_NONE, L"abc.dds")))
	{
		H(hr);
		return nullptr;
	}
	fout.close();
	return true;
}
bool FileIO::Load(MapFileData* data, ID3D11Device* pDevice, ID3D11DeviceContext*   pImmediateContext)
{
	ifstream fin("TEST.txt");
	char line[1000];
	float  iB;
		fin >> iB;
		data->fTile = iB;
		fin >> iB;
		data->fCell = iB;
		fin >> iB;
		data->fCellSize = iB;
		fin >> line;
		data->fTexture = to_mw(line);
		fin >> iB;
		int a = iB;
		for (int i = 0; i < a; i++)
		{
			fin >> iB;
			data->fyList.push_back(iB);
		}
		data->m_LayerList.resize(4);
		for (int j = 0; j < 4; j++)
		{
			fin >> line;
			data->m_LayerList[j] = to_mw(line);
		}
	fin.close();

	//TexMetadata* metadata;
	//ScratchImage image;

	//LoadFromDDSFile(L"abc.dds",
	//	DDS_FLAGS_NONE, metadata, image);
	//ID3D11ShaderResourceView *resourceView;
	//CreateShaderResourceView(pDevice, pImmediateContext,"abc.dds");
	ID3D11ShaderResourceView **resourceView = nullptr;
	HRESULT result = E_FAIL;

	DirectX::TexMetadata	md;
	DirectX::ScratchImage	img;
	result = LoadFromDDSFile(L"abc.dds", DDS_FLAGS_NONE, &md, img);
	result = CreateShaderResourceView(pDevice, img.GetImages(), img.GetImageCount(), md, &data->ResourceView);

	ID3D11Resource *res;
	data->ResourceView->GetResource(&res);
	res->QueryInterface<ID3D11Texture2D>(&data->btext);

	return true;
}