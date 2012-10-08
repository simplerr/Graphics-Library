#include "Object3D.h"
#include "Graphics.h"
#include "Effects.h"
#include "Runnable.h"
#include "Primitive.h"
#include "PrimitiveFactory.h"
#include "D3DCore.h"
#include "Model.h"
#include "Mesh.h"
#include "ModelImporter.h"

Object3D::Object3D(ModelImporter* importer, string filename)
{
	mNormalMap	= nullptr;
	mModel = importer->LoadModel(filename);

	SetDefaultOrientation();
}

Object3D::Object3D(Primitive* primitive)
{
	mNormalMap	= nullptr;
	mModel		= new Model();
	Mesh* mesh	= new Mesh();
	mesh->SetPrimitive(primitive);
	mesh->SetMaterial(Material(Colors::White));
	mModel->AddMesh(mesh);

	SetDefaultOrientation();
	SetMaterial(Material(Colors::White));
}

Object3D::~Object3D()
{
	mModel->Cleanup();
	delete mModel;
}

void Object3D::Update(float dt)
{

}
	
//! Draws the object.
void Object3D::Draw(Graphics* pGraphics)
{
	assert(mModel);

	// Draw the model.
	mModel->Draw(pGraphics, GetWorldMatrix());

	//pGraphics->DrawBoundingBox(&GetBoundingBox(), GetWorldMatrix(), Material(Colors::Blue));
}

//! Loads the texture to pass to the shader.
//! [NOTE] This only sets the texture in the first mesh, 
//! it's not intended to be used with a model loaded from a file.
void Object3D::SetTexture(string filename, float scale)
{
	assert(mModel);

	Texture2D* tex = gGame->GetGraphics()->LoadTexture(filename, scale);
	mModel->GetMeshList()->operator[](0)->SetTexture(tex);
}

//! Sets the model.
void Object3D::SetModel(Model* model)
{
	mModel = model;
}

//! Sets the texture to pass to the shader.
void Object3D::SetTexture(Texture2D* texture)
{
	assert(mModel);
	mModel->GetMeshList()->operator[](0)->SetTexture(texture);
}

void Object3D::SetNormalMap(string filename)
{
	mNormalMap = gGame->GetGraphics()->LoadTexture(filename);
}
	
void Object3D::SetNormalMap(Texture2D* normalMap)
{
	mNormalMap = normalMap;
}

//! Sets the objects material.
void Object3D::SetMaterial(Material material)
{
	assert(mModel);

	// Loop through all meshes and set their material.
	MeshList* meshList = mModel->GetMeshList();
	for(int i = 0; i < meshList->size(); i++)
		(*meshList)[i]->SetMaterial(material);
}

//! Sets the position.
void Object3D::SetPosition(XMFLOAT3 position)
{
	mPosition = position;
}
	
//! Sets the rotation.
void Object3D::SetRotation(XMFLOAT3 rotation)
{
	mRotation = rotation;
}
	
//! Sets the scale.
void Object3D::SetScale(XMFLOAT3 scale)
{
	mScale = scale;
}

//! Sets the default orientation.
void Object3D::SetDefaultOrientation()
{
	SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
}

//! Returns the world matrix.
XMMATRIX Object3D::GetWorldMatrix()
{
	XMMATRIX T, R, S, W;
	T = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	R = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
	S = XMMatrixScaling(mScale.x, mScale.y, mScale.z);

	return S*R*T;
}

//! Returns the model.
Model* Object3D::GetModel()
{
	return mModel;
}

//! Returns the material used in the first mesh.
Material Object3D::GetMaterial()
{
	assert(mModel);

	// Pretend that all meshes in the model have the same material.
	return (*mModel->GetMeshList())[0]->GetMaterial();
}

//! Returns the position.
XMFLOAT3 Object3D::GetPosition()
{
	return mPosition;
}

//! Returns the rotation.
XMFLOAT3 Object3D::GetRotation()
{
	return mRotation;
}
	
//! Returns the scale.
XMFLOAT3 Object3D::GetScale()
{
	return mScale;
}

//! Returns the bounding box in world space.
AxisAlignedBox Object3D::GetBoundingBox()
{
	assert(mModel);

	MeshList* meshList = mModel->GetMeshList();
	AxisAlignedBox aabb = (*meshList)[0]->GetPrimitive()->GetBoundingBox();
	XMFLOAT3 min = aabb.Center - aabb.Extents;
	XMFLOAT3 max = aabb.Center + aabb.Extents;
	for(int i = 1; i < meshList->size(); i++)
	{
		AxisAlignedBox meshAABB = (*meshList)[i]->GetPrimitive()->GetBoundingBox();
		XMFLOAT3 meshMin = meshAABB.Center - meshAABB.Extents;
		XMFLOAT3 meshMax = meshAABB.Center + meshAABB.Extents;

		XMStoreFloat3(&min, XMVectorMin(XMLoadFloat3(&meshMin), XMLoadFloat3(&min)));
		XMStoreFloat3(&max, XMVectorMax(XMLoadFloat3(&meshMax), XMLoadFloat3(&max)));
	}

	aabb.Center =  (min + max) * 0.5f;
	aabb.Extents = (max - min) * 0.5f;

	// Break up the world matrix into it's components.
	XMVECTOR scale, rotation, translation;
	XMMatrixDecompose(&scale, &rotation, &translation, GetWorldMatrix());

	// Transform the AABB with the components.
	TransformAxisAlignedBoxCustom(&aabb, &aabb, scale, rotation, translation);

	return aabb;
}