#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stop_sim() {
  (*(volatile unsigned short *)0xffff7ffc) = 0;
  (*(volatile unsigned short *)0xffff7ffe) = 0;
  __asm("halt");
}

void myputc(int i) {
  (*(volatile char *)0xffff7fe0) = (char)i;
}

const char *a(void);
const char *b(void);
void CallFuncs( const char *pcTrigger );

typedef struct {
    int m_nCh;
    const char *(*m_pFunc)(void); /* 関数ポインタの宣言 */
} FunctionMap;

FunctionMap fs_pMap[] = {
    { 'a', a },
    { 'b', b },
    {  -1, 0 }
};

int main(void)
{
    CallFuncs( "abaababb" );
    exit(-1);
    stop_sim();
}

void CallFuncs( const char *pcTrigger )
{
  int l;
  char buf[128];
  const char *pc = pcTrigger;
   for ( ; *pc!='\0'; pc++ ) {
        const FunctionMap *pMap = fs_pMap;
        for ( ; pMap->m_nCh!=-1; pMap++ ) {
            if ( pMap->m_nCh == *pc ) {
              strcpy(buf,pMap->m_pFunc()); /* 関数ポインタによる呼び出し */
              for (l=0; l<strlen(buf); l++)
                myputc( buf[l] );;
              break;
            }
        }
    }
}

const char *a(void)
{
    return "Called a().";
}

const char *b(void)
{
    return "Called b().";
}
