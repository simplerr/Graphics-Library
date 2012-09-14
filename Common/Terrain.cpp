#include <fstream>
#include <sstream>
#include <algorithm>
#include "Terrain.h"
#include "PrimitiveFactory.h"
#include "Graphics.h"
#include "Effects.h"
#include "Camera.h"
#include "Primitive.h"
#include "ShadowMap.h"

Terrain::Terrain()
	: mPrimitive(0), mLayerTextureArraySRV(0), mBlendMapSRV(0), mHeightMapSRV(0)
{
	// Identity matrix.
	XMStoreFloat4x4(&mWorldMatrix, XMMatrixIdentity());
}
	
Terrain::~Terrain()
{
	ReleaseCOM(mLayerTextureArraySRV);
	ReleaseCOM(mBlendMapSRV);
	ReleaseCOM(mHeightMapSRV);
}

void Terrain::Init(ID3D11Device* device, ID3D11DeviceContext* context, const InitInfo& initInfo)
{
	mInfo = initInfo;

	LoadHeightmap();
	Smooth();
	BuildHeightmapSRV(device);

	// Build the terrain primitive.
	mPrimitive = gPrimitiveFactory->CreateTerrain(this);

	// Create the SRV to the texture array.
	vector<string> layerFilenames;
	layerFilenames.push_back(mInfo.LayerMapFilename0);
	layerFilenames.push_back(mInfo.LayerMapFilename1);
	layerFilenames.push_back(mInfo.LayerMapFilename2);
	layerFilenames.push_back(mInfo.LayerMapFilename3);
	layerFilenames.push_back(mInfo.LayerMapFilename4);
	mLayerTextureArraySRV = d3dHelper::CreateTexture2DArraySRV(device, context, layerFilenames);

	// Create the SRV to the blend map.
	HR(D3DX11CreateShaderResourceViewFromFile(device, mInfo.BlendMapFilename.c_str(), 0, 0, &mBlendMapSRV, 0));
}
	
void Terrain::Draw(Graphics* pGraphics)
{
	ID3D11DeviceContext* context = pGraphics->GetContext();

	// Set the input layout and the primitive topology.
	context->IASetInputLayout(Effects::TerrainFX->GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Camera* camera = pGraphics->GetCamera();
	XMMATRIX view = XMLoadFloat4x4(&camera->GetViewMatrix());
	XMMATRIX proj = XMLoadFloat4x4(&camera->GetProjectionMatrix());
	Effects::TerrainFX->SetViewProj(view * proj);
	Effects::TerrainFX->SetEyePosW(camera->GetPosition());
	Effects::TerrainFX->SetLights(pGraphics->GetLightList());
	Effects::TerrainFX->SetFogColor(XMLoadFloat4(&pGraphics->GetFogColor()));
	Effects::TerrainFX->SetFogStart(1000.0f);
	Effects::TerrainFX->SetFogRange(50.0f);
	Effects::TerrainFX->SetMaterial(Material(Colors::White));

	Effects::TerrainFX->SetTexelCellSpaceU(1.0f / mInfo.HeightmapWidth);
	Effects::TerrainFX->SetTexelCellSpaceV(1.0f / mInfo.HeightmapHeight);
	Effects::TerrainFX->SetWorldCellSpace(mInfo.CellSpacing);

	Effects::TerrainFX->SetLayerMapArray(mLayerTextureArraySRV);
	Effects::TerrainFX->SetBlendMap(mBlendMapSRV);
	Effects::TerrainFX->SetHeightMap(mHeightMapSRV);

	ShadowMap* shadowMap = pGraphics->GetShadowMap();
	Effects::TerrainFX->SetShadowMap(shadowMap->GetSRV());
	Effects::TerrainFX->SetShadowTransform(XMLoadFloat4x4(&shadowMap->GetShadowTransform()));
	Effects::TerrainFX->SetTextureScale(20.0f);
	Effects::TerrainFX->Apply();

	mPrimitive->Draw(pGraphics->GetContext());
}

void Terrain::LoadHeightmap()
{
	// A height for each vertex
	std::vector<unsigned char> in( mInfo.HeightmapWidth * mInfo.HeightmapHeight );

	// Open the file.
	std::ifstream inFile;
	inFile.open(mInfo.HeightMapFilename.c_str(), std::ios_base::binary);

	if(inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array and scale it.
	mHeightMap.resize(mInfo.HeightmapHeight * mInfo.HeightmapWidth+258, 0);	// [HACK] [NOTE] + 258 to not go out of bounds!!
	for(UINT i = 0; i < mInfo.HeightmapHeight * mInfo.HeightmapWidth; ++i)
	{
		mHeightMap[i] = (in[i] / 255.0f) * mInfo.HeightScale;
	}
}
	
void Terrain::Smooth()
{
	std::vector<float> dest(mHeightMap.size());

	for(UINT i = 0; i < mInfo.HeightmapHeight; ++i)
	{
		for(UINT j = 0; j < mInfo.HeightmapWidth; ++j)
		{
			dest[i*mInfo.HeightmapWidth+j] = Average(i,j);
		}
	}

	// Replace the old heightmap with the filtered one.
	mHeightMap = dest;
}
	
bool Terrain::InBounds(int i, int j)
{
	// True if ij are valid indices; false otherwise.
	return i >= 0 && i < (int)mInfo.HeightmapHeight && j >= 0 && j < (int)mInfo.HeightmapWidth;
}
	
float Terrain::Average(int i, int j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	// Use int to allow negatives.  If we use UINT, @ i=0, m=i-1=UINT_MAX
	// and no iterations of the outer for loop occur.
	for(int m = i-1; m <= i+1; ++m)
	{
		for(int n = j-1; n <= j+1; ++n)
		{
			if(InBounds(m,n))
			{
				avg += mHeightMap[m*mInfo.HeightmapWidth + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

float Terrain::GetWidth()
{
	// Total terrain width.
	return (mInfo.HeightmapWidth-1)*mInfo.CellSpacing;
}

float Terrain::GetDepth()
{
	// Total terrain depth.
	return (mInfo.HeightmapHeight-1)*mInfo.CellSpacing;
}

float Terrain::GetHeight(float x, float z)
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*GetWidth()) /  mInfo.CellSpacing;
	float d = (z - 0.5f*GetDepth()) / -mInfo.CellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = mHeightMap[row*mInfo.HeightmapWidth + col];
	float B = mHeightMap[row*mInfo.HeightmapWidth + col + 1];
	float C = mHeightMap[(row+1)*mInfo.HeightmapWidth + col];
	float D = mHeightMap[(row+1)*mInfo.HeightmapWidth + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if( s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f-s)*uy + (1.0f-t)*vy;
	}
}

//! Builds the heightmap SRV.
void Terrain::BuildHeightmapSRV(ID3D11Device* device)
{
	// Fill out the texture description.
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mInfo.HeightmapWidth;
	texDesc.Height = mInfo.HeightmapHeight;
    texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// HALF is defined in xnamath.h, for storing 16-bit float.
	std::vector<HALF> hmap(mHeightMap.size());
	std::transform(mHeightMap.begin(), mHeightMap.end(), hmap.begin(), XMConvertFloatToHalf);
	
	// Set the initial data to be the heightmap.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hmap[0];
    data.SysMemPitch = mInfo.HeightmapWidth*sizeof(HALF);
    data.SysMemSlicePitch = 0;

	// Create the texture.
	ID3D11Texture2D* hmapTex = 0;
	HR(device->CreateTexture2D(&texDesc, &data, &hmapTex));

	// Create the SRV to the texture.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	HR(device->CreateShaderResourceView(hmapTex, &srvDesc, &mHeightMapSRV));

	// SRV saves reference.
	ReleaseCOM(hmapTex);
}

//XMMATRIX Terrain::GetWorld()
//{
//	return XMLoadFloat4x4(&mWorldMatrix);
//}
//	
//void Terrain::SetWorld(XMMATRIX world)
//{
//	XMStoreFloat4x4(&mWorldMatrix, world);
//}

InitInfo Terrain::GetInfo()
{
	return mInfo;
}

Primitive* Terrain::GetPrimitive()
{
	return mPrimitive;
}