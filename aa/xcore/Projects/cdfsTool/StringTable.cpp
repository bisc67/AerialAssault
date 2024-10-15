//==============================================================================
//
//  string_table.cpp
//
//==============================================================================

#include "StringTable.hpp"

//==============================================================================

s32 string_table::GetCount( void )
{
    return m_StringTable.GetCount();
}

//==============================================================================

s32 string_table::GetBytes( void )
{
    s32 nBytes = 0;

    if( GetCount() > 0 )
        nBytes = m_BytePosition[GetCount()-1] + m_StringTable[GetCount()-1].GetLength() + 1;

    return nBytes;
}

//==============================================================================

s32 string_table::Find( xstring& String )
{
    s32 Index       =  0;
    s32 FoundIndex  = -1;

    // Search Table for string
    while( Index < m_StringTable.GetCount() )
    {
        // Compare strings
        if( ( m_StringTable[Index].GetLength() == String.GetLength() ) &&
            ( m_StringTable[Index] == String ) )
        {
            // Found it
            FoundIndex = Index;
            break;
        }

        // Advance index
        Index++;
    }

    // Return found index
    return FoundIndex;
}

//==============================================================================

s32 string_table::Append( xstring& String )
{
    s32 Index = m_StringTable.GetCount();

    // Store byte position
    if( Index == 0 )
        m_BytePosition.Append() = 0;
    else
        m_BytePosition.Append() = m_BytePosition[Index-1] + m_StringTable[Index-1].GetLength() + 1;

    // Store string
    m_StringTable.Append() = String;

    // Return index of string within table
    return Index;
}

//==============================================================================

s32 string_table::Index2Byte( s32 Index )
{
    ASSERT( Index >= 0 );
    ASSERT( Index < m_StringTable.GetCount() );

    return m_BytePosition[Index];
}

//==============================================================================
