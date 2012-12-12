#include <cassert>
#include "ParticleSystem.h"
#include "Graphics.h"
#include "Camera.h"
#include "d3dUtil.h"
#include "Effects.h"

namespace GLib {

ParticleSystem::ParticleSystem(XMFLOAT3 position, string texture, int numMaxParticles)
	: Object3D(PARTICLE_SYSTEM, position),
	mTime(0.0f),
	mTextureName(texture),
	mNumMaxParticles(numMaxParticles)
{
	// Defaults.
	SetAccel(XMFLOAT3(0, -100, 0));
	SetSpawnFrequency(0.1f);
	SetLifetime(5.0f);

	// Create the texture.
	mTexture = GLib::GetGraphics()->LoadTexture(texture);

	// Allocate memory for maximum number of particles.
	mParticles.resize(mNumMaxParticles);
	mAliveParticles.reserve(mNumMaxParticles);
	mDeadParticles.reserve(mNumMaxParticles);

	// They start off all dead.
	for(int i = 0; i < mNumMaxParticles; ++i)
	{
		mParticles[i].lifeTime = -1.0f;
		mParticles[i].initialTime = 0.0f;
		mParticles[i].billboard = GLib::GetGraphics()->AddBillboard(XMFLOAT3(0, 0, 0), XMFLOAT2(1, 1), texture);

		// The billboards position and size will be updated in ParticleSystem::Update().
	}
}

ParticleSystem::~ParticleSystem()
{
	// Remove all the billboards.
	for(int i = 0; i < mNumMaxParticles; ++i)
		GLib::GetGraphics()->RemoveBillboard(mTextureName, mParticles[i].billboard);
}

float ParticleSystem::GetTime()
{
	return mTime;
}

void  ParticleSystem::SetLifetime(float lifetime)
{
	mLifetime = lifetime;
}

void ParticleSystem::SetTime(float time)
{
	mTime = time;
}

void ParticleSystem::AddParticle()
{
	if(mDeadParticles.size() > 0)
	{
		// Reinitialize a particle.
		Particle* p = mDeadParticles.back();
		p->initialTime = mTime;
		InitParticle(*p);

		// No longer dead.
		mDeadParticles.pop_back();
		mAliveParticles.push_back(p);
	}
}

void ParticleSystem::Update(float dt)
{
	mTime += dt;

	if(mTime >= mLifetime)
		Kill();

	mDeadParticles.resize(0);
	mAliveParticles.resize(0);

	// For each particle.
	for(int i = 0; i < mNumMaxParticles; ++i)
	{
		// Is the particle dead?
		if( (mTime - mParticles[i].initialTime) > mParticles[i].lifeTime)
		{
			mDeadParticles.push_back(&mParticles[i]);

			// [HACK] Sets the position outside vision.
			float infinity = numeric_limits<float>::infinity();
			mParticles[i].billboard->SetPos(XMFLOAT3(infinity, infinity, infinity));
		}
		else
		{
			mAliveParticles.push_back(&mParticles[i]);

			// [TODO] Run LUA script here.

			float age = mTime - mParticles[i].initialTime;
			mParticles[i].billboard->SetPos(mParticles[i].initialPos + mParticles[i].initialVelocity * age + 0.5f * mAccel * age * age);
			float size = mParticles[i].initialSize * sinf(age * 6);
			mParticles[i].billboard->SetSize(XMFLOAT2(size, size));
		}
	}

	// Emit particles.
	static float timeAccum = 0.0f;
	timeAccum += dt;
	while(timeAccum >= mSpawnFrequency)
	{
		AddParticle();
		timeAccum -= mSpawnFrequency;
	}
}

void ParticleSystem::SetAccel(XMFLOAT3 accel)
{
	mAccel = accel;
}

void ParticleSystem::SetSpawnFrequency(float frequency)
{
	mSpawnFrequency = frequency;
}

void ParticleSystem::Draw(Graphics* pGraphics)
{
	// Graphics::DrawBillboards() does the drawing.
}

}