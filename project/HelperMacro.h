#ifndef GUARD_HELPERMACRO_H
#define GUARD_HELPERMACRO_H
///
/// @file : HelperMacro.h
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-09 21:22
/// @description : Global macro API file
///

#define NOTUSE(__Expression__) ((void)__Expression__)
#define NAMESPACE(__Specifier__) __Specifier__
#define PARSE1(__LHS__, __RHS__) __LHS__ ## __RHS__
#define PARSE2(__LHS__, __RHS__) PARSE1(__LHS__, __RHS__)

#define M_INPUT_ARRAY(__Array__) __Array__, (sizeof(__Array__) / sizeof(__Array__[0]))

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BOOL
#define BOOL int
#endif

#endif /// GUARD_HELPERMACRO_H
