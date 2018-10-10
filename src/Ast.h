#pragma once
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include "Token.h"

namespace Ast
{
    using namespace Lexer;

    using std::shared_ptr;
    using std::string;
    using std::vector;
    using std::pair;

    class AstNode //: public std::enable_shared_from_this<AstNode>
    {
    public:
		typedef shared_ptr<AstNode>	Ptr;

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


    using TokenPtr = unique_ptr<Token>;
    using PrimaryPtr = shared_ptr<PrimaryNode>;
    using FactorPtr = shared_ptr<FactorNode>;
    using ExprPtr = shared_ptr<ExprNode>;
    using BlockPtr = shared_ptr<BlockNode>;
    using SimplePtr = shared_ptr<SimpleNode>;
    using StatementPtr = shared_ptr<StatementNode>;
    using ProgramPtr = shared_ptr<ProgramNode>;


    class PrimaryNode : public AstNode
    {
    public:
        //TODO: const reference
        PrimaryNode(ExprPtr pExprRhs, TokenPtr pTokenRhs) : pExpr(pExprRhs), pToken(std::move(pTokenRhs)) {}
        virtual                 ~PrimaryNode() {}
        // (Debug)
        virtual string          Dump() const override;

    private:
        ExprPtr                 pExpr = nullptr;
        TokenPtr                pToken = nullptr;

    };


    class FactorNode : public AstNode
    {
    public:
        FactorNode(PrimaryPtr pPrimaryRhs, TokenPtr pTokenRhs): pPrimary(pPrimaryRhs), pToken(std::move(pTokenRhs)) {}
        virtual             ~FactorNode() {}
        // (Debug)
        virtual string      Dump() const override;

    private:
        PrimaryPtr          pPrimary = nullptr;
        TokenPtr            pToken = nullptr;
    };


    class ExprNode : public AstNode
    {
    public:
        typedef pair<TokenPtr, FactorPtr>   Pair;
        typedef vector<Pair>                List;

        ExprNode(FactorPtr pFactorRhs, List factorListRhs): pFactor(pFactorRhs), factorList(std::move(factorListRhs)) {}
        virtual             ~ExprNode() {}
        // (Debug)
        virtual string      Dump() const override;

        void                ListHandler(TokenPtr, FactorPtr);
    private:
        FactorPtr           pFactor = nullptr;
        List                factorList;
    };


    class BlockNode : public AstNode
    {
    public:
        typedef vector<StatementPtr>                    List;

        BlockNode(StatementPtr pStatementRhs, List statementListRhs): pStatement(pStatementRhs), statementList(std::move(statementListRhs)) {}
        virtual                 ~BlockNode() {}
        // (Debug)
        virtual string          Dump() const override;
        void                    ListHandler(StatementPtr);
    private:
        StatementPtr            pStatement;
        List                    statementList;
    };


    class SimpleNode : public AstNode
    {
    public:
        SimpleNode(ExprPtr pExprRhs): pExpr(pExprRhs) {}
        virtual             ~SimpleNode() {}
        // (Debug)
        virtual string      Dump() const override;

    private:
        ExprPtr             pExpr = nullptr;
    };



    class StatementNode : public AstNode
    {
    public:
        enum Type { IF, IFELSE,WHILE, SIMPLE };

        StatementNode(StatementNode::Type typeRhs, ExprPtr pExprRhs, BlockPtr pBlockRhs, BlockPtr pBlockOfElseRhs, SimplePtr pSimpleRhs):
            type(typeRhs), pExpr(pExprRhs), pBlock(pBlockRhs), pBlockOfElse(pBlockOfElseRhs), pSimple(pSimpleRhs) {} 
        virtual                 ~StatementNode() {}
        // (Debug)
        virtual string          Dump() const override;

    private:

        Type                    type;
        ExprPtr                 pExpr = nullptr;
        BlockPtr                pBlock = nullptr;
        BlockPtr                pBlockOfElse = nullptr;        
        SimplePtr               pSimple = nullptr;
    };


    class ProgramNode : public AstNode
    {
    public:
        ProgramNode(StatementPtr pStatementRhs): pStatement(pStatementRhs) {}
        virtual             ~ProgramNode() {}
        // (Debug)
        virtual string      Dump() const override;

    private:
        StatementPtr        pStatement = nullptr;
    };
}