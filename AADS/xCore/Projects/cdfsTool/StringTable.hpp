//==============================================================================
//
//  string_table.hpp
//
//==============================================================================

#ifndef STRING_TABLE_HPP
#define STRING_TABLE_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "x_files.hpp"

//==============================================================================
//  string_table
//==============================================================================

class string_table
{
public:
//            string_table        ( void );
//           ~string_table        ( void );

    s32     GetCount            ( void );               // Return number of entries
    s32     GetBytes            ( void );               // Return number of bytes
    
    s32     Find                ( xstring& String );    // Find a string
    s32     Append              ( xstring& String );    // Append a string

    s32     Index2Byte          ( s32 Index );

protected:
    xarray<xstring>     m_StringTable;          // Array of strings
    xarray<s32>         m_BytePosition;         // Array of byte positions (includes nulls)
};

//==============================================================================

#endif //STRING_TABLE_HPP
