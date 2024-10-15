/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jan 14 21:21:32 2003
 */
/* Compiler settings for C:\Projects\AADS\xCore\Projects\xTB\xTB.odl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_XTB = {0x1E241520,0x7843,0x485E,{0x8B,0x44,0x45,0xEC,0xD9,0x4E,0x52,0x38}};


const IID IID_ICommands = {0x424A23B1,0xB766,0x4308,{0x9E,0xDA,0xC6,0x17,0xB2,0x15,0xB1,0x03}};


const CLSID CLSID_Commands = {0xE0F85A49,0xA880,0x4280,{0x93,0x9E,0x9C,0x1C,0x25,0xA2,0x7F,0x47}};


const CLSID CLSID_ApplicationEvents = {0x442BA762,0x2E4A,0x44A0,{0xB9,0x94,0x20,0x36,0x0A,0xFD,0x0D,0x93}};


const CLSID CLSID_DebuggerEvents = {0x82C762B1,0x1279,0x4130,{0xB6,0xFC,0xB1,0x58,0x06,0xCB,0x84,0x10}};


#ifdef __cplusplus
}
#endif

