//=========================================================================
//
//  GameParticles.hpp
//
//=========================================================================
#ifndef GAMEPARTICLES_HPP
#define GAMEPARTICLES_HPP

#include "ObjectMgr\Object.hpp"

class pfx_effect;
class pfx_emitter;



void SpawnDiskExplosion     ( const vector3& Point, const vector3& Normal, const pain& Pain );
void SpawnMortarExplosion   ( const vector3& Point, const vector3& Normal, const pain& Pain );
void SpawnGrenadeExplosion  ( const vector3& Point, const vector3& Normal, const pain& Pain );
void SpawnPlasmaExplosion   ( const vector3& Point, const vector3& Normal, const pain& Pain );
void SpawnLaserExplosion    ( const vector3& Point, const vector3& Normal, const pain& Pain );
void SpawnBulletExplosion   ( const vector3& Point, const vector3& Normal, const pain& Pain );
void SpawnBlasterExplosion  ( const vector3& Point, const vector3& Normal, const pain& Pain );

void InitDiskExplosion      ( pfx_effect& Effect );
void InitMortarExplosion    ( pfx_effect& Effect );
void InitGrenadeExplosion   ( pfx_effect& Effect );

void DiskExplosionCB        ( pfx_effect& Effect, pfx_emitter& Emitter );
void GrenadeExplosionCB     ( pfx_effect& Effect, pfx_emitter& Emitter );
void MortarExplosionCB      ( pfx_effect& Effect, pfx_emitter& Emitter );


#endif