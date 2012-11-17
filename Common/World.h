#pragma once
#include <vector>
#include <map>
#include <boost\function.hpp>
#include <boost\bind.hpp>
#include "d3dUtil.h"

using namespace std;

namespace GLib {
	// Forward declarations.
	class Graphics;
	class Light;
	class Sky;
	class BillboardManager;
	class Terrain;
	class Object3D;
	class WorldLoader;
	struct BillboardVertex;

	typedef std::vector<Object3D*> ObjectList;

	//! Contains all objects, lights and the terrain.
	class World
	{
	public:
		World();
		~World();

		virtual void Init(Graphics* pGraphics);
		virtual void Update(float dt);
		virtual void Draw(Graphics* pGraphics);

		void AddObject(Object3D* object);
		void AddLight(Light* pLight);
		void RemoveObject(Object3D* pObject);
		void RemoveLight(Light* pLight);

		LightList*	GetLights();
		ObjectList*			GetObjects();
		Terrain*		GetTerrain();
		int			GetNumLights();
		XMFLOAT3	GetTerrainIntersectPoint(Ray ray);
		Object3D*	GetSelectedObject(Ray ray);

		// Callback hookup.
		template <class T>
		void AddItemSelectedListender(void(T::*_callback)(void*, int), T* _object)	{
			OnItemSelected = boost::bind(_callback, _object, _1, _2);
		}
	private:
		// Callback.
		boost::function<void(void*, int)>	OnItemSelected;
	private:
		ObjectList		mObjectList;
		LightList	mLightList;
		WorldLoader*	mWorldLoader;
		Sky*		mSkyBox;
		Terrain*	mTerrain;
	};
}