#pragma once

#include "d3dUtil.h"
#include "Vertex.h"
#include <vector>
#include <string>
#include "Object3D.h"

using namespace std;

class LuaWrapper;

namespace GLib {
	class Object3D;
	class ParticleEffect;
	class Texture2D;
	class Graphics;
	class Particle;

	class ParticleSystem : public GLib::Object3D
	{
	public:
		ParticleSystem(XMFLOAT3 position, string luaScript);
		virtual ~ParticleSystem();

		Particle* CreateParticle();
		void InitLua();
		virtual void Update(float dt);
		virtual void Draw(Graphics* pGraphics);

		void SetSpawnFrequency(float frequency);
		void SetTime(float time);
		void SetLifetime(float lifetime);
		void SetNumMaxParticles(int numMaxParticles);
		void SetOriginObject(Object3D* pObject);
		void SetRadius(float radius);
		
		void AddParticle();

		float GetTime();
		void Init() {};
		bool RayIntersect(XMVECTOR origin, XMVECTOR direction, float& pDist) {return false;}
		virtual AxisAlignedBox GetBoundingBox();
		Particle* GetParticle(int id);
	protected:
		LuaWrapper*		 mLuaWrapper;
		ParticleEffect*	 mEffect;
		Object3D*		 mOriginObject;
		string			 mTextureName;
		string			 mLuaScript;
		float mTime;
		float mLifetime;
		float mSpawnFrequency;
		float mRadius;
		int	  mNumMaxParticles;

		std::vector<Particle*>	mParticles;
		std::vector<Particle*>	mAliveParticles;
		std::vector<Particle*>	mDeadParticles; 
	};

}