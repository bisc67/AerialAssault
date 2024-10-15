#include "x_files.hpp"
#include "netlib.hpp"
#include "e_netfs.hpp"
#include "e_netfsdefs.hpp"

#include "windows.h"

u8  ReceiveBuffer[MAX_PACKET_SIZE];

typedef struct s_open_file
{
    struct s_open_file *pNext;
    char    Filename[128];
    X_FILE    *Handle;
} open_file;

open_file *FileList=NULL;

void AddOpenFile(open_file *pOpenFile)
{
    pOpenFile->pNext = FileList;
    FileList = pOpenFile;
}

void DeleteOpenFile (open_file *pOpenFile)
{
    if (pOpenFile == FileList)
    {
        FileList = pOpenFile->pNext;
    }
    else
    {
        open_file *pPrev;
        pPrev = FileList;
        while (pPrev->pNext != pOpenFile)
        {
            pPrev = pPrev->pNext;
            ASSERT(pPrev);
        }
        pPrev->pNext = pOpenFile->pNext;
    }
}

open_file *FindOpenFile(s32 Handle)
{
    open_file *pOpenFile;

    pOpenFile = FileList;

    while (pOpenFile)
    {
        if (pOpenFile->Handle == (X_FILE *)Handle)
            return pOpenFile;
        pOpenFile = pOpenFile->pNext;
    }
    return NULL;
}

int main( int argc, char** argv )
{

    net_address         Server;
    net_address         Client;
    interface_info      info;
    s32                 Length;
    xbool               status;
    fileserver_request  Request;
    fileserver_reply    Reply;
    open_file           *pOpenFile;
    s32                 count;
    char                OpenMode[4];
    char                *pMode,*pNewMode;
    s32                 ClientIp;
    char                ClientName[128];
    xbool               ReadEnabled;
    xbool               WriteEnabled;

    x_Init();
    net_Init();

    x_DebugMsg("server_manager: Waiting for IP address to be assigned...\n");
    while (1)
    {
        s32 i;

        net_GetInterfaceInfo(0,info);
        if (info.Address != 0)
            break;
        for (i=0;i<100000;i++);
    }
    x_DebugMsg("server_manager: Interface attached, IP = %d\n",info.Address);

    ReadEnabled = TRUE;
    WriteEnabled=TRUE;
    

    while (1)
    {
        const char *pArg;
        if (argc > 1)
        {
            pArg = argv[1];

            if (*pArg=='-')
            {
                pArg++;
                if (x_strcmp(pArg,"disablewrite")==0)
                {
                    WriteEnabled = FALSE;
                    x_printf("**** WRITE DISABLED ****\n");
                }
                else if (x_strcmp(pArg,"disableread")==0)
                {
                    ReadEnabled = FALSE;
                    x_printf("**** READ DISABLED ****\n");
                }
                else
                {
                    x_printf("Unknown command line option -'%s'\n",pArg);
                }
                argc--;
                argv++;
            }
			else
			{
				break;
			}
        }
        else
        {
            break;
        }
    }

    if (argc==2)
    {
        x_strcpy(ClientName,argv[1]);
        ClientIp = net_ResolveName(ClientName);
        if (ClientIp == 0)
        {
            x_DebugMsg("Warning: Cannot resolve IP address of client in command line args, will respond to anyone.\n");
        }
    }
    else
    {
        DWORD   Type;
        BYTE    Data[128];
        DWORD   Length;
        HKEY    hKey;
        s32     status;

        status = RegOpenKeyEx(  HKEY_CURRENT_USER,
                                "Software\\Microsoft\\XBoxSDK",
                                NULL,
                                KEY_QUERY_VALUE,
                                &hKey);
        if (status==0)
        {
            Length = sizeof(Data);
            RegQueryValueEx(hKey,
                            "XBoxName",
                            NULL,
                            NULL,
                            Data,
                            &Length);
            x_DebugMsg("RegQueryValueEx returned %s\n",Data);
            RegCloseKey(hKey);
        }
        else
        {
            x_DebugMsg("Unable to open registry key");
        }

        ClientIp = 0;
        x_strcpy(ClientName,"anyone");
    }
    net_BindBroadcast(Server,FILESERVER_PORT);
    Server.SetBlocking(TRUE);

    x_printf("Fileserver initialized on port %d, waiting for connections from %s.\n",Server.Port,ClientName);
    while(1)
    {

        Client.Port = Server.Port;
        Client.IP   = (u32)-1;
        status = net_Receive(Server,Client,&Request,Length);
        if (status)
        {
//            x_DebugMsg("Received: Type=%d, client=%08x:%d,Length=%d\n",Request.Header.Type,Client.IP,Client.Port,Length);
            switch(Request.Header.Type)
            {
            case FSRV_REQ_FIND:
                net_IPToStr(Request.Find.Address,ClientName);
                x_printf("A client at address %s:%d is looking for a server.\n",ClientName,Request.Find.Port);
                if ( (Request.Find.Address == ClientIp) || (ClientIp == 0) )
                {
                    Reply.Find.Address  = Server.IP;
                    Reply.Find.Port     = Server.Port;
                    Length = sizeof(Reply.Find);
                }
                else
                {
                    Length = 0;
                }
                break;
//----------------------------------------------------------------------------
            case FSRV_REQ_RESET:
                count = 0;
                while (FileList)
                {
                    x_fclose(FileList->Handle);
                    DeleteOpenFile(FileList);
                    count++;
                }
                x_printf("Fileserver reset, closed %d files.\n",count);
                Length = sizeof(Reply.Reset);
                break;
//----------------------------------------------------------------------------
            case FSRV_REQ_OPEN:
                xbool skip;

                pOpenFile = (open_file *)x_malloc(sizeof(open_file));
                ASSERT(pOpenFile);
                x_printf("Open file %s, mode %s,",Request.Open.Filename,Request.Open.Mode);
                x_strcpy(pOpenFile->Filename,Request.Open.Filename);

                pMode = Request.Open.Mode;
                pNewMode = OpenMode;
                skip = FALSE;
                while (*pMode)
                {
                    switch (*pMode)
                    {
                    case 'R':
                    case 'r':
                        if (!ReadEnabled)
                        {
                            skip = TRUE;
                        }
                        *pNewMode++='r';
                        break;
                    case 'W':
                    case 'w':
                        if (!WriteEnabled)
                        {
                            skip = TRUE;
                        }
                        *pNewMode++='w';
                        break;
                    case 'A':
                    case 'a':
                        *pNewMode++='A';
                        break;
                    case 'B':
                    case 'b':
                        *pNewMode++='b';
                        break;
                    case 'T':
                    case 't':
                        *pNewMode++='b';
                        break;
                    default:
                        break;
                    }
                    pMode++;
                }
                *pNewMode=0x0;

                if (skip)
                {
                    pOpenFile->Handle = NULL;
                }
                else
                {
                    pOpenFile->Handle = x_fopen(Request.Open.Filename,OpenMode);
                }

                if (!pOpenFile->Handle)
                {
                    Reply.Open.Status = FSRV_ERR_NOTFOUND;
                    x_free(pOpenFile);
                    x_printf("Not found.\n");
                }
                else
                {
                    AddOpenFile(pOpenFile);
                    Reply.Open.Handle = (s32)pOpenFile->Handle;
                    x_fseek(pOpenFile->Handle,0,X_SEEK_END);
                    Reply.Open.Length = x_ftell(pOpenFile->Handle);
                    x_fseek(pOpenFile->Handle,0,X_SEEK_SET);
                    Reply.Open.Status = FSRV_ERR_OK;
                    x_printf("Handle = 0x%x,length=%d\n",pOpenFile->Handle,Reply.Open.Length);
                }
                Length = sizeof(Reply.Open);
                break;
//----------------------------------------------------------------------------
            case FSRV_REQ_CLOSE:
                pOpenFile = FindOpenFile(Request.Close.Handle);
                if (pOpenFile)
                {
                    x_printf("Close file %s\n",pOpenFile->Filename);
                    x_fclose(pOpenFile->Handle);
                    DeleteOpenFile(pOpenFile);
                    x_free(pOpenFile);
                    Reply.Close.Status = FSRV_ERR_OK;
                }
                else
                {
                    x_printf("Close handle 0x%x - not open\n",Request.Close.Handle);
                    Reply.Close.Status = FSRV_ERR_NOTFOUND;
                }
                Length = sizeof(Reply.Close);
                break;
//----------------------------------------------------------------------------
            case FSRV_REQ_READ:
                pOpenFile = FindOpenFile(Request.Read.Handle);
                if (pOpenFile)
                {
                    x_fseek(pOpenFile->Handle,Request.Read.Position,X_SEEK_SET);
                    Reply.Read.Length = x_fread(Reply.Read.Data,1,Request.Read.Length,pOpenFile->Handle);
                    Reply.Read.Status = FSRV_ERR_OK;
//                    x_printf("Read handle 0x%x, offset %d, length %d\n",Request.Read.Handle,Request.Read.Position,Request.Read.Length);
                }
                else
                {
                    x_printf("Read handle 0x%x, ---- Not open\n",Request.Read.Handle);
                    Reply.Read.Length = 0;
                    Reply.Read.Status = FSRV_ERR_NOTOPEN;
                }
                Length = sizeof(Reply.Read);
                break;
//----------------------------------------------------------------------------
            case FSRV_REQ_WRITE:
                pOpenFile = FindOpenFile(Request.Write.Handle);
                if (pOpenFile)
                {
                    x_printf("Write handle 0x%x, offset %d, length %d\n",Request.Write.Handle,Request.Write.Position,Request.Write.Length);
                    x_fseek(pOpenFile->Handle,Request.Write.Position,X_SEEK_SET);
                    Reply.Write.Length = x_fwrite(Request.Write.Data,1,Request.Write.Length,pOpenFile->Handle);
                    Reply.Write.Status = FSRV_ERR_OK;
                }
                else
                {
                    x_printf("Write handle 0x%x, ---- Not open\n",Request.Write.Handle);
                    Reply.Write.Length = 0;
                    Reply.Write.Status = FSRV_ERR_NOTOPEN;
                }
                Length = sizeof(Reply.Write);
                break;
//----------------------------------------------------------------------------
             default:
                Reply.Close.Status = FSRV_ERR_BADCOMMAND;
                Length = sizeof(Reply.Close);
                x_DebugMsg("Invalid request type %d\n",Request.Header.Type);
                break;
            }
            if (Length)
            {
                Length = Length+sizeof(Reply.Header);
                Reply.Header.Sequence = Request.Header.Sequence;
                net_Send(Server,Client,&Reply,Length);
            }
//            x_DebugMsg("Sent: Client=%08x:%d, Length=%d,sequence = \n",Client.IP,Client.Port,Length,Reply.Header.Sequence);
        }
    }

    return 0;
}