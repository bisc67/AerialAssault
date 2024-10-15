#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
typedef struct rc4_key
{      
   unsigned char state[256];       
   unsigned char x;        
   unsigned char y;
} rc4_key;

#define swap_byte(x,y) t = *(x); *(x) = *(y); *(y) = t

unsigned int net_StrToIP( const char* pStr )
{
    unsigned int IP=0;

    for( int i=0; i<4; i++ )
    {
        unsigned int D=0;
        char C = *pStr++;
        while( (C>='0') && (C<='9') )
        {
            D = D*10 + (C-'0');
            C = *pStr++;
        }
        IP |= (D<<(i*8));
    }

    return IP;
}


void prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key)
{
  unsigned char t;
  unsigned char index1;
  unsigned char index2;
  unsigned char* state;
  short counter;

  state = &key->state[0];
  for(counter = 0; counter < 256; counter++)
  state[counter] = (unsigned char)counter;
  key->x = 0;
  key->y = 0;
  index1 = 0;
  index2 = 0;
  for(counter = 0; counter < 256; counter++)
  {
    index2 = (key_data_ptr[index1] + state[counter] + index2) % 256;
    swap_byte(&state[counter], &state[index2]);
    index1 = (index1 + 1) % key_data_len;
  }
}

void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *key)
{
  unsigned char t;
  unsigned char x;
  unsigned char y;
  unsigned char* state;
  unsigned char xorIndex;
  short counter;

  x = key->x;
  y = key->y;
  state = &key->state[0];
  for(counter = 0; counter < buffer_len; counter++)
  {
    x = (x + 1) % 256;
    y = (state[x] + y) % 256;
    swap_byte(&state[x], &state[y]);
    xorIndex = (state[x] + state[y]) % 256;
    buffer_ptr[counter] ^= state[xorIndex];
  }
  key->x = x;
  key->y = y;
}

int main(int argc, char* argv[])
{
	int ip;
	char buffer[12];
	rc4_key key;

	if (argc!=4)
	{
		printf(	"Usage: makekey <key> <ip> <extra>\n"
				"      Where <key> is 4 characters\n"
				"      <ip> is in the form nnn.nnn.nnn.nnn\n"
				"      <extra> is a 4 character identifier\n"
				);
		return -1;
	}

	memset(buffer,0,sizeof(buffer));

	ip = net_StrToIP(argv[2]);
	if (ip==0)
	{
		printf("Invalid ip address '%s'\n",argv[2]);
		return -1;
	}

	if ( (strlen(argv[1])!=4) || (strlen(argv[3])!=4) )
	{
		printf(	"Usage: makekey <key> <ip> <extra>\n"
				"      Where <key> is 4 characters\n"
				"      <ip> is in the form nnn.nnn.nnn.nnn\n"
				"      <extra> is a 4 character identifier\n"
				);
		printf("Error: Need 4 characters for <key> and <extra>\n");
		exit(-1);
	}

	int identifier = *(int*)argv[3];
	int* pBuff32 = (int*)buffer;

	pBuff32[0] = identifier;
	pBuff32[1] = ip;
	pBuff32[2] = ip^identifier;

	printf("Key=0x%08x, IP=0x%08x, Extra=0x%08x\n",*(int*)argv[1],ip,identifier);

	prepare_key((unsigned char*)argv[1],sizeof(int),&key);
	rc4((unsigned char*)buffer,sizeof(buffer),&key);
	printf("Result=");
	for (int i=0;i<sizeof(buffer);i++)
	{
		printf("%02x",(unsigned char)buffer[i]);
	}
	printf("\n");
	return 0;
}