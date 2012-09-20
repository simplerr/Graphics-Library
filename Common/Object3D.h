#pragma once

#include <Windows.h>
#include <xnamath.h>
#include "d3dx11Effect.h"
#include "Light.h"
#include "xnacollision.h"

using namespace XNA;

class Graphics;
class BasicEffect;
class Primitive;
class Model;
class ModelImporter;
struct Texture2D;

/**
	Base class for all objects.
	Contains a primitive, effect and position information.
*/
class Object3D
{
public:
	Object3D(ModelImporter* importer, string filename);
	Object3D(Primitive* primitive);
	~Object3D();

	void Update(float dt);
	void Draw(Graphics* pGraphics);

	// Getters & setters.
	Model*	 GetModel();
	XMMATRIX GetWorldMatrix();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();
	Material GetMaterial();
	AxisAlignedBox GetBoundingBox();
	
	void SetModel(Model* model);
	void SetTexture(string filename, float scale = 1.0f);
	void SetNormalMap(string filename);
	void SetTexture(Texture2D* texture);
	void SetNormalMap(Texture2D* normalMap);
	void SetPosition(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);
	void SetScale(XMFLOAT3 scale);
	void SetMaterial(Material material);
	void SetDefaultOrientation();
private:
	Model*		mModel;
	Texture2D*	mNormalMap;
	XMFLOAT3	mPosition;
	XMFLOAT3	mRotation;
	XMFLOAT3	mScale;
};