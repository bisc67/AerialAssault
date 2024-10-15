//==============================================================================
//
//  cdfsBuild.cpp
//
//==============================================================================
//
//  CD File System Builder
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include <stdio.h>
#include "x_files.hpp"
#include "StringTable.hpp"
#include "CommandLine\CommandLine.hpp"

//==============================================================================
//  STRUCTURES
//==============================================================================

#define CDFS_VERSION    1                           // *** INCREMENT THIS EACH TIME STRUCT CHANGES ***
#define CDFS_MAGIC      'CDFS'                      // Magic Number

struct cdfs_header
{
    u32         Magic;                              // Magic string CDFS
    s32         Version;                            // Version Number
    s32         SectorSize;                         // Sector size in bytes
    s32         RecommendedCacheSize;               // Recommended cache size
    s32         FirstSectorOffset;                  // Offset to first data sector in cdfs file
    s32         TotalSectors;                       // Total sectors in filesystem
    s32         FileTableLength;                    // Length of FileTable in bytes
    s32         FileTableEntries;                   // Length of FileTable in entries
    s32         StringTableLength;                  // Length of StringTable in bytes
    s32         StringTableEntries;                 // Length of StringTable in entries
};

struct cdfs_entry
{
    s32         FileNameOffset;                     // Offset into StringTable
    s32         DirNameOffset;                      // Offset into StringTable
    s32         StartSector;                        // Start sector for file data
    s32         Length;                             // Length of file in bytes
};

//==============================================================================
//  VARIABLES
//==============================================================================

#define FILE_BUFFER_SIZE    (16*1024*1024)

static s32                      ScriptIndex = 0;

static s32                      SectorSize;
static s32                      CacheSize;

static s32                      TotalDataSize = 0;
static s32                      TotalDataWritten = 0;
static u8                       FileBuffer[FILE_BUFFER_SIZE];

//==============================================================================

class cdfs_builder
{
protected:
    struct file
    {
        xstring     PathName;
        cdfs_entry  Record;
    };

protected:
    xbool               m_FromBinary;                                               // TRUE if from binary cdfs
    xstring             m_FileName;                                                 // Filename read from

    cdfs_header         m_Header;                                                   // Header
    xarray<file>        m_FileTable;                                                // File table
    string_table        m_StringTable;                                              // String table

protected:
    void                Initialize          ( void );                               // Initialize cdfs

public:
                        cdfs_builder        ( void );
                       ~cdfs_builder        ( void );

    xbool               Create              ( void );                               // Create a blank cdfs
    xbool               CreateFromScript    ( xstring& FileName );                  // Create from a script file
    xbool               CreateFromCDFS      ( xstring& FileName );                  // Read from a cdfs file

    xbool               WriteCDFS           ( xstring& FileName );                  // Write to new cdfs file

    void                SetSectorSize       ( s32 SectorSize );                     // Set Sector Size in Header
    void                SetCacheSize        ( s32 CacheSize );                      // Set Cache Size in Header

    s32                 AppendFile          ( xstring& FileName );                  // Append a file to the cdfs

    s32                 FindFile            ( xstring& FileName );                  // Find a file by name
    s32                 GetFileStartSector  ( s32 Index );                          // Get Start Sector by index
    s32                 GetFileLength       ( s32 Index );                          // Get File Length by index

    s32                 ReadFile            ( s32 Index, void* pBuffer, s32 Len );  // Read Bytes from file

};

//==============================================================================


//==============================================================================

xstring ReadLine( xstring& String )
{
    s32 StartIndex = ScriptIndex;
    xstring Line;

    while( (ScriptIndex < String.GetLength()) && (String[ScriptIndex] != '\n') )
    {
        ScriptIndex++;
    }
    Line = String.Mid( StartIndex, ScriptIndex-StartIndex );
    ScriptIndex++;

    // Strip Leading and Trailing whitespace
    s32 Index1 = 0;
    s32 Index2 = Line.GetLength()-1;
    while( x_isspace(Line[Index1]) && (Index1<Index2) ) Index1++;
    while( x_isspace(Line[Index2]) && (Index1<Index2) ) Index2--;
    Line = Line.Mid( Index1, Index2-Index1+1 );

    // Return Line
    return Line;
}

//==============================================================================

/*

xbool ReadScript( const char* pScriptName )
{
    xbool   Success = FALSE;

    // Load script file
    xstring Script;
    if( Script.LoadFile( pScriptName ) )
    {
        // Read script header
//        OutputPathName = ReadLine( Script );
//        SectorSize     = x_atoi(ReadLine(Script));
//        CacheSize      = x_atoi(ReadLine(Script));
//        OutputPathName = "FILES.DAT";
        SectorSize = 2048;
        CacheSize = 128*1024;

        // Loop through all files
        while( ScriptIndex < Script.GetLength() )
        {
            // Read File Entry and convert forward to backward slashes
            xstring PathName = ReadLine( Script );
            {
                s32 i;
                for( i=0 ; i<PathName.GetLength() ; i++ )
                {
                    if( PathName[i] == '/' ) PathName.SetAt(i, '\\');
                }
            }

            if( PathName.GetLength() > 0 )
            {
                // Open file
                X_FILE* pFile = x_fopen( PathName, "rb" );
                if( pFile )
                {
                    // Create New FileTableEntry
                    FileTableEntry* pEntry = new FileTableEntry; 
                    ASSERT( pEntry );
                    if( pEntry )
                    {
                        // Get File Length
                        x_fseek( pFile, 0, X_SEEK_END );
                        s32 Len = x_ftell( pFile );
                        x_fseek( pFile, 0, X_SEEK_SET );

                        // Setup Structure
                        pEntry->PathName = PathName;
                        pEntry->Length   = Len;

                        // Add Entry to Array
                        FileTable.Append() = pEntry;

                        // Display details
//                        x_printf( "FILE:  %10d bytes - \"%s\"\n", pEntry->Length, (char*)pEntry->PathName );
                    }

                    // Close file
                    x_fclose( pFile );
                }
                else
                {
                    // Display error on file open
                    x_printf( "ERROR: Can't open File \"%s\"\n", (const char*)PathName );
                }
            }
        }

        x_printf( "\n" );

        // Success!
        Success = TRUE;
    }
    else
    {
        // Failed to open script file
        x_printf( "ERROR: loading script file\n" );
    }

    return Success;
}

*/

//==============================================================================
//  main
//==============================================================================

int main( int argc, char** argv )
{
    command_line    CommandLine;
    xbool           NeedHelp;

//    s32 CurrentSector = 0;
//    s32 i;
//    s32 j;

    // Setup recognized command line options
    CommandLine.AddOptionDef( "SWITCH" );
    CommandLine.AddOptionDef( "STRING", command_line::STRING );

    // Parse command line
    NeedHelp = CommandLine.Parse( argc, argv );
    if( NeedHelp )
    {
        x_printf( "usage: cdfsTool <script> <binary>\n" );
        return 10;
    }

    x_printf( "\"%s\"\n", CommandLine.GetExecutableName() );
    x_printf( "NumOptions = %d\n", CommandLine.GetNumOptions() );
    for( s32 i=0 ; i<CommandLine.GetNumOptions() ; i++ )
    {
        x_printf( " \"%s\" \"%s\"\n", CommandLine.GetOptionName(i), CommandLine.GetOptionString(i) );
    }
    x_printf( "NumArguments = %d\n", CommandLine.GetNumArguments() );
    for( i=0 ; i<CommandLine.GetNumArguments() ; i++ )
    {
        x_printf( " \"%s\"\n", CommandLine.GetArgument(i) );
    }

	xarray<xstring> Results;
	s32 NumFiles = CommandLine.Glob( CommandLine.GetArgument(0), Results, TRUE );
    for( i=0 ; i<Results.GetCount() ; i++ )
    {
        x_printf( "%s\n", Results[i] );
    }
	x_printf( "Num Files = %d\n", NumFiles );

/*
    if( CommandLine.Valid() )
    {
        // Read script and Create FileTable
        xbool Success = ReadScript( argv[1] );

        // Make a pass through the files setting up data to create cdfs
        for( i=0 ; i<FileTable.GetCount() ; i++ )
        {
            // Get pointer to file entry
            FileTableEntry* pFile = FileTable[i];
            ASSERT( pFile );

            // Setup Sector Offset for file
            pFile->StartSector = CurrentSector;
            CurrentSector += (pFile->Length+SectorSize-1) / SectorSize;
            TotalDataSize += pFile->Length;

            // Seperate Path and File & Add to Table
            xstring Path;
            xstring File;
            SplitPathName( pFile->PathName, Path, File );
            pFile->DirNameOffset  = AddString( Path );
            pFile->FileNameOffset = AddString( File );
        }

        // Open output file
        X_FILE* pFile = x_fopen( OutputPathName, "wb" );
        if( pFile )
        {
            cdfsHeader  Header;
            s32         nBytes;
            s32         PadBytes;
            s32         HeaderLen         = sizeof(cdfsHeader);
            s32         FileTableLen      = FileTable.GetCount() * sizeof(cdfsEntry);
            s32         StringTableLen    = StringTable.GetLength() ;
            s32         FirstSectorOffset;
            s32         FirstSectorOffsetPadded;

            // Get First Sector Offset, rounded up to next sector
            FirstSectorOffset = HeaderLen + FileTableLen + StringTableLen;
            FirstSectorOffsetPadded = FirstSectorOffset;
            if( (FirstSectorOffset % SectorSize) > 0 )
                FirstSectorOffsetPadded += SectorSize - (FirstSectorOffset % SectorSize);

            // Build and write cdfs Header
            Header.Magic                = CDFS_MAGIC;
            Header.Version              = CDFS_VERSION;
            Header.SectorSize           = SectorSize;
            Header.RecommendedCacheSize = CacheSize;
            Header.FirstSectorOffset    = FirstSectorOffsetPadded;
            Header.TotalSectors         = CurrentSector;
            Header.FileTableLength      = FileTable.GetCount() * sizeof(cdfsEntry);
            Header.FileTableEntries     = FileTable.GetCount();
            Header.StringTableLength    = StringTable.GetLength();
            Header.StringTableEntries   = StringTableEntries;

#if 0
            // Dump cdfsHeader details
            x_printf( "CDFS File            = \"%s\"\n", (char*)OutputPathName );
            x_printf( "\n" );
            x_printf( "Magic                = 0x%08X\n", Header.Magic );
            x_printf( "Version              = %d\n", Header.Version );
            x_printf( "SectorSize           = %d\n", Header.SectorSize );
            x_printf( "RecommendedCacheSize = %d\n", Header.RecommendedCacheSize );
            x_printf( "FirstSectorOffset    = %d\n", Header.FirstSectorOffset );
            x_printf( "TotalSectors         = %d\n", Header.TotalSectors );
            x_printf( "FileTableLength      = %d\n", Header.FileTableLength );
            x_printf( "FileTableEntries     = %d\n", Header.FileTableEntries );
            x_printf( "StringTableLength    = %d\n", Header.StringTableLength );
            x_printf( "StringTableEntries   = %d\n", Header.StringTableEntries );
            x_printf( "\n" );
#endif

            // Write cdfsHeader
            nBytes = x_fwrite( &Header, 1, sizeof(cdfsHeader), pFile );
            if( nBytes != sizeof(cdfsHeader) )
            {
                x_printf( "ERROR: writing Header\n" );
            }

            // Write File Table
            for( i=0 ; i<FileTable.GetCount() ; i++ )
            {
                cdfsEntry   Entry;

                // Build Entry
                Entry.DirNameOffset  = FileTable[i]->DirNameOffset;
                Entry.FileNameOffset = FileTable[i]->FileNameOffset;
                Entry.StartSector    = FileTable[i]->StartSector;
                Entry.Length         = FileTable[i]->Length;

                // Write Entry
                nBytes = x_fwrite( &Entry, 1, sizeof(cdfsEntry), pFile );
                if( nBytes != sizeof(cdfsEntry) )
                {
                    x_printf( "ERROR: writing FileTable Entry\n" );
                }
            }

            // Write String Table
            nBytes = x_fwrite( (const char*)StringTable, 1, StringTable.GetLength(), pFile );
            if( nBytes != StringTable.GetLength() )
            {
                x_printf( "ERROR: writing StringTable\n" );
            }

            // Pad out File Data
            for( j=0 ; j<(FirstSectorOffsetPadded - FirstSectorOffset) ; j++ )
            {
                u8 Pad = 0;
                if( x_fwrite( &Pad, 1, 1, pFile ) != 1 )
                {
                    x_printf( "ERROR: writing to file \"%s\"\n", OutputPathName );
                }
            }

            // Write File Data
            for( i=0 ; i<FileTable.GetCount() ; i++ )
            {
    //            x_printf( "%s - %08x\n", (const char*)FileTable[i]->PathName, (x_ftell(pFile)-0x1000)/0x800 );

                // Open File
                X_FILE* pSrcFile = x_fopen( FileTable[i]->PathName, "rb" );
                if( pSrcFile )
                {
                    s32 BytesLeft = FileTable[i]->Length;

                    // Copy File to output file
                    do
                    {
                        static s32  LastPercentageComplete = -1;
                        s32         PercentageComplete;
                        s32         BytesToCopy = MIN( BytesLeft, FILE_BUFFER_SIZE );

                        // Read Data
                        nBytes = x_fread( FileBuffer, 1, BytesToCopy, pSrcFile );
                        if( nBytes != BytesToCopy )
                        {
                            x_printf( "ERROR: reading from file \"%s\"\n", FileTable[i]->PathName );
                        }

                        // Write Data
                        nBytes = x_fwrite( FileBuffer, 1, BytesToCopy, pFile );
                        if( nBytes != BytesToCopy )
                        {
                            x_printf( "ERROR: writing to file \"%s\"\n", OutputPathName );
                        }

                        // Decrement Bytes left to copy
                        BytesLeft -= BytesToCopy;

                        // Increment TotalDataWritten
                        TotalDataWritten += BytesToCopy;

                        // Display % complete
                        PercentageComplete = (s32)(((f64)TotalDataWritten * 100) / TotalDataSize);
                        if( PercentageComplete != LastPercentageComplete )
                        {
                            LastPercentageComplete = PercentageComplete;
                            x_printf( "Building CDFS File - %3d%%\r", PercentageComplete );
                        }
                    } while( BytesLeft > 0 );

                    // Pad output to sector boundary
                    PadBytes = ((FileTable[i]->Length % SectorSize) > 0) ? (SectorSize - (FileTable[i]->Length % SectorSize)) : 0;
                    for( j=0 ; j<PadBytes ; j++ )
                    {
                        u8 Pad = 0;
                        if( x_fwrite( &Pad, 1, 1, pFile ) != 1 )
                        {
                            x_printf( "ERROR: writing to file \"%s\"\n", OutputPathName );
                        }
                    }

                    // Close file
                    x_fclose( pSrcFile );
                }
                else
                {
                    x_printf( "ERROR: can't open \"%s\" for reading\n", FileTable[i]->PathName );
                }
            }

            // Close file
            x_fclose( pFile );

            // Print done
            x_printf( "\n" );
            x_printf( "\n" );
            x_printf( "Success!\n" );
        }
        else
        {
            // Error opening file for output
            x_printf( "ERROR: opening \"%s\" for output\n", OutputPathName );
        }

        // Return success code
        return 0;
    }
*/
    return 0;
}
