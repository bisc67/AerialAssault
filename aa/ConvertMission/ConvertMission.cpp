//==============================================================================
//
//  ConvertMission
//
//==============================================================================

//
// NOTES:
//
//  - Added code to extract rotations of camera points, but values look to be
//    wrong.
//
//  - Teams are exported as 0, 1, and -1.
//

//
// GENERAL OUTPUT FORMAT: 
/*

------------ TT SS PP   +####.####   +####.####   +####.####   +####.####   +####.####   +####.####

LABEL        %2d    %2d   %10.4f   %10.4f   %10.4f        TEAM        POWER

LABEL           %2d %2d   %10.4f   %10.4f   %10.4f             SWITCH POWER

LABEL        %2d         %10.4f   %10.4f   %10.4f        TEAM        

LABEL           %2d      %10.4f   %10.4f   %10.4f              SWITCH

LABEL              %2d   %10.4f   %10.4f   %10.4f                     POWER

LABEL                   %10.4f   %10.4f   %10.4f

*/

//==============================================================================
//  INCLUDES
//==============================================================================

#include "x_files.hpp"
#include "..\Support\Tokenizer\Tokenizer.hpp"

//==============================================================================
//  TYPES
//==============================================================================

template <class T>
class xptrarray : public xarray<T>
{
public:
    ~xptrarray( void ) 
    {   
        while( m_Count > 0 )
        {
            m_Count--;
            delete m_pData[ m_Count ];
        }
    };
};

//==============================================================================

class attribute
{
public:
        xstring                 Name;
        xstring                 Value;

                               ~attribute   ( void ) { };
        void                    Dump        ( X_FILE* pFile, s32 Indent );                
static  attribute*              Read        ( token_stream& Tok );

        xcolor                  GetColor    ( void );
        vector3                 GetPosition ( void );
        vector3                 GetDirection( void );
        vector3                 GetScale    ( void );
        radian3                 GetRotation ( void );
        f32                     GetFloat    ( void );
        vector3                 Get3Floats  ( void );
        vector4                 Get4Floats  ( void );
};

//==============================================================================

class item
{
public:
        xstring                 Name;
        xstring                 Type;
        xptrarray<attribute*>   Attributes;
        xptrarray<item*>        Items;
        s32                     PowerCircuit;
        s32                     SwitchCircuit;
        s32                     StringIndex;

                               ~item        ( void ) { };
        void                    Dump        ( X_FILE* pFile, s32 Indent );                
static  item*                   Read        ( token_stream& Tok );
        attribute*              FindAttr    ( const char* pName );
        void                    AddString   ( const xstring& String );

        xcolor                  GetColor    ( const char* pName );
        vector3                 GetPosition ( const char* pName );
        vector3                 GetDirection( const char* pName );
        vector3                 GetScale    ( const char* pName );
        radian3                 GetRotation ( const char* pName );
        f32                     GetFloat    ( const char* pName );
        vector3                 Get3Floats  ( const char* pName );
        vector4                 Get4Floats  ( const char* pName );
        
        void                    Fixup       ( void );
};

//==============================================================================

typedef xstring item_fn( item* pItem );

//==============================================================================

struct game_type_info
{
    char*   pFileName;
    char*   pString;
    char*   pGameTypeString;
    xbool   TeamBased;
    xbool   Active;
};

//==============================================================================
//  GLOBAL STORAGE
//==============================================================================

token_stream    Tok;
char*           pInputFileName;
X_FILE*         pFile = NULL;

item*           pRootItem = NULL;

xarray<item*>   Stack;

s32             NPowerCircuits  = 1;
s32             PowerSupply[16] = { 1 };

s32             NSwitchCircuits = 4;
s32             SwitchBits[16]  = { -1, 1, 2, 0,
                                    -1,-1,-1,-1,
                                    -1,-1,-1,-1,
                                    -1,-1,-1,-1 };

char            MissionName[256];
s32             GameType;

const s32       MaxComments = 64;
s32             NumComments = -1;
char            Comment[ MaxComments ][ 1024 ];

xarray<xstring> StringTable;

game_type_info  GameTypeInfo[] = 
{               
    // FileName          String       GameTypeString    TeamBased   Active
    "Mission.txt",       "---",       "???",            TRUE,       TRUE,   // Used for lighting.
    "CTF-Mission.txt",   "CTF",       "CTF",            TRUE,       FALSE,
    "CNH-Mission.txt",   "CNH",       "CNH",            TRUE,       FALSE,
    "TDM-Mission.txt",   "TDM",       "TDM",            TRUE,       FALSE,
    "DM-Mission.txt",    "DM",        "DM",             FALSE,      FALSE,
    "HUNT-Mission.txt",  "HUNTERS",   "HUNTER",         FALSE,      FALSE,
    "SOLO-Mission.txt",  "CAMPAIGN",  "CAMPAIGN",       TRUE,       FALSE,
};

enum
{
    EXPORT,  CTF,  CNH,  TDM,  DM,  HUNTER,  CAMPAIGN, 
};

//==============================================================================

char* BuildingList[] =
{                     
    "BBASE1",  
    "BBASE4CM",
    "BBASE6",  
    "BBASE7",  
    "BBASE9",  
    "BBRDG0",  
    "BBRDG1",  
    "BBRDG2",  
    "BBRDG8",  
    "BBRDG9",  
    "BBRDGA",  
    "BBRDGB",  
    "BBRDGN",  
    "BBRDGO",  
    "BBUNK1",  
    "BBUNK2",  
    "BBUNK5",  
    "BBUNK7",  
    "BBUNK8",  
    "BBUNK9",  
    "BBUNKB",  
    "BBUNKC",  
    "BBUNKD",
    "BWALL1",
    "BWALL2",
    "BWALL3",
    "BWALL4",
    "BMISC1",  
    "BMISC2",  
    "BMISC3",  
    "BMISC4",  
    "BMISC6",  
    "BMISC7",  
    "BMISC8",  
    "BMISC9",  
    "BPLAT1",  
    "BPLAT2",  
    "BPLAT3",  
    "BPLAT6",  
    "BPOWER1",
    "BTOWR2",  
    "BTOWR5",  
    "BTOWR6",  
    "BTOWR8",  
    "BTOWRA",  
    "BVPAD",   
    "DBASE2",  
    "DBASE3",  
    "DBASE4",  
    "DBRDG10", 
    "DMISC1",  
    "DPLAT1",  
    "DPLAT3",  
    "DTOWR1",  
    "DTOWR4",  
    "DVENT",   
    "DVPAD",   
    "DVPAD1",  
    "PBASE3",  
    "PBRDG1",  
    "PBRDG3",  
    "PBRDG4",
    "PBRDGN",
    "PBRDGO",
    "PBUNK1",
    "PBUNK2",
    "PBUNK3",
    "PBUNK5",
    "PBUNK6",
    "PBUNK7",
    "PBUNK8",
    "PMISC1",
    "PMISC2",
    "PMISC3",
    "PMISC4",
    "PPLAT1",
    "PPLAT2",
    "PPLAT3",
    "PPLAT4",
    "PPLAT5",
    "PTOWR1",  
    "PTOWR2",  
    "PTOWR4",  
    "PVBAY1",  
    "PVPAD",   
    "SBASE1",  
    "SBASE3",  
    "SBRDG5",  
    "SBRDGO",  
    "SBUNK2",
    "SBUNK9",
    "SMISC1",
    "SMISC3",
    "SMISC4",
    "SMISCB",
    "SPLAT7",
    "STOWR1",
    "STOWR3",
    "STOWR4",
    "STOWR6",
    "SVPAD",
    "XBASE1",
    "XBRDG0",
    "XBRDG1",
    "XBRDG4",
    "XBRDG8",
    "XBRDG10",
    "XBRDGO",
    "XBUNK9",
    "XTOWR1",
};

char* SceneryList[] = 
{
    "BORG1",                    
    "BORG13",                    
    "BORG16",                    
    "BORG17",                    
    "BORG18",                    
    "BORG19",
    "BORG20",
    "BORG33",
    "BORG34",
    "BORG5",
    "BROCK6",                
    "BROCK7",                
    "BROCK8",                
    "BROCKA",                
    "BROCKC",           
    "BSPIR1",           
    "BSPIR2",           
    "BSPIR3",           
    "BSPIR4",           
    "BSPIR5",
    
    "DORG16",           
    "DORG18",           
    "DORG19",           
    "DROCK8",           

    "PORG1",           
    "PORG2",           
    "PORG20",           
    "PORG3",           
    "PORG5",           
    "PORG6",           
    "PROCK6",           
    "PROCK7",           
    "PROCK8",           
    "PROCKA",           
    "PROCKB",           
    "PROCKC",           
    "PSPIR1",                    
    "PSPIR2",                    
    "PSPIR3",                    
    "PSPIR4",                    
    "PSPIR5",                    

    "SORG20",
    "SORG22",
    "SROCK6",           
    "SROCK7",           
    "SROCK8",           
    "SSPIR1",                    
    "SSPIR2",                    
    "SSPIR3",                    
    "SSPIR4",

    "STACKABLE1L",
    "STACKABLE2L",
    "STACKABLE3L",
    "STACKABLE4L",
    "STACKABLE5L",
    "STACKABLE1M",
    "STACKABLE2M",
    "STACKABLE3M",
    "STACKABLE1S",
    "STACKABLE2S",
    "STACKABLE3S",

    "STATUE_BASE",
    "STATUE_HMALE",
    "VEHICLE_AIR_SCOUT_WRECK",
    "VEHICLE_LAND_ASSAULT_WRECK",

    "NEXUSBASE",
    "NEXUSCAP",
};

//==============================================================================
//  FUNCTIONS
//==============================================================================

attribute* attribute::Read( token_stream& Tok )
{
    attribute* pAttribute = new attribute;

    ASSERT( Tok.Type() == token_stream::TOKEN_SYMBOL );

    // Current token is the Name.
    pAttribute->Name = Tok.String();
    pAttribute->Name.MakeUpper();

    // Next token is probably "=", but could be "[".
    Tok.Read();
    ASSERT( Tok.Type() == token_stream::TOKEN_DELIMITER );
    if( Tok.String()[0] == '[' )
    {
        Tok.Read(); // Numeric value
        pAttribute->Name += Tok.String();
        Tok.Read(); // "]".
        ASSERT( Tok.Type() == token_stream::TOKEN_DELIMITER );
        Tok.Read(); // "=".
        ASSERT( Tok.Type() == token_stream::TOKEN_DELIMITER );
    }

    // Read the Value as a string or symbol.
    Tok.Read();
    ASSERT( (Tok.Type() == token_stream::TOKEN_STRING) ||
            (Tok.Type() == token_stream::TOKEN_SYMBOL) );
    pAttribute->Value = Tok.String();
    if( (pAttribute->Name != "NAMETAG") && (pAttribute->Name != "NAME") && (pAttribute->Name != "LABEL") )
        pAttribute->Value.MakeUpper();

    // Read off ";".
    Tok.Read();
    ASSERT( Tok.Type() == token_stream::TOKEN_DELIMITER );
    
    // Prime read for next whatever.
    Tok.Read();

    return( pAttribute );
}

//==============================================================================

xcolor attribute::GetColor( void )
{
    xcolor      C;
    const char* p = Value;

    C.R = u8( x_atof( p ) * 255.0f );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    C.G = u8( x_atof( p ) * 255.0f );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    C.B = u8( x_atof( p ) * 255.0f );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    C.A = u8( x_atof( p ) * 255.0f );

    return( C );
}

//==============================================================================

vector3 attribute::GetPosition( void )
{
    vector3     P;
    const char* p = Value;

    P.Z = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    P.X = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    P.Y = x_atof( p );
    P.X += 1024.0f;
    P.Z += 1024.0f;

    return( P );
}

//==============================================================================

vector3 attribute::GetDirection( void )
{
    vector3     D;
    const char* p = Value;

    D.Z = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    D.X = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    D.Y = x_atof( p );

    return( D );
}

//==============================================================================

vector3 attribute::GetScale( void )
{
    vector3     S;
    const char* p = Value;

    S.X = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    S.Z = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    S.Y = x_atof( p );

    return( S );
}

//==============================================================================

void CheckOrientation( matrix4& M1, matrix4& M2 )
{
    vector3 X  = vector3( 1, 0, 0 );
    vector3 Y  = vector3( 0, 1, 0 );
    vector3 Z  = vector3( 0, 0, 1 );

    vector3 AX = M1.Transform( X );
    vector3 AY = M1.Transform( Y );
    vector3 AZ = M1.Transform( Z );
    vector3 BX = M2.Transform( X );
    vector3 BY = M2.Transform( Y );
    vector3 BZ = M2.Transform( Z );
    
    vector3 DX = AX - BX;
    vector3 DY = AY - BY;
    vector3 DZ = AZ - BZ;
    
    f32 e = 0.005f;
    
    ASSERT( ( ABS(DX.X) < e ) && ( ABS(DX.Y) < e ) && ( ABS(DX.Z) < e ));
    ASSERT( ( ABS(DY.X) < e ) && ( ABS(DY.Y) < e ) && ( ABS(DY.Z) < e ));
    ASSERT( ( ABS(DZ.X) < e ) && ( ABS(DZ.Y) < e ) && ( ABS(DZ.Z) < e ));
}

//==============================================================================

radian3 attribute::GetRotation( void )
{
    radian3 R;
    vector3 Axis, X, Y, Z;
    radian  Angle;
    matrix4 M, T;
    const char* p = Value;

    //
    // convert from Dynamix to Inevitable space
    //

    Axis.Z = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    Axis.X = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    Axis.Y = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    Angle  = DEG_TO_RAD( x_atof( p ) );

    Axis.Normalize();
    
    M.Setup( Axis, -Angle );

    //
    // perform 90 degree rotation around local Y-Axis
    //
    
    M.GetColumns( X, Y, Z );
    quaternion Q = quaternion( Y, R_90 );
    
    M = Q * M;

    //
    // extract rotations and ensure they are correct
    //
    
    R = M.GetRotation();
    T = matrix4( R );

    CheckOrientation( T, M );

    return( R );
}

//==============================================================================

f32 attribute::GetFloat( void )
{
    return( x_atof( Value ) );
}

//==============================================================================

vector3 attribute::Get3Floats( void )
{
    vector3     F;
    const char* p = Value;

    F[0] = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    F[1] = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    F[2] = x_atof( p );

    return( F );
}

//==============================================================================

vector4 attribute::Get4Floats( void )
{
    vector4     F;
    const char* p = Value;

    F[0] = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    F[1] = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    F[2] = x_atof( p );  VERIFY(p=x_strchr(p,' '));  while(x_isspace(*p)) p++;
    F[3] = x_atof( p );

    return( F );
}

//==============================================================================

void attribute::Dump( X_FILE* pFile, s32 Indent )
{
    ASSERT( Indent < 128 );
    char Spaces[128];
    x_memset( Spaces, ' ', Indent );
    Spaces[Indent] = '\0';
    x_fprintf( pFile, "%s%s = \"%s\";\n", Spaces, Name, Value );
}

//==============================================================================

item* item::Read( token_stream& Tok )
{
    item* pItem = new item;

    // No string index at this point.
    pItem->StringIndex = -1;

    // Default to "geothermal" power and the "neutral" switch circuit.
    pItem->PowerCircuit  = 0;
    pItem->SwitchCircuit = 0;

    // Current token must be "new".
    ASSERT( Tok.Type() == token_stream::TOKEN_SYMBOL );
    ASSERT( x_strcmp( Tok.String(), "new" ) == 0 );

    // Read the Type.
    Tok.Read();
    ASSERT( Tok.Type() == token_stream::TOKEN_SYMBOL );
    pItem->Type = Tok.String();
    pItem->Type.MakeUpper();
    
    // Read off "(".
    Tok.Read();
    ASSERT( Tok.Type() == token_stream::TOKEN_DELIMITER );
    
    // Read the Name (optional).
    Tok.Read();
    if( Tok.Type() == token_stream::TOKEN_SYMBOL )
    {
        pItem->Name = Tok.String();
        pItem->Name.MakeUpper();

        // Read off ")".
        Tok.Read();
    }

    // Make sure we got the ")".
    ASSERT( Tok.Type() == token_stream::TOKEN_DELIMITER );
    
    // Read off "{".
    Tok.Read();
    ASSERT( Tok.Type() == token_stream::TOKEN_DELIMITER );

    // Prime read for next whatever.
    Tok.Read();

    // Read all embedded attributes and items.
    while( Tok.Type() == token_stream::TOKEN_SYMBOL )
    {
        if( x_strcmp( Tok.String(), "new" ) == 0 )
            pItem->Items.Append( item::Read( Tok ) );
        else
            pItem->Attributes.Append( attribute::Read( Tok ) );
    }

    // Make sure we got a "}".
    ASSERT( Tok.Type() == token_stream::TOKEN_DELIMITER );

    // Read off ";".
    Tok.Read();
    ASSERT( Tok.Type() == token_stream::TOKEN_DELIMITER );

    // Prime read for next whatever.
    Tok.Read();

    return( pItem );
}

//==============================================================================

attribute* item::FindAttr( const char* pName )
{
    s32 i;

    for( i = 0; i < Attributes.GetCount(); i++ )
        if( Attributes[i]->Name == pName )
            return( Attributes[i] );

    return( NULL );
}

//==============================================================================

void item::AddString( const xstring& S )
{
    if( StringIndex == -1 )
    {
        StringIndex = StringTable.Find( S );
        if( StringIndex == -1 )
        {
            StringIndex = StringTable.GetCount();
            StringTable.Append( S );
        }
    }

    ASSERT( StringIndex != -1 );
    ASSERT( StringTable[StringIndex] == S );
}

//==============================================================================

xcolor item::GetColor( const char* pName )
{
    attribute* pAttr;
    xcolor     C;

    if( pAttr = FindAttr( pName ) )
        C = pAttr->GetColor();

    return( C );
}

//==============================================================================

vector3 item::GetPosition( const char* pName )
{
    attribute* pAttr;
    vector3    P;

    if( pAttr = FindAttr( pName ) )
        P = pAttr->GetPosition();

    return( P );
}

//==============================================================================

vector3 item::GetDirection( const char* pName )
{
    attribute* pAttr;
    vector3    D;

    if( pAttr = FindAttr( pName ) )
        D = pAttr->GetDirection();

    return( D );
}

//==============================================================================

vector3 item::GetScale( const char* pName )
{
    attribute* pAttr;
    vector3    S;

    if( pAttr = FindAttr( pName ) )
        S = pAttr->GetScale();

    return( S );
}

//==============================================================================

radian3 item::GetRotation( const char* pName )
{
    attribute* pAttr;
    radian3    R;

    if( pAttr = FindAttr( pName ) )
        R = pAttr->GetRotation();

    return( R );
}

//==============================================================================

f32 item::GetFloat( const char* pName )
{
    attribute* pAttr;
    f32        F;

    if( pAttr = FindAttr( pName ) )
        F = pAttr->GetFloat();

    return( F );
}

//==============================================================================

vector3 item::Get3Floats( const char* pName )
{
    attribute* pAttr;
    vector3    F;

    if( pAttr = FindAttr( pName ) )
        F = pAttr->Get3Floats();

    return( F );
}

//==============================================================================

vector4 item::Get4Floats( const char* pName )
{
    attribute* pAttr;
    vector4    F;

    if( pAttr = FindAttr( pName ) )
        F = pAttr->Get4Floats();

    return( F );
}

//==============================================================================

void item::Dump( X_FILE* pFile, s32 Indent )
{
    s32 i;
    ASSERT( Indent < 128 );
    char Spaces[128];
    x_memset( Spaces, ' ', Indent );
    Spaces[Indent] = '\0';
    
    x_fprintf( pFile, "%snew %s(%s) { // [SWITCH:%d(%d),POWER:%d] \n", 
                      Spaces, Type, Name, 
                      SwitchCircuit, 
                      SwitchBits[ SwitchCircuit ],
                      PowerCircuit );
    for( i = 0; i < Attributes.GetCount(); i++ )
    {
        Attributes[i]->Dump( pFile, Indent + 4 );
    }
    for( i = 0; i < Items.GetCount(); i++ )
    {
        Items[i]->Dump( pFile, Indent + 4 );
    }
    x_fprintf( pFile, "%s};\n", Spaces );
}

//==============================================================================

void item::Fixup( void )
{
    s32        i;
    attribute* pAttrNameTag;
    attribute* pAttrName;
    attribute* pAttrLabel;

    // If we have a NAMETAG or a NAME, then copy it to the LABEL.

    // Find everything up front.
    pAttrNameTag = FindAttr( "NAMETAG" );
    pAttrName    = FindAttr( "NAME"    );
    pAttrLabel   = FindAttr( "LABEL"   );

    // If we have either a NAMETAG or a NAME...
    if( pAttrNameTag || pAttrName )
    {
        // If we have no label, create it.
        if( !pAttrLabel )
        {
            pAttrLabel = new attribute;
            pAttrLabel->Name = "LABEL";
            Attributes.Append( pAttrLabel );

            if( pAttrNameTag )
                pAttrLabel->Value = pAttrNameTag->Value;
            else
                pAttrLabel->Value = pAttrName->Value;
        }
        else
        {
            // We have a label, so fixup the NameTag and Name.
            if( pAttrNameTag )
                pAttrNameTag->Value = pAttrLabel->Value;
            
            if( pAttrName )
                pAttrName->Value = pAttrLabel->Value;
        }
            
        ASSERT( pAttrLabel->Value[0] != '\\' );
    }

    // Recurse on all child items.
    for( i = 0; i < Items.GetCount(); i++ )
    {
        Items[i]->Fixup();
    }
}

//==============================================================================
//==============================================================================

void TreeWalk( item* pItem, item_fn* pFn )
{
    xstring    Str;
    s32        i;
    attribute* pAttr;

    Str = pFn( pItem );

    // If there was output, then we need to check for tags and may game type.
    if( Str.GetLength() > 0 )
    {
        // Should we tag this item?
        if( (GameType == CAMPAIGN) && (pAttr = pItem->FindAttr( "TAG" )) )
        {
            Str = "#TAG \"" + pAttr->Value + "\"\n" + Str;
        }

        // Is this item restricted by the game type?
        if( (GameType != EXPORT) &&
            (pAttr = pItem->FindAttr( "MISSIONTYPESLIST" )) )
        {
            xbool Restricted = TRUE;
            s32 Where = pAttr->Value.Find( GameTypeInfo[GameType].pString );
            if( Where != -1 )
            {
                // Make sure that we didn't find the DM within TDM.
                if( (GameType == DM) &&
                    (Where > 0) && 
                    (pAttr->Value[Where-1] == 'T') )
                {
                    // Look for next occurance of DM.
                    Where = pAttr->Value.Find( GameTypeInfo[GameType].pString, Where+1 );
                }

                if( Where != -1 )
                    Restricted = FALSE;
            }

            if( Restricted )
            {
                Str = "/*\n" + Str + "*/\n";
            }
        }

        // Output the string.
        if( pFile )
            x_fprintf( pFile, "%s", Str );
    }

    // Recurse.
    Stack.Insert( 0, pItem );
    for( i = 0; i < pItem->Items.GetCount(); i++ )
        TreeWalk( pItem->Items[i], pFn );
    Stack.Delete( 0 );
}

//==============================================================================
//==============================================================================

s32 GetTeam( void )
{
    s32 i;

    for( i = 0; i < Stack.GetCount(); i++ )
    {
        if( (Stack[i]->Type == "SIMGROUP") && (Stack[i]->Name == "TEAM1") )
            return( 0 );

        if( (Stack[i]->Type == "SIMGROUP") && (Stack[i]->Name == "TEAM2") )
            return( 1 );
    }

    return( -1 );
}

//==============================================================================
//==============================================================================

xstring ExtractFogAndSky( item* pItem )
{
    xstring    Str;
    attribute* pAttr;
    xcolor     C;
    vector3    V;
    s32        Volumes = 0;
    xstring    Dir;

    if( pItem->Type != "SKY" )
        return( Str );

    // FOG, and fog map size.
    Str.AddFormat( "FOG     128 32          // Fog Map Size\n" );

    // Fog and haze colors.
    C = pItem->GetColor( "FOGCOLOR" );
    Str.AddFormat( "        %3d %3d %3d     // Fog  Color\n", C.R, C.G, C.B );
    Str.AddFormat( "        %3d %3d %3d     // Haze Color\n", C.R, C.G, C.B );

    // Fog distances.
    f32 FogDist = pItem->GetFloat( "FOGDISTANCE" );
    f32 VisDist = pItem->GetFloat( "VISIBLEDISTANCE" );
    if( VisDist <= FogDist )
        VisDist = FogDist + 10.0f;

    Str.AddFormat( "        %3d %3d         // Start/Finish Distances\n", (s32)FogDist, (s32)VisDist );
    Str.AddFormat( "        11 2            // Haze Start/Finish Angles\n" );

    // Min/Max alphas.
    Str.AddFormat( "        0.00 1.00       // Min/Max Band Alpha\n" );

    // Number of fog volumes.
    while( Volumes < 8 )
    {
        pAttr = pItem->FindAttr( xfs( "FOGVOLUME%d", Volumes+1 ) );
        if( (pAttr) && (pAttr->Value != "0 0 0") )
            Volumes++;
        else
            break;
    }
    Str.AddFormat( "        %d               // Number of Fog Volumes\n", Volumes );

    // Each fog volume.
    for( s32 i = 0; i < Volumes; i++ )
    {
        // They have (vis distance, min, max) in fogVolumeN.
        // We want (min, max, vis distance).
        V = pItem->Get3Floats( xfs( "FOGVOLUME%d", i+1 ) );
        Str.AddFormat( "        %3d %3d %3d     // Volume %d Min/Max Height, VisDist\n",
                     (s32)V[1], (s32)V[2], (s32)V[0], i );
    }

    // Now the sky.
    Str.AddFormat( "\n" );
    pAttr = pItem->FindAttr( "MATERIALLIST" );
    ASSERT( pAttr );

    // Determine which sky directory to use.
    Dir = "lush";
    if( pAttr->Value.Find( "ICE"          ) != -1 )    Dir = "ice";
    if( pAttr->Value.Find( "LUSH"         ) != -1 )    Dir = "lush";
    if( pAttr->Value.Find( "LAVA"         ) != -1 )    Dir = "desert2";
    if( pAttr->Value.Find( "BADLAND"      ) != -1 )    Dir = "badlands";
    if( pAttr->Value.Find( "STARRY"       ) != -1 )    Dir = "ice";
    if( pAttr->Value.Find( "DESERT"       ) != -1 )    Dir = "desert";
    if( pAttr->Value.Find( "DESERT_BLUE"  ) != -1 )    Dir = "desert";
    if( pAttr->Value.Find( "DESERT_BROWN" ) != -1 )    Dir = "desert2";

    // Manual sky replacements
    if( x_stricmp( MissionName, "Firestorm" ) == 0 )   Dir = "desert2";
    if( x_stricmp( MissionName, "Katabatic" ) == 0 )   Dir = "lush";
    if( x_stricmp( MissionName, "Rimehold" ) == 0 )    Dir = "lush";
    if( x_stricmp( MissionName, "Whiteout" ) == 0 )    Dir = "lush";
    if( x_stricmp( MissionName, "Mission11" ) == 0 )   Dir = "lush";

    // And print it all out.
    Str.AddFormat( "SKY     Data/Sky/%s/SkyT.bmp      // Top\n",   Dir );
    Str.AddFormat( "        Data/Sky/%s/SkyF.bmp      // Front\n", Dir );
    Str.AddFormat( "        Data/Sky/%s/SkyL.bmp      // Left\n",  Dir );
    Str.AddFormat( "        Data/Sky/%s/SkyB.bmp      // Back\n",  Dir );
    Str.AddFormat( "        Data/Sky/%s/SkyR.bmp      // Right\n", Dir );

    return( Str );
}

//==============================================================================

xstring EstablishPowerGrid( item* pItem )
{
    xstring    Str;
    attribute* pAttr;
    item*      pParent;

    if( pItem->Type != "STATICSHAPE" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );
    if( pAttr->Value != "GENERATORLARGE" )
        return( Str );

    // OK.  We have a generator.  If its PARENT item (a sim group) already has
    // power, then we are done.  However, if the parent item does NOT have 
    // power, then it gets a new power circuit.

    pParent = Stack[0];
    if( pParent->PowerCircuit == 0 )
    {
        pParent->PowerCircuit = NPowerCircuits;
        NPowerCircuits++;
    }
    ASSERT( NPowerCircuits < 16 );
    PowerSupply[pParent->PowerCircuit]++;

    return( Str );
}

//==============================================================================

xstring EstablishSwitchGrid( item* pItem )
{
    xstring    Str;
    s32        T;
    attribute* pAttr;
    item*      pParent;

    // Look for a flip flop.
    if( (pItem->Type == "STATICSHAPE") &&
        (pAttr = pItem->FindAttr( "DATABLOCK" )) &&
        (pAttr->Value == "FLIPFLOP") )
    {
        // OK.  We have a flip flip.  Each one gets its own switch circuit.

        pParent = Stack[0];
        pParent->SwitchCircuit = NSwitchCircuits;

        // See if the flip flop already belongs to a team.
        T = GetTeam();                          // Returns -1, 0, 1.
        if( T != -1 )  T++;                     // Need    -1, 1, 2.

        SwitchBits[NSwitchCircuits] = T;

        NSwitchCircuits++;
        ASSERT( NSwitchCircuits < 16 );
    }

    // If we have a team simgroup, then assign it a dedicated switch circuit.

    // Look for a team1 simgroup.  Assign it the reserved team 1 circuit.
    if( (pItem->Type == "SIMGROUP") && (pItem->Name == "TEAM1") )
    {
        pItem->SwitchCircuit = 1;
    }

    // Look for a team2 simgroup.  Assign it the reserved team 2 circuit.
    if( (pItem->Type == "SIMGROUP") && (pItem->Name == "TEAM2") )
    {
        pItem->SwitchCircuit = 2;
    }

    return( Str );
}

//==============================================================================

xstring DistributePowerAndSwitch( item* pItem )
{
    xstring Str;

    if( (pItem->PowerCircuit == 0) && (Stack.GetCount() > 0) )
        pItem->PowerCircuit = Stack[0]->PowerCircuit;

    if( (pItem->SwitchCircuit == 0) && (Stack.GetCount() > 0) )
        pItem->SwitchCircuit = Stack[0]->SwitchCircuit;

    return( Str );
}

//==============================================================================

xstring NonTeamBasedPowerAndSwitch( item* pItem )
{
    xstring Str;

    pItem->PowerCircuit  = 0;
    pItem->SwitchCircuit = 0;

    return( Str );
}

//==============================================================================

xstring ExtractMusic( item* pItem )
{
    xstring    Str;
    attribute* pAttr;
    
    if( pItem->Type != "SIMGROUP" )
        return( Str );
    if( pItem->Name != "MISSIONGROUP" )
        return( Str );

    pAttr = pItem->FindAttr( "MUSICTRACK" );
    ASSERT( pAttr );

    Str.AddFormat( "MUSIC           %s\n", pAttr->Value );

    return( Str );
}
    
//==============================================================================

xstring ExtractEnvironment( item* pItem )
{
    xstring    Str;
    attribute* pAttr;
    xstring    Environment;

    if( pItem->Type != "TERRAINBLOCK" )
        return( Str );

    pAttr = pItem->FindAttr( "DETAILTEXTURE" );
    ASSERT( pAttr );

    if( pAttr->Value.Find( "ICE"     ) != -1 )   Environment = "ICE";
    if( pAttr->Value.Find( "SNOW"    ) != -1 )   Environment = "ICE";
    if( pAttr->Value.Find( "LUSH"    ) != -1 )   Environment = "LUSH";
    if( pAttr->Value.Find( "LAVA"    ) != -1 )   Environment = "LAVA";
    if( pAttr->Value.Find( "DESERT"  ) != -1 )   Environment = "DESERT";
    if( pAttr->Value.Find( "BAD"     ) != -1 )   Environment = "BADLAND";

    ASSERT( Environment != "" );

    Str.AddFormat( "ENVIRONMENT     %s\n", Environment );

    return( Str );
}

//==============================================================================

xstring ExtractSun( item* pItem )
{
    xstring Str;
    vector3 Direction;
    xcolor  Direct;
    xcolor  Ambient;

    if( pItem->Type != "SUN" )
        return( Str );

    Direction = pItem->GetDirection( "DIRECTION" );
    Direction = -Direction; 
    
    Direct    = pItem->GetColor( "COLOR" );
    Ambient   = pItem->GetColor( "AMBIENT" );
                      
    Str.AddFormat( "SUN     %10.4f   %10.4f   %10.4f    // Direction\n", Direction.X, Direction.Y, Direction.Z );
    Str.AddFormat( "        %3d %3d %3d     // Direct Color\n",  Direct.R,  Direct.G,  Direct.B  );
    Str.AddFormat( "        %3d %3d %3d     // Ambient Color\n", Ambient.R, Ambient.G, Ambient.B );

    return( Str );
}

//==============================================================================

xstring ExtractCameraPoints( item* pItem )
{
    xstring Str;
    vector3 P;
    radian3 R;

    if( pItem->Type != "CAMERA" )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetRotation( "ROTATION" );
                       
    Str.AddFormat( "CAMERA                  %10.4f   %10.4f   %10.4f // %10.4f   %10.4f\n", 
                   P.X, P.Y, P.Z,
                   RAD_TO_DEG( R.Pitch ), 
                   RAD_TO_DEG( R.Yaw ) );

    //
    // NOTE:  The rotations are a little funky.  Figure it out later.
    //

    return( Str );
}

//==============================================================================

xstring ExtractSpawnPoints( item* pItem )
{
    xstring Str;
    vector3 P;
    f32     R;
    s32     T;

    if( pItem->Type != "SPAWNSPHERE" )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetFloat   ( "RADIUS"   );
    T = GetTeam();
                       
    Str.AddFormat( "SPAWN        %2d         %10.4f   %10.4f   %10.4f   %10.4f    0.0000    0.0000\n",
                   T, P.X, P.Y, P.Z, R );

    return( Str );
}

//==============================================================================

xstring ExtractBounds( item* pItem )
{
    xstring Str;
    vector4 F;
    vector3 Min;
    vector3 Max;
    f32     C, R;

    if( pItem->Type != "MISSIONAREA" )
        return( Str );

    F = pItem->Get4Floats( "AREA" );
    C = pItem->GetFloat  ( "FLIGHTCEILING" );
    R = pItem->GetFloat  ( "FLIGHTCEILINGRANGE" );

    Min.Z = F[0] + 1024.0f;
    Min.X = F[1] + 1024.0f;
    Min.Y = -2048.0f;

    Max.Z = F[2] + Min.Z;
    Max.X = F[3] + Min.X;
    Max.Y = C + R;
                       
    Str.AddFormat( "BOUNDS                  %10.4f   %10.4f   %10.4f\n", Min.X, Min.Y, Min.Z );
    Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", Max.X, Max.Y, Max.Z );

    return( Str );
}

//==============================================================================

xstring ExtractFlags( item* pItem )
{
    xstring Str;
    vector3 P;
    radian  Yaw;
    s32     T;
    attribute* pAttr;

    if( pItem->Type != "ITEM" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );
    if( pAttr->Value != "FLAG" )
        return( Str );

    P   = pItem->GetPosition( "POSITION" );
    Yaw = pItem->GetRotation( "ROTATION" ).Yaw;
    T   = GetTeam();

    Str.AddFormat( "FLAG_HOME    %2d         %10.4f   %10.4f   %10.4f   %10.4f\n",
                      T, P.X, P.Y, P.Z, Yaw );

    return( Str );
}

//==============================================================================

xstring ExtractPickups( item* pItem )
{
    char* List[] =
    {
        "DISC",                             
        "PLASMA",                           
        "SNIPERRIFLE",                      
        "GRENADELAUNCHER",                  
        "CHAINGUN",                         
        "BLASTER",                          
        "ELFGUN",                           

        "DISCAMMO",                         
        "CHAINGUNAMMO",                     
        "PLASMAAMMO",
        "GRENADELAUNCHERAMMO", 
        "GRENADE",
        "MINE",

        "REPAIRPATCH",
        "REPAIRKIT",

        "AMMOPACK",                         
        "ENERGYPACK",                       
        "REPAIRPACK",                       
        "SHIELDPACK",                       

        "SATCHELCHARGE",                    
    };

    xstring Str;
    vector3 P;
    s32     i;
    attribute* pAttr;

    if( pItem->Type != "ITEM" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );

    // Search for the datablock value in the list.    
    for( i = 0; i < sizeof(List) / 4; i++ )
    {
        if( List[i] == pAttr->Value )
            break;
    }

    if( i == sizeof(List) / 4 )
        return( Str );

    P = pItem->GetPosition( "POSITION" );

    // No more ELF gun pickups.
    if( i == 6 )
    {
        x_printf( "ELF gun pickup found.\n" );
    }

    Str.AddFormat( "PICKUP                  %10.4f   %10.4f   %10.4f   %s\n",
                   P.X, P.Y, P.Z, List[i] );

    return( Str );
}

//==============================================================================

xstring ExtractInventoryStations( item* pItem )
{
    xstring Str;
    xstring N;
    vector3 P;
    radian3 R;
    s32     T;
    attribute* pAttr;

    if( pItem->Type != "STATICSHAPE" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );
    if( pAttr->Value != "STATIONINVENTORY" )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetRotation( "ROTATION" );
    T = GetTeam();
                       
    pAttr = pItem->FindAttr( "LABEL" );
    if( pAttr && !pAttr->Value.IsEmpty() )
        N = pAttr->Value + " Station";
    else
        N = "Inventory Station";

    pItem->AddString( N );

    if( N.GetLength() > 27 )
        x_printf( "*** WARNING ***  Label too long: \"%s\"\n", (const char*)N );

    Str.AddFormat( "INVEN           %2d %2d   %10.4f   %10.4f   %10.4f   %2d\n",
                   pItem->SwitchCircuit, pItem->PowerCircuit, 
                   P.X, P.Y, P.Z, pItem->StringIndex );
                       
    Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n\n", 
                   RAD_TO_DEG( R.Pitch ), 
                   RAD_TO_DEG( R.Yaw   ), 
                   RAD_TO_DEG( R.Roll  ) );

    return( Str );
}

//==============================================================================

xstring ExtractVehiclePads( item* pItem )
{
    xstring Str;
    xstring N;
    vector3 P;
    radian3 R;
    s32     T;
    attribute* pAttr;

    if( pItem->Type != "STATICSHAPE" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );
    if( pAttr->Value != "STATIONVEHICLEPAD" )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetRotation( "ROTATION" );
    T = GetTeam();
                      
//  pAttr = pItem->FindAttr( "LABEL" );
//  if( pAttr )
//      N = pAttr->Value + " Vehicle Station";
//  else
//      N = "Vehicle Station";
    N = "Vehicle Station";
    pItem->AddString( N );

    if( N.GetLength() > 27 )
        x_printf( "*** WARNING ***  Label too long: \"%s\"\n", (const char*)N );

    Str.AddFormat( "VEHICLEPAD      %2d %2d   %10.4f   %10.4f   %10.4f   %2d\n",
                   pItem->SwitchCircuit, pItem->PowerCircuit, 
                   P.X, P.Y, P.Z, pItem->StringIndex );
                       
    Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n\n", 
                   RAD_TO_DEG( R.Pitch ), 
                   RAD_TO_DEG( R.Yaw   ), 
                   RAD_TO_DEG( R.Roll  ) );

    return( Str );
}

//==============================================================================

xstring ExtractTurrets( item* pItem )
{
    xstring Str;
    xstring N;
    vector3 P;
    radian3 R;
    s32     T;
    attribute* pAttr;
    xstring    Barrel;

    if( pItem->Type != "TURRET" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );

    if( pAttr->Value == "TURRETBASELARGE" )
    {
        pAttr = pItem->FindAttr( "INITIALBARREL" );
        if( !pAttr )
            return( Str );

        if( pAttr->Value == "AABARRELLARGE"      )     Barrel = "AA";
        if( pAttr->Value == "ELFBARRELLARGE"     )     Barrel = "ELF";
        if( pAttr->Value == "MISSILEBARRELLARGE" )     Barrel = "Missile";
        if( pAttr->Value == "MORTARBARRELLARGE"  )     Barrel = "Mortar";
        if( pAttr->Value == "PLASMABARRELLARGE"  )     Barrel = "Plasma";

        ASSERT( Barrel != "" );
    }
    else
    if( pAttr->Value == "SENTRYTURRET" )
    {
        Barrel = "Sentry";
    }
    else
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetRotation( "ROTATION" );
    T = GetTeam();

    pAttr = pItem->FindAttr( "LABEL" );
    if( pAttr && !pAttr->Value.IsEmpty() )
        N = pAttr->Value + " Turret";
    else
        N = "Turret";

    pItem->AddString( N );

    if( N.GetLength() > 27 )
        x_printf( "*** WARNING ***  Label too long: \"%s\"\n", (const char*)N );

    Str.AddFormat( "TURRET          %2d %2d   %10.4f   %10.4f   %10.4f   %2d\n",
                   pItem->SwitchCircuit, pItem->PowerCircuit, 
                   P.X, P.Y, P.Z, pItem->StringIndex );
                       
    Str.AddFormat( "                        %10.4f   %10.4f   %10.4f   %s\n\n", 
                   RAD_TO_DEG( R.Pitch ), 
                   RAD_TO_DEG( R.Yaw   ), 
                   RAD_TO_DEG( R.Roll  ),
                   Barrel );

    return( Str );
}

//==============================================================================

xstring ExtractSensors( item* pItem )
{
    xstring Str;
    xstring N;
    vector3 P;
    radian3 R;
    char    S = '\0';
    attribute* pAttr;

    if( pItem->Type != "STATICSHAPE" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );

    if( pAttr->Value == "SENSORLARGEPULSE" )
        S = 'L';

    if( pAttr->Value == "SENSORMEDIUMPULSE" )
        S = 'M';

    if( S == '\0' )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetRotation( "ROTATION" );

    pAttr = pItem->FindAttr( "LABEL" );
    if( pAttr && !pAttr->Value.IsEmpty() )
        N = pAttr->Value + " Sensor";
    else
        N = "Sensor";

    pItem->AddString( N );

    if( N.GetLength() > 27 )
        x_printf( "*** WARNING ***  Label too long: \"%s\"\n", (const char*)N );

    Str.AddFormat( "SENSOR %c        %2d %2d   %10.4f   %10.4f   %10.4f   %2d\n",
                   S, pItem->SwitchCircuit, pItem->PowerCircuit, 
                   P.X, P.Y, P.Z, pItem->StringIndex );
                       
    Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n\n", 
                   RAD_TO_DEG( R.Pitch ), 
                   RAD_TO_DEG( R.Yaw   ), 
                   RAD_TO_DEG( R.Roll  ) );

    return( Str );
}

//==============================================================================

xstring ExtractGenerators( item* pItem )
{
    xstring Str;
    xstring N;
    vector3 P;
    radian3 R;
    attribute* pAttr;

    if( pItem->Type != "STATICSHAPE" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );
    if( pAttr->Value != "GENERATORLARGE" )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetRotation( "ROTATION" );

    pAttr = pItem->FindAttr( "LABEL" );
    if( pAttr && !pAttr->Value.IsEmpty() )
    {
        if( pAttr->Value.GetLength() > 17 )
            N = pAttr->Value + " Gen";
        else
            N = pAttr->Value + " Generator";
    }
    else
        N = "Generator";

    pItem->AddString( N );

    if( N.GetLength() > 27 )
        x_printf( "*** WARNING ***  Label too long: \"%s\"\n", (const char*)N );

    Str.AddFormat( "GENERATOR       %2d %2d   %10.4f   %10.4f   %10.4f   %2d\n",
                   pItem->SwitchCircuit, pItem->PowerCircuit, 
                   P.X, P.Y, P.Z, pItem->StringIndex );
                       
    Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n\n", 
                   RAD_TO_DEG( R.Pitch ), 
                   RAD_TO_DEG( R.Yaw   ), 
                   RAD_TO_DEG( R.Roll  ) );

    return( Str );
}

//==============================================================================

xstring ExtractForceFields( item* pItem )
{
    xstring Str;
    xstring N;
    vector3 P;
    vector3 S;
    radian3 R;
    s32     T;
    attribute* pAttr;

    if( pItem->Type != "FORCEFIELDBARE" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );

    if( pAttr->Value == "DEFAULTFORCEFIELDBARE" )
        pItem->SwitchCircuit = 3;

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetRotation( "ROTATION" );
    S = pItem->GetScale   ( "SCALE"    );
    T = GetTeam();
                      
    Str.AddFormat( "FORCEFIELD      %2d %2d   %10.4f   %10.4f   %10.4f\n",
                   pItem->SwitchCircuit, pItem->PowerCircuit, 
                   P.X, P.Y, P.Z, N );
                       
    Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                   RAD_TO_DEG( R.Pitch ), 
                   RAD_TO_DEG( R.Yaw   ), 
                   RAD_TO_DEG( R.Roll  ) );

    Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n\n", 
                   S.X, S.Y, S.Z );

    return( Str );
}

//==============================================================================

xstring ExtractFlipFlops( item* pItem )
{
    xstring Str;
    vector3 P;
    radian3 R;
    attribute* pAttr;

    if( pItem->Type != "STATICSHAPE" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );
    if( pAttr->Value != "FLIPFLOP" )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetRotation( "ROTATION" );

    Str.AddFormat( "FLIPFLOP        %2d      %10.4f   %10.4f   %10.4f\n",
                   pItem->SwitchCircuit,
                   P.X, P.Y, P.Z );
                       
    Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n\n", 
                   RAD_TO_DEG( R.Pitch ), 
                   RAD_TO_DEG( R.Yaw   ), 
                   RAD_TO_DEG( R.Roll  ) );

    return( Str );
}

//==============================================================================

xstring ExtractProjectors( item* pItem )
{
    xstring Str;
    vector3 P;
    radian3 R;
    attribute* pAttr;

    if( pItem->Type != "STATICSHAPE" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );
    if( pAttr->Value != "LOGOPROJECTOR" )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetRotation( "ROTATION" );

    Str.AddFormat( "PROJECTOR       %2d      %10.4f   %10.4f   %10.4f\n",
                   pItem->SwitchCircuit,
                   P.X, P.Y, P.Z );
                       
    Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n\n", 
                   RAD_TO_DEG( R.Pitch ), 
                   RAD_TO_DEG( R.Yaw   ), 
                   RAD_TO_DEG( R.Roll  ) );

    return( Str );
}

//==============================================================================

xstring ExtractWaypoints( item* pItem )
{
    xstring Str;
    xstring Name;
    vector3 P;
    attribute* pAttr;
    attribute* pNameAttr;

    if( pItem->Type != "WAYPOINT" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );
    if( pAttr->Value != "WAYPOINTMARKER" )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    pNameAttr = pItem->FindAttr( "LABEL" );

    if( pNameAttr )
    {
        Name = pNameAttr->Value;
    }  

    pItem->AddString( Name );

    Str.AddFormat( "WAYPOINT        %2d      %10.4f   %10.4f   %10.4f   %2d\n",
                   pItem->SwitchCircuit,
                   P.X, P.Y, P.Z, pItem->StringIndex );

    return( Str );
}

//==============================================================================

xstring ExtractNexus( item* pItem )
{
    xstring Str;
    vector3 P;
    vector3 S;
    attribute* pAttr;

    if( pItem->Type != "ITEM" )
        return( Str );

    pAttr = pItem->FindAttr( "DATABLOCK" );
    if( !pAttr )
        return( Str );
    if( pAttr->Value != "NEXUS" )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    S = pItem->GetScale( "SCALE" );

    Str.AddFormat( "NEXUS                   %10.4f   %10.4f   %10.4f   %10.4f\n",
                    P.X, P.Y, P.Z, S.Y );
 
    return( Str );
}

//==============================================================================
//  A valid building must be in the "BuildingList" and NOT in the "SceneryList".

xstring ExtractBuildings( item* pItem )
{
    xstring Str;
    s32     i;
    vector3 P;
    radian3 R;
    vector3 S;
    xbool   InBuilding = FALSE;
    xbool   InScenery  = FALSE;
    attribute* pAttr;
    xstring    Name;

    if( pItem->Type != "INTERIORINSTANCE" )
        return( Str );

    P = pItem->GetPosition( "POSITION" );
    R = pItem->GetRotation( "ROTATION" );
    S = pItem->GetScale   ( "SCALE"    );
    
    pAttr = pItem->FindAttr( "INTERIORFILE" );
    ASSERT( pAttr );
    Name = pAttr->Value;
    Name = Name.Left( Name.Find('.') );
    
    for( i = 0; i < sizeof(BuildingList)/4; i++ )
        if( Name == BuildingList[i] )
            InBuilding = TRUE;

    for( i = 0; i < sizeof(SceneryList)/4; i++ )
        if( Name == SceneryList[i] )
            InScenery = TRUE;

    if( InBuilding && !InScenery )
    {
        Str.AddFormat( "BUILDING           %2d   %10.4f   %10.4f   %10.4f\n",
                          pItem->PowerCircuit, 
                          P.X, P.Y, P.Z );
                       
        Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                          RAD_TO_DEG( R.Pitch ), 
                          RAD_TO_DEG( R.Yaw   ), 
                          RAD_TO_DEG( R.Roll  ) );

        Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                          S.X, S.Y, S.Z );

        Str.AddFormat( "                        \"%s\"\n\n", 
                          Name );
    }

    if( !InBuilding && !InScenery )
    {
        x_printf( "*** WARNING ***  Unknown building instance found: %s\n", Name );
        Str.AddFormat( "PLACE_HOLDER            %10.4f   %10.4f   %10.4f\n",
                          P.X, P.Y, P.Z );                       
        Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                          RAD_TO_DEG( R.Pitch ), 
                          RAD_TO_DEG( R.Yaw   ), 
                          RAD_TO_DEG( R.Roll  ) );
        Str.AddFormat( "                        \"BLDG:%s\"\n\n", Name );
    }

    return( Str );
}

//==============================================================================

xstring ExtractScenery( item* pItem )
{
    xstring Str;
    s32     i;
    vector3 P;
    radian3 R;
    vector3 S;
    xbool   InScenery = FALSE;
    attribute* pAttr;
    xstring    Name;

    if( pItem->Type == "INTERIORINSTANCE" )
    {
        P = pItem->GetPosition( "POSITION" );
        R = pItem->GetRotation( "ROTATION" );
        S = pItem->GetScale   ( "SCALE"    );
    
        pAttr = pItem->FindAttr( "INTERIORFILE" );
        ASSERT( pAttr );
        Name = pAttr->Value;
        Name = Name.Left( Name.Find('.') );
  
        for( i = 0; i < sizeof(SceneryList)/4; i++ )
            if( Name == SceneryList[i] )
                InScenery = TRUE;

        if( InScenery )
        {
            Str.AddFormat( "SCENERY                 %10.4f   %10.4f   %10.4f\n",
                              P.X, P.Y, P.Z );
                       
            Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                              RAD_TO_DEG( R.Pitch ), 
                              RAD_TO_DEG( R.Yaw   ), 
                              RAD_TO_DEG( R.Roll  ) );

            Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                              S.X, S.Y, S.Z );

            Str.AddFormat( "                        \"%s\"\n\n", 
                              Name );
        }
    }

    if( pItem->Type == "TSSTATIC" ) 
    {
        P = pItem->GetPosition( "POSITION" );
        R = pItem->GetRotation( "ROTATION" );
        S = pItem->GetScale   ( "SCALE"    );
    
        pAttr = pItem->FindAttr( "SHAPENAME" );
        ASSERT( pAttr );
        Name = pAttr->Value;
        Name = Name.Left( Name.Find('.') );
  
        for( i = 0; i < sizeof(SceneryList)/4; i++ )
            if( Name == SceneryList[i] )
                InScenery = TRUE;

        if( InScenery )
        {
            Str.AddFormat( "SCENERY                 %10.4f   %10.4f   %10.4f\n",
                              P.X, P.Y, P.Z );
                       
            Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                              RAD_TO_DEG( R.Pitch ), 
                              RAD_TO_DEG( R.Yaw   ), 
                              RAD_TO_DEG( R.Roll  ) );

            Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                              S.X, S.Y, S.Z );

            Str.AddFormat( "                        \"%s\"\n\n", 
                              Name );
        }

        if( !InScenery )
        {
            x_printf( "*** WARNING ***  Unknown static shape instance found: %s\n", Name );
            Str.AddFormat( "PLACE_HOLDER            %10.4f   %10.4f   %10.4f\n",
                              P.X, P.Y, P.Z );                       
            Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                              RAD_TO_DEG( R.Pitch ), 
                              RAD_TO_DEG( R.Yaw   ), 
                              RAD_TO_DEG( R.Roll  ) );
            Str.AddFormat( "                        \"STATIC:%s\"\n\n", Name );
            return( Str );
        }
    }

    if( pItem->Type == "STATICSHAPE" ) 
    {
        P = pItem->GetPosition( "POSITION" );
        R = pItem->GetRotation( "ROTATION" );
        S = pItem->GetScale   ( "SCALE"    );
    
        pAttr = pItem->FindAttr( "DATABLOCK" );
        ASSERT( pAttr );
        Name = pAttr->Value;
  
        for( i = 0; i < sizeof(SceneryList)/4; i++ )
            if( Name == SceneryList[i] )
                InScenery = TRUE;

        if( InScenery )
        {
            Str.AddFormat( "SCENERY                 %10.4f   %10.4f   %10.4f\n",
                              P.X, P.Y, P.Z );
                       
            Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                              RAD_TO_DEG( R.Pitch ), 
                              RAD_TO_DEG( R.Yaw   ), 
                              RAD_TO_DEG( R.Roll  ) );

            Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                              S.X, S.Y, S.Z );

            Str.AddFormat( "                        \"%s\"\n\n", 
                              Name );
        }
    }

    // Slight special case for the flag stand.
    if( pItem->Type == "STATICSHAPE" )
    {
        P = pItem->GetPosition( "POSITION" );
        R = pItem->GetRotation( "ROTATION" );
        S = pItem->GetScale   ( "SCALE"    );
    
        pAttr = pItem->FindAttr( "DATABLOCK" );
        ASSERT( pAttr );
        if( pAttr->Value == "EXTERIORFLAGSTAND" )
        {
            Str.AddFormat( "SCENERY                 %10.4f   %10.4f   %10.4f\n",
                              P.X, P.Y, P.Z );
                       
            Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                              RAD_TO_DEG( R.Pitch ), 
                              RAD_TO_DEG( R.Yaw   ), 
                              RAD_TO_DEG( R.Roll  ) );

            Str.AddFormat( "                        %10.4f   %10.4f   %10.4f\n", 
                              S.X, S.Y, S.Z );

            Str.AddFormat( "                        \"FLAGSTAND\"\n\n" );
        }
    }

    return( Str );
}

//==============================================================================

xstring ExtractTerrain( item* pItem )
{
    xstring    Str;
    attribute* pAttr;

    if( pItem->Type != "TERRAINBLOCK" )
        return( Str );

    pAttr = pItem->FindAttr( "EMPTYSQUARES" );

    if( pAttr )
    {
        s32 i=0;
        Str.AddFormat( "EMPTYSQUARES " );

        const char* p = (const char*)(pAttr->Value);

        while( 1 )
        {
            // Skip whitespace
            while( *p && ((*p<'0') || (*p>'9')) ) p++;
            if( *p==0 ) break;

            // Read number
            s32 Number = x_atoi( p );
            s32 Square = Number & 0xFFFF;
            s32 Count  = Number >> 16;
            while( Count > 0 )
            {
                if( i%8 == 0 ) Str.AddFormat( "\n" );
                i++;
                Str.AddFormat( "%d ", Square );
                Square++;
                Count--;
            }

            // Skip number, then white space.
            while( (*p >= '0') && (*p <= '9') )  p++;
        }

        Str.AddFormat( "-1\n" );
    } 

    return( Str );
}

//==============================================================================

void ShowUsage( void )
{
    x_printf( "Usage:  ConvertMission <File.mis>\n\n" );
}

//==============================================================================

void DetermineMissionInfo( void )
{
    xbool   GotName  = FALSE;
    xbool   GotTypes = FALSE;
    xstring Line;
    xstring Name;

    Tok.Rewind();
    for( s32 i = 0; i < 2; i++ )
    {
        Line = Tok.ReadLine();

        if( Line.Find( "DisplayName" ) != -1 )
        {
            x_strcpy( MissionName, Line.Right( Line.GetLength() - 17 ) );
            while( MissionName[ x_strlen(MissionName)-1 ] == ' ' )
                MissionName[ x_strlen(MissionName)-1 ] = '\0';
            if( x_strlen( MissionName ) > 0 )
                GotName = TRUE;
        }

        if( Line.Find( "MissionTypes" ) != -1 )
        {
            Line.MakeUpper();
            for( s32 j = CTF; j <= CAMPAIGN; j++ )
            {
                if( Line.Find( GameTypeInfo[j].pString ) != -1 )
                    GameTypeInfo[j].Active = TRUE;
            }
            GotTypes = TRUE;
        }
    }

    if( !GotName )
    {
        x_splitpath( pInputFileName, NULL, NULL, MissionName, NULL );
        x_strtolower( MissionName );
        MissionName[0] = x_toupper( MissionName[0] );
    }
}

//==============================================================================

void ProcessMission( void )
{
    s32     i;
    char    OutFileName[ X_MAX_PATH  ];
    char    Drive      [ X_MAX_DRIVE ];
    char    Dir        [ X_MAX_DIR   ];
    char    FName      [ X_MAX_FNAME ];

    if( GameTypeInfo[GameType].TeamBased )
    {
        // Establish the power grid, the switch grid, and distribute the values.
        for( i = 1; i < 16; i++ )
            PowerSupply[i] = 0;
        PowerSupply[0]  = 1;    // Geothermal power.

        NPowerCircuits  = 1;    // One is reserved for geothermal.
        NSwitchCircuits = 4;    // Four reserved (all teams, Team0, Team1, and no teams).

        TreeWalk( pRootItem, EstablishPowerGrid       );
        TreeWalk( pRootItem, EstablishSwitchGrid      );
        TreeWalk( pRootItem, DistributePowerAndSwitch );
    }
    else
    {
        PowerSupply[0]  = 1;
        SwitchBits[0]   = 0xFFFFFFFF;

        NPowerCircuits  = 1;
        NSwitchCircuits = 1; 
        
        TreeWalk( pRootItem, NonTeamBasedPowerAndSwitch );
    }

    // Create output file.
    x_splitpath( pInputFileName, Drive, Dir, FName, NULL );
    x_makepath ( OutFileName, Drive, Dir, GameTypeInfo[GameType].pFileName, NULL );

    pFile = x_fopen( OutFileName, "wt" );
    if( !pFile )
    {
        x_printf( "Unable to open file \"%s\" for output.\n\n", OutFileName );
        return;
    }

    x_fprintf( pFile, "//==============================================================================\n" );
    x_fprintf( pFile, "// Mission file created by ConvertMission from %s.mis.\n", FName );
    x_fprintf( pFile, "//==============================================================================\n" );
    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "//------------------------------------------------------------------------------\n" );
    x_fprintf( pFile, "//  CLIENT AND SERVER, NON-OBJECT MISSION DATA\n" );
    x_fprintf( pFile, "//------------------------------------------------------------------------------\n" );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "MISSION         \"%s\"\n", MissionName );

    x_fprintf( pFile, "\n" );
    TreeWalk( pRootItem, ExtractMusic );

    x_fprintf( pFile, "\n" );
    TreeWalk( pRootItem, ExtractEnvironment );

    x_fprintf( pFile, "\n" );
    TreeWalk( pRootItem, ExtractSun );

    x_fprintf( pFile, "\n" );
    TreeWalk( pRootItem, ExtractFogAndSky );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Camera points:            X            Y            Z           Pitch         Yaw\n" );
    TreeWalk( pRootItem, ExtractCameraPoints );

    x_fprintf( pFile, "\n" );
    TreeWalk( pRootItem, ExtractBounds );

// MONEY SHOT

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "//------------------------------------------------------------------------------\n" );
    x_fprintf( pFile, "//  CLIENT AND SERVER, PERMANENT OBJECTS\n" );
    x_fprintf( pFile, "//------------------------------------------------------------------------------\n" );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "TERRAIN     \"%s.ter\"\n", FName );
    TreeWalk( pRootItem, ExtractTerrain );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Buildings:      Power     X            Y            Z\n" );
    x_fprintf( pFile, "//                         Pitch         Yaw         Roll\n" );
    x_fprintf( pFile, "//                          SX           SY           SZ\n" );
    x_fprintf( pFile, "//                      FileName\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractBuildings );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Scenery:                  X            Y            Z\n" );
    x_fprintf( pFile, "//                         Pitch         Yaw         Roll\n" );
    x_fprintf( pFile, "//                          SX           SY           SZ\n" );
    x_fprintf( pFile, "//                      FileName\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractScenery );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Gens:    Switch Power     X            Y            Z       Name\n" );
    x_fprintf( pFile, "//                         Pitch         Yaw         Roll\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractGenerators );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Invens:  Switch Power     X            Y            Z       Name\n" );
    x_fprintf( pFile, "//                         Pitch         Yaw         Roll\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractInventoryStations );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// VPads:   Switch Power     X            Y            Z       Name\n" );
    x_fprintf( pFile, "//                         Pitch         Yaw         Roll\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractVehiclePads );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Turrets: Switch Power     X            Y            Z       Name\n" );
    x_fprintf( pFile, "//                         Pitch         Yaw         Roll      Barrel\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractTurrets );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Sensors: Switch Power     X            Y            Z       Name\n" );
    x_fprintf( pFile, "//                         Pitch         Yaw         Roll\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractSensors );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// FFields: Switch Power     X            Y            Z       Type\n" );
    x_fprintf( pFile, "//                         Pitch         Yaw         Roll\n" );
    x_fprintf( pFile, "//                          SX           SY           SZ\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractForceFields );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// FlipFlop:Switch           X            Y            Z\n" );
    x_fprintf( pFile, "//                         Pitch         Yaw         Roll\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractFlipFlops );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Projector:Switch          X            Y            Z\n" );
    x_fprintf( pFile, "//                         Pitch         Yaw         Roll\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractProjectors );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Waypts:  Switch           X            Y            Z\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractWaypoints );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Nexus:                    X            Y            Z         Height\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractNexus );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "//==============================================================================\n" );
    x_fprintf( pFile, "    WATERMARK // CLIENT DOES NOT READ BEYOND THIS POINT\n" );
    x_fprintf( pFile, "//==============================================================================\n" );
    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "//------------------------------------------------------------------------------\n" );
    x_fprintf( pFile, "//  SERVER ONLY, NON-OBJECT MISSION DATA\n" );
    x_fprintf( pFile, "//------------------------------------------------------------------------------\n" );

// Points to win
// Time limit

// MAX_SHRIKES, etc

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "POWER        " );
    for( i = 0; i < 16; i++ )
        x_fprintf( pFile, " %2d ", PowerSupply[i] );
    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "SWITCH       " );
    for( i = 0; i < 16; i++ )
        x_fprintf( pFile, " %2d ", SwitchBits[i] );
    x_fprintf( pFile, "\n" );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Spawn:   Team             X            Y            Z         Radius     Pitch      Yaw\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractSpawnPoints );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Flags:   Team             X            Y            Z           Yaw\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractFlags );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "//------------------------------------------------------------------------------\n" );
    x_fprintf( pFile, "//  SERVER ONLY, DYNAMIC OBJECTS\n" );
    x_fprintf( pFile, "//------------------------------------------------------------------------------\n" );

    x_fprintf( pFile, "\n" );
    x_fprintf( pFile, "// Pickups:                  X            Y            Z           Kind\n" );
    x_fprintf( pFile, "//\n" );
    TreeWalk( pRootItem, ExtractPickups );

// Vehicles
// Pre-deployed deployables

#if 0
    x_fprintf( pFile, "\n\n\n/*\n" );
    pRootItem->Dump( pFile, 0 );
    x_fprintf( pFile, "*/\n" ); 
#endif

    x_fclose( pFile );
}

//==============================================================================

void OutputMissionFile( void )
{
    char    OutFileName[ X_MAX_PATH  ];
    char    Drive      [ X_MAX_DRIVE ];
    char    Dir        [ X_MAX_DIR   ];
    char    FName      [ X_MAX_FNAME ];

    x_splitpath( pInputFileName,  Drive, Dir, FName, NULL );
    x_makepath ( OutFileName, Drive, Dir, FName, ".mis" );
    
    X_FILE* pFile = x_fopen( OutFileName, "wt" );
    if( !pFile )
    {
        x_printf( "Unable to open file \"%s\" for output.\n\n", OutFileName );
        return;
    }

    // Write out comments at start of file.
    for( s32 i = 0; i < NumComments; i++ )
    {
        x_fprintf( pFile, "%s\n", Comment[i] );
    }

    pRootItem->Dump( pFile, 0 );

    x_fprintf( pFile, "//--- OBJECT WRITE END ---\n" );

    x_fclose( pFile );
}    

//==============================================================================

void OutputStringTable( void )
{
    char    OutFileName[ X_MAX_PATH  ];
    char    Drive      [ X_MAX_DRIVE ];
    char    Dir        [ X_MAX_DIR   ];
    char    FName      [ X_MAX_FNAME ];
    char    Header[]  = { (char)0xFF, (char)0xFE };
    char    NewLine[] = { (char)0x0D, (char)0x00, (char)0x0A, (char)0x00 };
    xstring Narrow;      
    xstring Wide;

    // Create output file.
    x_splitpath( pInputFileName, Drive, Dir, FName, NULL );
    x_makepath ( OutFileName, Drive, Dir, "Strings", "txt" );

    X_FILE* pStringFile = x_fopen( OutFileName, "wb" );

    if( pStringFile == NULL )
        return;

    x_fwrite( Header, 1, 2, pStringFile );

    if( StringTable.GetCount() == 0 )
        StringTable.Append( "X" );

    for( s32 i = 0; i < StringTable.GetCount(); i++ )
    {
        if( StringTable[i] == "" )
        {
            StringTable[i] = " ";
        }
        
        Narrow.Format( "S%02d\t%s", i, StringTable[i] );
        Wide = Narrow + Narrow;
        for( s32 j = 0; j < Narrow.GetLength(); j++ )
        {
            Wide[j*2+0] = Narrow[j];
            Wide[j*2+1] = '\0';
        }

        x_fwrite( (const char*)Wide, 1, Wide.GetLength(), pStringFile );
        x_fwrite( NewLine, 1, 4, pStringFile );
    }

    x_fclose( pStringFile );
}

//==============================================================================

void ProcessFile( void )
{
    // Read in the comments at the start of the file.
    {
    
        NumComments = -1;
    
        do
        {
            NumComments++;
            ASSERT(  NumComments < MaxComments );
            x_strcpy( Comment[ NumComments ], Tok.ReadLine() );
        
        } while( x_stricmp( Comment[ NumComments ], "//--- OBJECT WRITE BEGIN ---" ));

        // Include the last string
        NumComments++;
    }

    // Read in the entire mission tree.
    Tok.Read();
    pRootItem = item::Read( Tok );
    
    // Fixup nameTag entries.
    pRootItem->Fixup();

    // Get some information about the mission: mission name and game types.
    DetermineMissionInfo();

    // Process the mission for export purposes.
    GameType = EXPORT;
    ProcessMission();

    // Process the mission for each game type it supports.
    for( GameType = CTF; GameType <= CAMPAIGN; GameType++ )
    {
        if( GameTypeInfo[GameType].Active )
            ProcessMission();
    }

    Tok.CloseFile();

    OutputMissionFile();
    OutputStringTable();
    
    delete pRootItem;
}

//==============================================================================

void main( s32 argc, char* argv[] )
{
    x_Init();

    // Check arg count.
    if( argc != 2 )
    {
        ShowUsage();
        goto SHUT_DOWN;
    }

    // Open input file.
    pInputFileName = argv[1];
    if( !Tok.OpenFile( pInputFileName ) )
    {
        x_printf( "File \"%s\" not found.\n\n", pInputFileName );
        goto SHUT_DOWN;
    }

    ProcessFile();

    //x_MemDump();

SHUT_DOWN:
    ;
//    x_Kill();
}

//==============================================================================
