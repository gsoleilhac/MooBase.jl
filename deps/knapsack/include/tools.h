/*! \file tools.c
  * Utility routines
  */
#ifndef TOOLS_H
#define TOOLS_H

/** \fn void passert( int test, char * message )
  * \brief assertion routine
  * \param[in] test boolean test
  * \param[in] message exit message
  */
void 
passert( int , char* );

/** \fn void * palloc( int num, size_t size )
  * \brief safe allocation routine
  * \param[in] num number of elements
  * \param[in] size size of an element
  */
void* 
palloc( int, size_t );

#endif

