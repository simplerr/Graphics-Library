#pragma once

#include "d3dUtil.h"
#include "Vertex.h"
#include <vector>
#include <string>
#include "Object3D.h"
using namespace std;

namespace GLib {
	class Object3D;
	class ParticleEffect;
	class Texture2D;
	class Graphics;

	//! Particle vertex.
	struct Particle
	{
		BillboardVertex* billboard;
		XMFLOAT3	initialPos;
		XMFLOAT3	initialVelocity;
		float       initialSize; // In pixels.
		float       initialTime;
		float       lifeTime;
		float       mass;
		XMFLOAT4    initialColor;
	};

	class ParticleSystem : public GLib::Object3D
	{
	public:
		ParticleSystem(XMFLOAT3 position, string texture, int numMaxParticles);
		virtual ~ParticleSystem();

		virtual void InitParticle(Particle& out) = 0;
		void Update(float dt);
		void Draw(Graphics* pGraphics);

		void SetAccel(XMFLOAT3 accel);
		void SetSpawnFrequency(float frequency);
		void SetTime(float time);
		void SetLifetime(float lifetime);
		
		void AddParticle();

		float GetTime();
		void Init() {};
		bool RayIntersect(XMVECTOR origin, XMVECTOR direction, float& pDist) {return false;}
		AxisAlignedBox GetBoundingBox() {return AxisAlignedBox();}
	protected:
		ParticleEffect*	 mEffect;
		GLib::Texture2D* mTexture;
		XMFLOAT3		 mAccel;
		string			 mTextureName;
		float mTime;
		float mLifetime;
		float mSpawnFrequency;
		int	  mNumMaxParticles;

		std::vector<Particle>	mParticles;
		std::vector<Particle*>	mAliveParticles;
		std::vector<Particle*>	mDeadParticles; 
	};

}