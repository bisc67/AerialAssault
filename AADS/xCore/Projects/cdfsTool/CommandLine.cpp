//==============================================================================
//
//  CommandLine.cpp
//
//==============================================================================
//
//  Command Line Parser
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include <io.h>
#include "CommandLine.hpp"

//==============================================================================
//  command_line
//==============================================================================

command_line::command_line( void )
{
}

//==============================================================================

command_line::~command_line( void )
{
}

//==============================================================================

s32 command_line::GetNumOptions( void ) const
{
    return m_Options.GetCount();
}

//==============================================================================

s32 command_line::GetNumArguments( void ) const
{
    return m_Arguments.GetCount();
}

//==============================================================================

s32 command_line::FindOption( xstring& Option ) const
{
    s32     i;
    s32     Found   = -1;
    xstring o;

    // Get Uppercase version of option we are looking for
    o = Option;
    o.MakeUpper();

    // Search for option
    for( i=0 ; i<m_Options.GetCount() ; i++ )
    {
        if( m_OptionDefs[m_Options[i].OptionDefIndex].Name == o )
        {
            Found = i;
        }
    }

    // Return Found Index;
    return Found;
}

//==============================================================================

const xstring& command_line::GetExecutableName( ) const
{
    return m_ExecutableName;
}

//==============================================================================

const xstring& command_line::GetOptionName( s32 Index ) const
{
    ASSERT( Index >= 0 );
    ASSERT( Index <  m_Options.GetCount() );

    return m_OptionDefs[m_Options[Index].OptionDefIndex].Name;
}

//==============================================================================

const xstring& command_line::GetOptionString( s32 Index ) const
{
    ASSERT( Index >= 0 );
    ASSERT( Index <  m_Options.GetCount() );

    return m_Options[Index].String;
}

//==============================================================================

const xstring& command_line::GetArgument( s32 Index ) const
{
    ASSERT( Index >= 0 );
    ASSERT( Index <  m_Arguments.GetCount() );

    return m_Arguments[Index];
}

//==============================================================================

void command_line::StoreOption( s32 iOption, xstring& String )
{
    option_entry& o = m_Options.Append();
    o.OptionDefIndex = iOption;
    o.String = String;
}

//==============================================================================

void command_line::StoreArgument( xstring& String )
{
    m_Arguments.Append() = String;
}

//==============================================================================

void command_line::AddOptionDef( xstring Option, option_type OptionType )
{
    option_def& o = m_OptionDefs.Append();
    Option.MakeUpper();
    o.Name = Option;
    o.Type = OptionType;
}

//==============================================================================

xbool command_line::ReadResponseFile( xstring& PathName, xarray<xstring>& Args )
{
    xbool   Success;
    xstring r;
    xstring a;
    s32     iStart;
    s32     iEnd;
	s32		iNext;

    // Load file into string
    Success = r.LoadFile( PathName );
    if( Success )
    {
        // Get Length of string
        s32 Length = r.GetLength();

        // Process the file into seperate arguments
        iStart = 0;
        while( iStart<Length )
        {
            // Skip Whitespace
            while( (iStart<Length) && x_isspace(r[iStart]) )
            {
                iStart++;
            }

			// At end of string?
			if( iStart<Length )
			{
				// Find limits of string
				iEnd = iStart+1;
				if( r[iStart] == '"' )
				{
					iStart++;
					while( (iEnd<Length) && (r[iEnd] != '"') )
						iEnd++;
					iNext = iEnd+1;
				}
				else
				{
					while( (iEnd<Length) && !x_isspace(r[iEnd]) )
						iEnd++;
					iNext = iEnd;
				}

				// Add to argument array
				Args.Append() = r.Mid( iStart, iEnd-iStart );

				// Set start past end of string
				iStart = iNext;
			}
        }
    }

    // Return success code
    return Success;
}

//==============================================================================

xbool command_line::Parse( int argc, char** argv )
{
    xbool           NeedHelp = TRUE;
    xarray<xstring> Args;
    s32             i;
    s32             iOption;

    // Need at least 1 argument
    if( argc > 0 )
    {
        // Grab executable name
        m_ExecutableName = *argv++;
        argc--;

        // Setup Args from command line
        while( argc-- > 0 )
        {
            Args.Append() = *argv++;
        }

        if( Args.GetCount() > 0 )
        {
            // Clear Help Indicator
            NeedHelp = FALSE;

            // Process Args
            for( i=0 ; i<Args.GetCount() ; )
            {
                // Read Argument
                xstring  a = Args[i++];
                xstring au = a;
                au.MakeUpper();

                // Check for Help?
                if( (au == "?") || (au == "-?") || (au == "-HELP") )
                {
                    NeedHelp = TRUE;
                }
                // Check for response file
                else if( a[0] == '@' )
                {
					// Read and insert the response file
					xarray<xstring> Response;
					if( ReadResponseFile( a.Right(a.GetLength()-1), Response ) )
						Args.Insert( i, Response );
					else
						NeedHelp = TRUE;
                }
                // Check for an option
                else if( a[0] == '-' )
                {
                    xbool   Found = FALSE;

                    // Remove leading '-'
                    au = au.Right( au.GetLength()-1 );

                    // Search for option and read value into option list
                    for( iOption=0 ; iOption<m_OptionDefs.GetCount() ; iOption++ )
                    {
                        // Check if found
                        if( m_OptionDefs[iOption].Name == au )
                        {
                            xstring OptionValue;

                            Found = TRUE;

                            // Switch on type
                            switch( m_OptionDefs[iOption].Type )
                            {
                            case SWITCH:
                                StoreOption( iOption, xstring("") );
                                break;
                            case STRING:
                                if( i < Args.GetCount() )
                                {
                                    StoreOption( iOption, Args[i++] );
                                }
                                break;
                            }
                            break;
                        }
                    }

                    // Check if option was found
                    if( !Found )
                        NeedHelp = TRUE;
                }
                // Must be an Argument
                else
                {
                    // Add to argument list
                    m_Arguments.Append() = a;
                }
            }
        }
    }

    // Return NeedHelp Flag
    return NeedHelp;
}

//==============================================================================

void command_line::SplitPath( const xstring& PathName, xstring& Path, xstring& File )
{
    s32 Index = PathName.GetLength()-1;
    while( (Index >= 0) && (PathName[Index] != '\\') && (PathName[Index] != '/') )
        Index--;

    if( Index >= 0 )
        Path = PathName.Left( Index );
    else
        Path = "";

    File = PathName.Right( PathName.GetLength() - (Index+1) );
}

//==============================================================================

s32 command_line::Glob( const xstring& Pattern, xarray<xstring>& Results, xbool Recursive )
{
	_finddata_t	fd;

	// TODO: Add recursive capabilitied
/*
	xstring Path;
	xstring	File;
	SplitPath( Pattern, Path, File );
    x_printf( "'%s' - '%s':'%s'\n", (const char*)Pattern, (const char*)Path, (const char*)File );
*/

	// Begin find
	s32 handle = _findfirst( Pattern, &fd );
	if( handle != -1 )
	{
		// Add first file into list
		if( !(fd.attrib & _A_SUBDIR) )
			Results.Append() = fd.name;

		// Loop through rest of matches
		while( _findnext( handle, &fd ) == 0 )
		{
			// Add file
			if( !(fd.attrib & _A_SUBDIR) )
				Results.Append() = fd.name;
		}
	}

	// Return number of files found
	return Results.GetCount();
}

//==============================================================================
