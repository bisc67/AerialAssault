//==============================================================================
//
//  GameParticles.cppp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "..\Support\Objects\Projectiles\ParticleObject.hpp"
#include "..\Support\Particles\ParticleEffect.hpp"
#include "Entropy.hpp"
#include "..\AADS\Globals.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"

#include "..\AADS\GameParticles.hpp"

//==============================================================================
//  DEFINES
//==============================================================================
#define MORTAR_OFFSET       2.0f
#define DISK_OFFSET         1.0f

//==============================================================================
//  FUNCTIONS
//==============================================================================

//==============================================================================
// Spawn a Disk explosion for all to see

void SpawnDiskExplosion( const vector3& Point, const vector3& Normal, const pain& Pain )
{
    particle_object* pExplosion = (particle_object*)ObjMgr.CreateObject( object::TYPE_PARTICLE_EFFECT );
    pExplosion->Initialize( Point, Normal, PARTICLE_TYPE_EXP_DISC, Pain );
    ObjMgr.AddObject( pExplosion, obj_mgr::AVAILABLE_SERVER_SLOT );
}

//==============================================================================
// Spawn a Mortar explosion for all to see

void SpawnMortarExplosion( const vector3& Point, const vector3& Normal, const pain& Pain )
{
    particle_object* pExplosion = (particle_object*)ObjMgr.CreateObject( object::TYPE_PARTICLE_EFFECT );
    pExplosion->Initialize( Point, Normal, PARTICLE_TYPE_EXP_MORTAR, Pain );
    ObjMgr.AddObject( pExplosion, obj_mgr::AVAILABLE_SERVER_SLOT );
}

//==============================================================================
// Spawn a Grenade explosion for all to see

void SpawnGrenadeExplosion( const vector3& Point, const vector3& Normal, const pain& Pain )
{
    particle_object* pExplosion = (particle_object*)ObjMgr.CreateObject( object::TYPE_PARTICLE_EFFECT );
    pExplosion->Initialize( Point, Normal, PARTICLE_TYPE_EXP_GRENADE, Pain );
    ObjMgr.AddObject( pExplosion, obj_mgr::AVAILABLE_SERVER_SLOT );
}

//==============================================================================
// Spawn a Plasma explosion for all to see

void SpawnPlasmaExplosion( const vector3& Point, const vector3& Normal, const pain& Pain )
{
    particle_object* pExplosion = (particle_object*)ObjMgr.CreateObject( object::TYPE_PARTICLE_EFFECT );
    pExplosion->Initialize( Point, Normal, PARTICLE_TYPE_EXP_PLASMA, Pain );
    ObjMgr.AddObject( pExplosion, obj_mgr::AVAILABLE_SERVER_SLOT );
}

//==============================================================================
// Spawn a Laser effect for all to see

void SpawnLaserExplosion( const vector3& Point, const vector3& Normal, const pain& Pain )
{
    particle_object* pExplosion = (particle_object*)ObjMgr.CreateObject( object::TYPE_PARTICLE_EFFECT );
    pExplosion->Initialize( Point, Normal, PARTICLE_TYPE_EXP_LASER, Pain );
    ObjMgr.AddObject( pExplosion, obj_mgr::AVAILABLE_SERVER_SLOT );
}

//==============================================================================
// Spawn a Bullet effect for all to see

void SpawnBulletExplosion( const vector3& Point, const vector3& Normal, const pain& Pain )
{
    particle_object* pExplosion = (particle_object*)ObjMgr.CreateObject( object::TYPE_PARTICLE_EFFECT );
    pExplosion->Initialize( Point, Normal, PARTICLE_TYPE_EXP_BULLET, Pain );
    ObjMgr.AddObject( pExplosion, obj_mgr::AVAILABLE_SERVER_SLOT );
}

//==============================================================================
// Spawn a Blaster effect for all to see

void SpawnBlasterExplosion( const vector3& Point, const vector3& Normal, const pain& Pain )
{
    particle_object* pExplosion = (particle_object*)ObjMgr.CreateObject( object::TYPE_PARTICLE_EFFECT );
    pExplosion->Initialize( Point, Normal, PARTICLE_TYPE_EXP_BLASTER, Pain );
    ObjMgr.AddObject( pExplosion, obj_mgr::AVAILABLE_SERVER_SLOT );
}
//==============================================================================

void InitDiskExplosion( pfx_effect& Effect )
{
    s32 nEmit = Effect.GetNumEmitters();
    s32 i;

    // create a random rotation for the mortar explosion, and offset it a random amount
    for ( i = 0; i < nEmit; i++ )
    {
        f32 rot;
        //vector3 pos;

        pfx_emitter& Emitter = Effect.GetEmitter(i);

        if ( Emitter.GetType() == pfx_emitter_def::SHAPE )
        {
            rot = x_frand(0.0f, 2*PI);

            Emitter.SetRotation( rot );

            //pos = Emitter.GetPosition();
            //pos.X += x_frand(-MORTAR_OFFSET, DISK_OFFSET);
            //pos.Y += x_frand(-MORTAR_OFFSET, DISK_OFFSET);
            //pos.Z += x_frand(-MORTAR_OFFSET, DISK_OFFSET);
            //Emitter.SetPosition( pos );
        }
    }        
}

//==============================================================================

void InitGrenadeExplosion( pfx_effect& Effect )
{
    s32 nEmit = Effect.GetNumEmitters();
    s32 i;

    // nobody should mess with the grenade explosion definition
    ASSERT( nEmit > 2 );
    vector3 Normal = Effect.GetNormal();
    plane   P;
    P.Setup( Effect.GetPosition(), Effect.GetNormal() );
    vector3 A,B;

    P.GetOrthoVectors( A, B );

    // determine angular range per tendril
    s32 nTendrils = nEmit - 2;
    ASSERT( nTendrils > 0 );

    f32 AngStart = ((2 * PI) / nTendrils);
    f32 AngRange = AngStart / 2.0f;

    // create a random rotation for the mortar explosion, and offset it a random amount
    for ( i = 2; i < nEmit; i++ )
    {
        radian Pitch;
        radian Yaw;

        Pitch = x_frand( -R_70, -R_20 );
        Yaw = x_frand( AngStart * i, AngStart * i + AngRange );
        vector3 Vel( Pitch, Yaw );

        f32 speed = x_frand( 20.0f, 30.0f );

        Vel *= speed;

        pfx_emitter& Emitter = Effect.GetEmitter(i);

        // rotate the vector into plane space
        vector3 NewVel = ( A * Vel.X ) + ( Normal * Vel.Y ) + ( B * Vel.Z );
        vector3 Decel = NewVel * -0.1f;

        Emitter.SetVelocity( NewVel );
        Emitter.SetAcceleration( vector3( 0.0f, -19.0f, 0.0f ) + Decel );
    } 
}

//==============================================================================

void InitMortarExplosion( pfx_effect& Effect )
{
    s32 nEmit = Effect.GetNumEmitters();
    s32 i;

    // create a random rotation for the mortar explosion, and offset it a random amount
    for ( i = 0; i < nEmit; i++ )
    {
        f32 rot;
        vector3 pos;

        pfx_emitter& Emitter = Effect.GetEmitter(i);

        rot = x_frand(0.0f, 2*PI);

        Emitter.SetRotation( rot );

        pos = Emitter.GetPosition();
        pos.X += x_frand(-MORTAR_OFFSET, MORTAR_OFFSET);
        pos.Y += x_frand(-MORTAR_OFFSET, MORTAR_OFFSET);
        pos.Z += x_frand(-MORTAR_OFFSET, MORTAR_OFFSET);
        //Emitter.SetPosition( pos );
    }        
}

//==============================================================================

void DiskExplosionCB( pfx_effect& Effect, pfx_emitter& Emitter )
{
    (void)Effect;
    (void)Emitter;
}

//==============================================================================

void GrenadeExplosionCB( pfx_effect& Effect, pfx_emitter& Emitter )
{
    (void)Effect;
    (void)Emitter;
}

//==============================================================================

void MortarExplosionCB( pfx_effect& Effect, pfx_emitter& Emitter )
{
    (void)Effect;
    (void)Emitter;
}

