#include "pch.h"
#include "FileIO.h"

bool FileIO::Save(MapFileData data, ID3D11Device* pDevice, ID3D11DeviceContext*   pImmediateContext, vector<SRT>* srt)
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
	fout << srt->size() << endl;
	for (int i = 0; i < srt->size(); i++)
	{
		fout << to_wm(srt->at(i).name) << endl;
		fout << srt->at(i).m_MatrixList.size() << endl;
		for (int j = 0; j < srt->at(i).m_MatrixList.size(); j++)
		{
			fout << srt->at(i).m_MatrixList[j]._11 << endl;
			fout << srt->at(i).m_MatrixList[j]._22 << endl;
			fout << srt->at(i).m_MatrixList[j]._33 << endl;
			fout << srt->at(i).m_MatrixList[j]._41 << endl;
			fout << srt->at(i).m_MatrixList[j]._42 << endl;
			fout << srt->at(i).m_MatrixList[j]._43 << endl;
			fout << srt->at(i).m_Quaternion[j].x << endl;
			fout << srt->at(i).m_Quaternion[j].y << endl;
			fout << srt->at(i).m_Quaternion[j].z << endl;
		}
	}
	fout.close();
	return true;
}
bool FileIO::Load(MapFileData* data, ID3D11Device* pDevice, ID3D11DeviceContext*   pImmediateContext, vector<SRT>* srt, SCamera& mainc)
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
		fin >> iB;
		int index = iB;
		srt->clear();
		for (int i = 0; i < index; i++)
		{
			SRT BufferSRT;
			fin >> line;
			BufferSRT.m_pObj = new SModel;
			BufferSRT.name = to_mw(line);
			BufferSRT.m_pObj->Build(line, &mainc);
			fin >> iB;
			int listindex = iB;
			for (int j = 0; j < listindex; j++)
			{
				Matrix mB;
				fin >> iB;
				mB._11 = iB;
				fin >> iB;
				mB._22 = iB;
				fin >> iB;
				mB._33 = iB;
				fin >> iB;
				mB._41 = iB;
				fin >> iB;
				mB._42 = iB;
				fin >> iB;
				mB._43 = iB;
				BufferSRT.m_MatrixList.push_back(mB);
				Quaternion qB;
				fin >> iB;
				qB.x = iB;
				fin >> iB;
				qB.y = iB;
				fin >> iB;
				qB.z = iB;
				BufferSRT.m_Quaternion.push_back(qB);
			}
			srt->push_back(BufferSRT);
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