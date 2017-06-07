#ifndef BI2PHASES_HPP
#define BI2PHASES_HPP

// #include "vopt.h"
#ifdef __cplusplus
extern "C"
#endif
void bi2phases( int nb_obj, int sz, int *obj1, int *obj2, int *weigths ,int capacity ,int * &z1, int * &z2, int * &solW, bool * &res, int &nbSol);
#endif

