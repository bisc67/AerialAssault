//==============================================================================
//
//  Contrail.hpp
//
//==============================================================================

#ifndef __CONTRAIL_HPP
#define __CONTRAIL_HPP

//==============================================================================
//  DEFINES
//==============================================================================

#define MAXNODES            20
#define SEGLEN              2.0f
#define SEGLEN2             ( SEGLEN * SEGLEN )
#define MAXLIFE             1.0f
#define MAX_OPACITY         64
#define MAX_WIDTH           0.5f
#define MIN_WIDTH           0.1f

//==============================================================================
//  TYPES
//==============================================================================

class contrail
{

public:
//------------------------------------------------------------------------------
//  Public Functions
//------------------------------------------------------------------------------
    // simple ctor      
    contrail()                          { m_CurNode = 0; }
                                        
    void                AddNode         ( const vector3& NewNode, f32 Alpha = 1.0f );
    void                Update          ( f32 DeltaTime );
    void                Render          ( const vector3& CurPos, xbool Connect = TRUE );
                                        
private:                                

//------------------------------------------------------------------------------
//  Private functions
//------------------------------------------------------------------------------
                                        
    void                NextNode        ( void );       // inc the curnode, cycle
    s32                 GetPrevNodeIdx  ( s32 Idx );    // decrement Idx, w/cycle

    xbool               AnyActiveNodes  ( void );       // any still living?

//------------------------------------------------------------------------------
//  Private types
//------------------------------------------------------------------------------
    struct node         
    {                   
        vector3         m_Node ;                    // the location of the node
        f32             m_Age ;                     // the age of the node
        xbool           m_Alive ;                   // is the node still active
        f32             m_MaxAlpha ;                // each node has it's own now
                        
        node()          { m_Age = 0.0f; m_Alive = FALSE; }      // ctor
    };                  
                        
//------------------------------------------------------------------------------
//  Private data
//------------------------------------------------------------------------------
    node                m_Nodes[ MAXNODES ] ;       // node storage
    s32                 m_CurNode;                  // the active node                        

};    



#endif