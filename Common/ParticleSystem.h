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
		void Update(float dt);
		void Draw(Graphics* pGraphics);

		void SetSpawnFrequency(float frequency);
		void SetTime(float time);
		void SetLifetime(float lifetime);
		void SetNumMaxParticles(int numMaxParticles);
		
		void AddParticle();

		float GetTime();
		void Init() {};
		bool RayIntersect(XMVECTOR origin, XMVECTOR direction, float& pDist) {return false;}
		AxisAlignedBox GetBoundingBox() {return AxisAlignedBox();}
	protected:
		LuaWrapper*		 mLuaWrapper;
		ParticleEffect*	 mEffect;
		string			 mTextureName;
		string			 mLuaScript;
		float mTime;
		float mLifetime;
		float mSpawnFrequency;
		int	  mNumMaxParticles;

		std::vector<Particle*>	mParticles;
		std::vector<Particle*>	mAliveParticles;
		std::vector<Particle*>	mDeadParticles; 
	};

}