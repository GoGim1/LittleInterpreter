#pragma once
#include <sstream>
#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <variant>
#include <tuple>
#include <initializer_list>
#include "Error.h"
#include "Token.h"
#include "Helper.h"
#include "Environment.h"
#include "tools/FloatCompare.h"

namespace Ast
{
    using namespace Lexer;
    using namespace Environment;
    using namespace Util;

    using std::stringstream;
    using std::make_pair;
    using std::shared_ptr;
    using std::string;
    using std::vector;
    using std::pair;
    using std::variant;
    using std::tuple;
    using std::make_tuple;
    using std::get;
    using std::initializer_list;

    class AstNode //: public std::enable_shared_from_this<AstNode>
    {
    public:
        AstNode() {}
        virtual                         ~AstNode() {}
        virtual const string            Dump() const = 0;
        virtual variant<int, double>    Eval() = 0;
    };

    class PrimaryNode;
    class FactorNode;
    class ExprNode;
    class BlockNode;
    class SimpleNode;
    class StatementNode;
    class ProgramNode;

    class ParamNode;
    class ParamsNode;
    class ParamListNode;
    class DefNode;
    class ArgsNode;
    class PostfixNode;


    using TokenPtr      = shared_ptr<Token>;
    using PrimaryPtr    = shared_ptr<PrimaryNode>;
    using FactorPtr     = shared_ptr<FactorNode>;
    using ExprPtr       = shared_ptr<ExprNode>;
    using BlockPtr      = shared_ptr<BlockNode>;
    using SimplePtr     = shared_ptr<SimpleNode>;
    using StatementPtr  = shared_ptr<StatementNode>;
    using ProgramPtr    = shared_ptr<ProgramNode>;

    using ParamPtr      = shared_ptr<ParamNode>;
    using ParamsPtr     = shared_ptr<ParamsNode>;
    using ParamListPtr  = shared_ptr<ParamListNode>;
    using DefPtr        = shared_ptr<DefNode>;
    using ArgsPtr       = shared_ptr<ArgsNode>;
    using PostfixPtr    = shared_ptr<PostfixNode>;

    class PrimaryNode : public AstNode
    {
    public:
        PrimaryNode(const ExprPtr& pExprRhs, const TokenPtr& pTokenRhs, const PostfixPtr& pPostfixRhs = nullptr):
                     pExpr(pExprRhs), pToken(pTokenRhs), pPostfix(pPostfixRhs)
        {
            Assert((!pToken && pExpr)||(pToken && !pExpr), "PrimaryNode must have one and only one non-nullptr member.");            
        }
        virtual                         ~PrimaryNode() {}
        virtual const string            Dump() const override;
        virtual variant<int, double>    Eval() override;

        bool                            isIdentifier() const;
        const string&                   GetIdentifierName() const;
        const pair<int, int>            GetIdentifierPos() const;
    private:
        ExprPtr     pExpr               = nullptr;
        TokenPtr    pToken              = nullptr;
        PostfixPtr  pPostfix            = nullptr;
    };


    class FactorNode : public AstNode
    {
    public:
        FactorNode(const PrimaryPtr& pPrimaryRhs, const TokenPtr& pTokenRhs): pPrimary(pPrimaryRhs), pToken(pTokenRhs) 
        {
            Assert((!pToken && pPrimary)||(pToken && pPrimary), "FactorNode must have non-nullptr members or have one non-nullptr pPrimary one nullptr pToken.");
            Assert(!pToken || (pToken->getType() == Token::SUB || pToken->getType() == Token::PLUS), "FactorNode pToken's type should be SUB or PLUS.");      
        }
        virtual                             ~FactorNode() {}
        virtual const string                Dump() const override;
        virtual variant<int, double>        Eval() override;

        bool                                isIdentifier() const;
        const string&                       GetIdentifierName() const;
        const pair<int, int>                GetIdentifierPos() const;
    private:
        PrimaryPtr  pPrimary                = nullptr;
        TokenPtr    pToken                  = nullptr;
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
        virtual                             ~ExprNode() {}
        virtual const string                Dump() const override;
        virtual variant<int, double>        Eval() override;
        void                                ListHandler(const TokenPtr&, const FactorPtr&);
    private:
        FactorPtr   pFactor                 = nullptr;
        List        factorList;
    };


    class BlockNode : public AstNode
    {
    public:
        typedef vector<StatementPtr>        List;

        BlockNode(const StatementPtr& pStatementRhs): pStatement(pStatementRhs) {}
        virtual                             ~BlockNode() {}
        virtual const string                Dump() const override;
        virtual variant<int, double>        Eval() override;        
        void                                ListHandler(const StatementPtr&);
    private:            
        StatementPtr                        pStatement;
        List                                statementList;
    };


    class SimpleNode : public AstNode
    {
    public:
        SimpleNode(const ExprPtr& pExprRhs, const ArgsPtr& pArgsRhs = nullptr):
                    pExpr(pExprRhs), pArgs(pArgsRhs)
        {
            Assert(pExpr, "SimpleNode pExpr can not be nullptr.");
        }
        virtual                             ~SimpleNode() {}
        virtual const string                Dump() const override;
        virtual variant<int, double>        Eval() override;

    private:
        ExprPtr pExpr                       = nullptr;
        ArgsPtr pArgs                       = nullptr;
    };



    class StatementNode : public AstNode
    {
    public:
        enum Type { UNKNOWN, IF, IFELSE, WHILE, SIMPLE };

        StatementNode(const Type& typeRhs, const ExprPtr& pExprRhs, const BlockPtr& pBlockRhs, const BlockPtr& pBlockOfElseRhs, const SimplePtr& pSimpleRhs):
            type(typeRhs), pExpr(pExprRhs), pBlock(pBlockRhs), pBlockOfElse(pBlockOfElseRhs), pSimple(pSimpleRhs) 
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
        virtual                                 ~StatementNode() {}
        virtual const string                    Dump() const override;
        virtual variant<int, double>            Eval() override;
    private:
        Type        type                        = UNKNOWN;
        ExprPtr     pExpr                       = nullptr;
        BlockPtr    pBlock                      = nullptr;
        BlockPtr    pBlockOfElse                = nullptr;        
        SimplePtr   pSimple                     = nullptr;
    };


    class ProgramNode : public AstNode
    {
    public:
        typedef vector<StatementPtr>        List;
        typedef vector<DefPtr>              DefList;

        ProgramNode() {}
        virtual                             ~ProgramNode() {}
        virtual const string                Dump() const override;
        virtual variant<int, double>        Eval() override;        
        void                                ListHandler(const StatementPtr&);
        void                                DefListHandler(const DefPtr&);        
    private:
        List                                statementList{};
        DefList                             defList{};
    };



    /***************************************************************************************
     *  Function
     * ************************************************************************************/
    class ParamNode 
    {
    public:
        ParamNode(const TokenPtr& pTokenRhs);
        virtual ~ParamNode() {}
        const string                        Dump() const ;
    private:
        TokenPtr                            pToken = nullptr;
    };

    class ParamsNode 
    {
    public:
        using List = vector<ParamPtr>;
    
        ParamsNode(const ParamPtr&); 
        virtual ~ParamsNode() {}
        const string                        Dump() const ;
        void                                ListHandler(const ParamPtr&);
        const vector<string>                GetParams() const;
    private: 
        ParamPtr                            pParam = nullptr;
        List                                paramList{};
    };

    class ParamListNode 
    {
    public:
        ParamListNode(const ParamsPtr& = nullptr);
        virtual ~ParamListNode() {}
        const string                        Dump() const ;
        const vector<string>                GetParams() const;
    private:
        ParamsPtr                           pParams = nullptr;
    };

    class DefNode : public std::enable_shared_from_this<DefNode>
    {
    public:
        DefNode(const TokenPtr&, const ParamListPtr&, const BlockPtr&); 
        virtual ~DefNode() {}
        const string                Dump() const ;
        void                        Definite();
        variant<int, double>        Eval(const vector<variant<int, double>>&, int, int) ;  
        const string&               GetFunctionName() const; 

    private:    
        TokenPtr                            pToken      = nullptr;
        ParamListPtr                        pParamList  = nullptr;
        BlockPtr                            pBlock      = nullptr;     
    };

    class ArgsNode 
    {
    public:
        using List = vector<ExprPtr>;

        ArgsNode(const ExprPtr&);
        virtual ~ArgsNode() {}
        const string                                    Dump() const ;
        void                                            ListHandler(const ExprPtr&);
        const vector<variant<int, double>>              GetArgs() ;
    
    private:
        ExprPtr                                         pExpr = nullptr;
        List                                            exprList{};  
    };

    class PostfixNode 
    {
    public:
        PostfixNode(const ArgsPtr& = nullptr);
        virtual ~PostfixNode() {}
        const string                                    Dump() const ;
        const vector<variant<int, double>>              GetArgs() const;
    private: 
        ArgsPtr                                         pArgs = nullptr;   
    };

}
