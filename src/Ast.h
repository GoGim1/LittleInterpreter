#pragma once
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include "Token.h"

#include <iostream>
using namespace std;

namespace Ast
{
    using namespace Lexer;

    using std::shared_ptr;
    using std::string;
    using std::vector;
    using std::pair;
    using std::make_shared;

    class AstNode //: public std::enable_shared_from_this<AstNode>
    {
    public:
        AstNode() {}
        virtual         ~AstNode() {}
        virtual string  Dump() const = 0;
    };

    class PrimaryNode;
    class FactorNode;
    class ExprNode;
    class BlockNode;
    class SimpleNode;
    class StatementNode;
    class ProgramNode;


    using TokenPtr      = shared_ptr<Token>;
    using PrimaryPtr    = shared_ptr<PrimaryNode>;
    using FactorPtr     = shared_ptr<FactorNode>;
    using ExprPtr       = shared_ptr<ExprNode>;
    using BlockPtr      = shared_ptr<BlockNode>;
    using SimplePtr     = shared_ptr<SimpleNode>;
    using StatementPtr  = shared_ptr<StatementNode>;
    using ProgramPtr    = shared_ptr<ProgramNode>;

    class PrimaryNode : public AstNode
    {
    public:
        //TODO: const reference
        PrimaryNode(const ExprPtr& pExprRhs, const TokenPtr& pTokenRhs) : pExpr(pExprRhs), pToken(pTokenRhs) {}
        virtual                 ~PrimaryNode() {}
        // (Debug)
        virtual string          Dump() const override;

    private:
        ExprPtr                 pExpr   = nullptr;
        TokenPtr                pToken  = nullptr;

    };


    class FactorNode : public AstNode
    {
    public:
        FactorNode(const PrimaryPtr& pPrimaryRhs, const TokenPtr& pTokenRhs): pPrimary(pPrimaryRhs), pToken(pTokenRhs) {}
        virtual             ~FactorNode() {}
        // (Debug)
        virtual string      Dump() const override;

    private:
        PrimaryPtr          pPrimary    = nullptr;
        TokenPtr            pToken      = nullptr;
    };


    class ExprNode : public AstNode
    {
    public:
        typedef pair<TokenPtr, FactorPtr>   Pair;
        typedef vector<Pair>                List;

        ExprNode(const FactorPtr& pFactorRhs): pFactor(pFactorRhs) {}
        virtual             ~ExprNode() {}
        // (Debug)
        virtual string      Dump() const override;

        void                ListHandler(const TokenPtr&, const FactorPtr&);
    private:
        FactorPtr           pFactor     = nullptr;
        List                factorList;
    };


    class BlockNode : public AstNode
    {
    public:
        typedef vector<StatementPtr>                    List;

        BlockNode(const StatementPtr& pStatementRhs): pStatement(pStatementRhs) {}
        virtual                 ~BlockNode() {}
        // (Debug)
        virtual string          Dump() const override;
        void                    ListHandler(const StatementPtr&);
    private:
        StatementPtr            pStatement;
        List                    statementList;
    };


    class SimpleNode : public AstNode
    {
    public:
        SimpleNode(const ExprPtr& pExprRhs): pExpr(pExprRhs) {}
        virtual             ~SimpleNode() {}
        // (Debug)
        virtual string      Dump() const override;

    private:
        ExprPtr             pExpr   = nullptr;
    };



    class StatementNode : public AstNode
    {
    public:
        enum Type { IF, IFELSE,WHILE, SIMPLE };

        StatementNode(const Type& typeRhs, const ExprPtr& pExprRhs, const BlockPtr& pBlockRhs, const BlockPtr& pBlockOfElseRhs, const SimplePtr& pSimpleRhs):
            type(typeRhs), pExpr(pExprRhs), pBlock(pBlockRhs), pBlockOfElse(pBlockOfElseRhs), pSimple(pSimpleRhs) {} 
        virtual                 ~StatementNode() {}
        // (Debug)
        virtual string          Dump() const override;

    private:

        Type                    type;
        ExprPtr                 pExpr           = nullptr;
        BlockPtr                pBlock          = nullptr;
        BlockPtr                pBlockOfElse    = nullptr;        
        SimplePtr               pSimple         = nullptr;
    };


    class ProgramNode : public AstNode
    {
    public:
        typedef vector<StatementPtr> List;

        ProgramNode() {}
        virtual             ~ProgramNode() {}
        // (Debug)
        virtual string      Dump() const override;
        void                ListHandler(const StatementPtr&);
    private:
        List                statementList;
    };
}