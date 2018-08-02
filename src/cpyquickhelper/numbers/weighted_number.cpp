#include "weighted_number.h"

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

const char * get_weighted_class_name(double x) { return "Double"; }
const char * get_weighted_class_name(float x) { return "Float"; }
const char * get_weighted_class_name(int x) { return "Int"; }


