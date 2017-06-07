#include <stdio.h>
#include <stdlib.h>

#include "tools.h"

void
passert ( int test, char * message )
{
   if( test ){
      fputs( message, stderr );
      exit( EXIT_FAILURE );
   }   
}

void *
palloc( int num, size_t size )
{
   void * res = calloc( num, size );
   passert( res == NULL, "Error memory allocation\n" );
   return res;   
}
