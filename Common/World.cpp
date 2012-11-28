#include "World.h"
#include "Object3D.h"
#include "Light.h"
#include "PrimitiveFactory.h"
#include "Runnable.h"
#include "Effects.h"
#include "Graphics.h"
#include "xnacollision.h"
#include "Camera.h"
#include "Sky.h"
#include "Terrain.h"
#include "StaticModel.h"
#include "BillboardManager.h"
#include "Input.h"
#include "Effects.h"
#include "WorldLoader.h"
#include <limits>

//! Graphics Library namespace.
namespace GLib
{
	//! Constructor.
	World::World()
	{
		
	}

	//! Init.
	void World::Init(Graphics* pGraphics)
	{
		// Create the sky box.
		mSkyBox = new Sky(pGraphics, "textures/sunsetcube1024.dds", 1000.0f);

		// Create the terrain.
		mTerrain = new Terrain();

		// Create the world loader.
		mWorldLoader = new WorldLoader();
		mWorldLoader->LoadWorld(this, mTerrain, "world.wmap");
	}

	//! Cleanup the object and light lists.
	World::~World()
	{
		//mWorldLoader->SaveWorld(this, mTerrain, "world.wmap");

		delete mWorldLoader;
		delete mSkyBox;
		delete mTerrain;

		// Delete all objects.
		for(int i = 0; i < mObjectList.size(); i++)
			delete mObjectList[i];

		// Delete all lights.
		for(int i = 0; i < mLightList.size(); i++)
			delete mLightList[i];
	}

	//! Updates all objects.
	void World::Update(float dt)
	{
		// Loop through all objects.
		for(int i = 0; i < mObjectList.size(); i++)
		{
			// Update the object if alive.
			if(mObjectList[i]->GetAlive())
				mObjectList[i]->Update(dt);
			else { // Remove if dead. 
				RemoveObject(mObjectList[i]);
			}
		}

		HandleCollisions();
	}

	//! Test if any objects are intersecting.
	void World::HandleCollisions()
	{
		// Loop through all objects.
		for(int i = 0; i < mObjectList.size(); i++)
		{
			for(int j = i+1; j < mObjectList.size(); j++)
			{
				XMFLOAT3 diff = mObjectList[i]->GetPosition() - mObjectList[j]->GetPosition();
				float dist = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

				// Collision! [NOTE] Fix proper collision detection.
				if(dist < 4.0f && !OnObjectCollision.empty())
					OnObjectCollision(mObjectList[i], mObjectList[j]);
			}
		}
	}

	//! Draws all objects.
	void World::Draw(Graphics* pGraphics)
	{
		// Draw all objects to the shadow map.
		pGraphics->ActiveShadowMap();
		for(int i = 0; i < mObjectList.size(); i++)
			mObjectList[i]->Draw(pGraphics);
		pGraphics->DeactiveShadowMap();

		// Draw the terrain.
		mTerrain->Draw(pGraphics);

		// Draw the skybox. [TODO] if(mDrawSkyBox)
		//mSkyBox->Draw(pGraphics);

		// Draw all the objects.
		for(int i = 0; i < mObjectList.size(); i++)
			mObjectList[i]->Draw(pGraphics);

		// Draw the lights with a billboard.
		pGraphics->DrawBillboards();
	}

	//! Returns the selected object, if any.
	Object3D* World::GetSelectedObject(Ray ray, ObjectType type)
	{
		// Loop through all objects.
		float closestDist = numeric_limits<float>::infinity();
		Object3D* closestObject = nullptr;
		for(int i = 0; i < mObjectList.size(); i++)
		{
			// Only selected object with specified type.
			if(mObjectList[i]->GetType() != type && type != ANY)
				continue;

			float dist;
			XMFLOAT3 pos = GetCamera()->GetPosition();
			XMFLOAT3 dir = ray.direction;
			if(XNA::IntersectRayAxisAlignedBox(XMLoadFloat3(&pos), XMLoadFloat3(&dir), &mObjectList[i]->GetBoundingBox(), &dist)) 
			{
				if(mObjectList[i]->RayIntersect(XMLoadFloat3(&pos), XMLoadFloat3(&dir), dist))
				{
					if(dist < closestDist) {
						closestObject = mObjectList[i];
						closestDist = dist;
					}
				}
			}
		}

		return closestObject;
	}

	//! Adds an object to the object list.
	void World::AddObject(Object3D* object)
	{
		// [TODO] Set Id.
		static int id = 0;
		object->SetId(id++);
		object->SetWorld(this);
		object->Init();
		mObjectList.push_back(object);
	}

	//! Adds a light to the light list.
	void World::AddLight(Light* light)
	{
		static int id = 0;
		light->SetId(id++);
		mLightList.push_back(light);
	}

	//! Removes an object from the list.
	void World::RemoveObject(Object3D* pObject)
	{
		RemoveObject(pObject->GetId());
	}

	void World::RemoveObject(int id)
	{
		// Loop through all objects and find out which one to delete.
		int i = 0;
		auto itr =  mObjectList.begin();
		while(itr != mObjectList.end() && i < mObjectList.size())
		{
			if(mObjectList[i]->GetId() == id)
			{
				// Callback.
				if(!OnObjectRemoved.empty())
					OnObjectRemoved(mObjectList[i]);

				delete mObjectList[i];		
				mObjectList[i] = NULL;
				itr = mObjectList.erase(itr);	
				break;
			}
			else	{
				itr++;
				i++;
			}
		}
	}

	//! Removes a light from the light list.
	void World::RemoveLight(Light* pLight)
	{
		// Loop through all objects and find out which one to delete.
		int i = 0;
		auto itr =  mLightList.begin();
		while(itr != mLightList.end() && i < mLightList.size())
		{
			if(mLightList[i]->GetId() == pLight->GetId())
			{
				delete mLightList[i];		
				mLightList[i] = NULL;
				itr = mLightList.erase(itr);	
				break;
			}
			else	{
				itr++;
				i++;
			}
		}
	}

	int World::GetNumObjects(ObjectType type)
	{
		int num = 0;
		for(int i = 0; i < mObjectList.size(); i++)
		{
			if(mObjectList[i]->GetType() == type)
				num++;
		}

		return num;
	}

	//! Returns the address to the light list.
	LightList* World::GetLights()
	{
		return &mLightList;
	}

	//! Returns the address to the object list.
	ObjectList* World::GetObjects()
	{
		return &mObjectList;
	}

	//! Returns a pointer to the terrain.
	Terrain* World::GetTerrain()
	{
		return mTerrain;
	}

	//! Returns the number of light in the world.
	int World::GetNumLights()
	{
		return mLightList.size();
	}

	//! Returns the intersect point with the terrain.
	XMFLOAT3 World::GetTerrainIntersectPoint(Ray ray)
	{
		return mTerrain->GetIntersectPoint(ray);
	}

	Object3D* World::GetObjectById(int id)
	{
		for(int i = 0; i < mObjectList.size(); i++)
		{
			if(mObjectList[i]->GetId() == id)
				return mObjectList[i];
		}

		return nullptr;
	}
}	// End of Graphics Library namespace.