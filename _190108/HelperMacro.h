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

#endif /// GUARD_HELPERMACRO_H
