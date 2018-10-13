#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <cassert>
#include "Token.h"
#include "Helper.h"


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
        PrimaryNode(const ExprPtr& pExprRhs, const TokenPtr& pTokenRhs) : pExpr(pExprRhs), pToken(pTokenRhs) 
        {
            Assert((!pToken && pExpr)||(pToken && !pExpr), "PrimaryNode must have one and only one non-nullptr member.");            
        }
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
        FactorNode(const PrimaryPtr& pPrimaryRhs, const TokenPtr& pTokenRhs): pPrimary(pPrimaryRhs), pToken(pTokenRhs) 
        {
            Assert((!pToken && pPrimary)||(pToken && pPrimary), "FactorNode must have non-nullptr members or have one non-nullptr pPrimary one nullptr pToken.");
            Assert(!pToken || (pToken->getType() == Token::SUB || pToken->getType() == Token::PLUS), "FactorNode pToken's type should be SUB or PLUS.");      
        }
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

        ExprNode(const FactorPtr& pFactorRhs): pFactor(pFactorRhs) 
        {
            Assert(pFactor, "ExprNode pFactor can not be nullptr.");
        }
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
        SimpleNode(const ExprPtr& pExprRhs): pExpr(pExprRhs) 
        {
            Assert(pExpr, "SimpleNode pExpr can not be nullptr.");
        }
        virtual             ~SimpleNode() {}
        // (Debug)
        virtual string      Dump() const override;

    private:
        ExprPtr             pExpr   = nullptr;
    };



    class StatementNode : public AstNode
    {
    public:
        enum Type { UNKNOWN, IF, IFELSE, WHILE, SIMPLE };

        StatementNode(const Type& typeRhs, const ExprPtr& pExprRhs, const BlockPtr& pBlockRhs, const BlockPtr& pBlockOfElseRhs, const SimplePtr& pSimpleRhs):
            type(typeRhs), pExpr(pExprRhs), pBlock(pBlockRhs), pBlockOfElse(pBlockOfElseRhs), pSimple(pSimpleRhs) {} 
        virtual                 ~StatementNode() 
        {
            #ifdef DEBUG
            switch (type)
            {
            case IF:
                Assert(pExpr && pBlock && !pBlockOfElse && !pSimple, "StatementNode in IF type have incorrect members.");
                break;
            case WHILE:
                Assert(pExpr && pBlock && !pBlockOfElse && !pSimple, "StatementNode in WHILE type have incorrect members.");
                break;
            case IFELSE:
                Assert(pExpr && pBlock && pBlockOfElse && !pSimple, "StatementNode in IFELSE type have incorrect members.");
                break;
            case SIMPLE:
                Assert(!pExpr && !pBlock && !pBlockOfElse && pSimple, "StatementNode in SIMPLE type have incorrect members.");
                break;
            case UNKNOWN:
                Assert(1, "StatementNode must have type.");
                break;
            }
            #endif
        }
        // (Debug)
        virtual string          Dump() const override;

    private:

        Type                    type            = UNKNOWN;
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
