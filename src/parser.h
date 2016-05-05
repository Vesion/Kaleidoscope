#ifndef KAL_PARSER_H
#define KAL_PARSER_H

#include <map>
#include <cstdio>
#include "token.h"
#include "ast.h"

extern int CurTok;
extern int getNextToken();

extern std::map<char, int> BinopPrecedence;

int GetTokPrecedence();

Expr_Type LogError(const char *Str);
Proto_Type LogErrorP(const char *Str);


Expr_Type ParseNumberExpr();
Expr_Type ParseParenExpr();
Expr_Type ParseIdentifierExpr();
Expr_Type ParsePrimary();

Expr_Type ParseBinOpRHS(int ExprPrec, Expr_Type LHS);
Expr_Type ParseExpression();
Proto_Type ParsePrototype();
Func_Type ParseDefinition();
Func_Type ParseTopLevelExpr();
Proto_Type ParseExtern();

void HandleDefinition();
void HandleExtern();
void HandleTopLevelExpression();
void MainLoop();

#endif
