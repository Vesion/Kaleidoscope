#include "parser.h"

// CurTok is the current token the parser is looking at.
int CurTok;

// getNextToken reads another token from the lexer and updates CurTok with its results.
int getNextToken() { return CurTok = gettok(); }

// BinopPrecedence - This holds the precedence for each binary operator that is defined.
std::map<char, int> BinopPrecedence;

// GetTokPrecedence - Get the precedence of the pending binary operator token.
int GetTokPrecedence() {
    if (!isascii(CurTok)) return -1;

    // Make sure it's a declared binop.
    int TokPrec = BinopPrecedence[CurTok];
    return TokPrec <= 0 ? -1 : TokPrec;
}

// LogError* - These are little helper functions for error handling.
Expr_Type LogError(const char *Str) { fprintf(stderr, "Error: %s\n", Str); return nullptr; }
Proto_Type LogErrorP(const char *Str) { LogError(Str); return nullptr; } 


/// Parsing

// numberexpr ::= number
Expr_Type ParseNumberExpr() {
    auto Result = helper::make_unique<NumberExprAST>(NumVal);
    getNextToken(); // consume the number
    return std::move(Result);
}

// parenexpr ::= '(' expression ')'
Expr_Type ParseParenExpr() {
    getNextToken(); // eat '('
    auto V = ParseExpression();
    if (!V) return nullptr;

    if (CurTok != ')') return LogError("expected ')'");
    getNextToken(); // eat ')'
    return V;
}

// identifierexpr
//   ::= identifier
//   ::= identifier '(' expression* ')'
Expr_Type ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    getNextToken(); // eat identifier

    if (CurTok != '(') // simple variable ref
        return helper::make_unique<VariableExprAST>(IdName);

    // Call.
    getNextToken(); // eat '('
    std::vector<Expr_Type> Args;
    if (CurTok != ')') {
        while (true) {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else return nullptr;

            if (CurTok == ')') break;

            if (CurTok != ',') return LogError("Expected ')' or ',' in argument list");
            getNextToken(); // eat ','
        }
    }

    getNextToken(); // eat the ')'.
    return helper::make_unique<CallExprAST>(IdName, std::move(Args));
}

// primary
//   ::= identifierexpr
//   ::= numberexpr
//   ::= parenexpr
Expr_Type ParsePrimary() {
    switch (CurTok) {
        default: return LogError("unknown token when expecting an expression");
        case tok_identifier: return ParseIdentifierExpr();
        case tok_number: return ParseNumberExpr();
        case '(': return ParseParenExpr();
    }
    return nullptr;
}

// binoprhs
//   ::= [binop, primary]*
Expr_Type ParseBinOpRHS(int ExprPrec, Expr_Type LHS) {
    // If this is a binop, find its precedence.
    while (true) {
        int TokPrec = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the current binop, consume it,
        // otherwise we are done.
        if (TokPrec < ExprPrec) return LHS;

        // Okay, we know this is a binop.
        int BinOp = CurTok;
        getNextToken(); // eat binop

        // Parse the primary expression after the binary operator.
        auto RHS = ParsePrimary();
        if (!RHS) return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, 
        // let the pending operator take RHS as its LHS.
        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS) return nullptr;
        }

        // Merge [LHS, binop, RHS] as new LHS.
        LHS = helper::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

// expression
//   ::= primary binoprhs
Expr_Type ParseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS) return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

// prototype
//   ::= id '(' id* ')'
Proto_Type ParsePrototype() {
  if (CurTok != tok_identifier) return LogErrorP("Expected function name in prototype");

  std::string FnName = IdentifierStr;
  getNextToken(); // eat identifier

  if (CurTok != '(') return LogErrorP("Expected '(' in prototype");

  // Read the list of parameter names.
  std::vector<std::string> ArgNames;
  while (getNextToken() == tok_identifier)
    ArgNames.push_back(IdentifierStr);
  if (CurTok != ')') return LogErrorP("Expected ')' in prototype");

  getNextToken();  // eat ')'.

  return helper::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

// definition ::= 'def' prototype expression
Func_Type ParseDefinition() {
    getNextToken(); // eat def.
    auto Proto = ParsePrototype();
    if (!Proto) return nullptr;

    if (auto E = ParseExpression())
        return helper::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

// external ::= 'extern' prototype
Proto_Type ParseExtern() {
    getNextToken(); // eat extern.
    return ParsePrototype();
}

// toplevelexpr ::= expression
Func_Type ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        // Make an anonymous proto.
        auto Proto = helper::make_unique<PrototypeAST>("__anony_proto__", std::vector<std::string>());
        return helper::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}


/// Top-Level parsing

void HandleDefinition() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        getNextToken(); // Skip token for error recovery.
    }
}

void HandleExtern() {
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        
        getNextToken(); // Skip token for error recovery.
    }
}

void HandleTopLevelExpression() {
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        getNextToken(); // Skip token for error recovery.
    }
}

// top ::= definition | external | expression | ';'
void MainLoop() {
    while (1) {
        fprintf(stderr, "ready> ");
        switch (CurTok) {
            case tok_eof: return;
            case ';': getNextToken(); break; // ignore top-level semicolons.;
            case tok_def: HandleDefinition(); break;
            case tok_extern: HandleExtern(); break;
            default: HandleTopLevelExpression(); break;
        }
    }
}
