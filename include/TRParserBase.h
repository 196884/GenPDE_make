//
//  ParserBase.h
//  GenPDE
//
//  Created by Regis Dupont on 8/20/14.
//  Copyright (c) 2014 Regis Dupont. All rights reserved.
//

#ifndef TRPARSERBASE_H_
#define TRPARSERBASE_H_

#include <stack>
#include <string>

#include <boost/shared_ptr.hpp>

#include "Framework.h"

#include "GpDate.h"

#include "PEBinaryOperation.h"
#include "PEUnaryOperation.h"

#include "PayoutExpression.h"
#include "PEOperators.h"
#include "PEConditional.h"
#include "PEComparisonOperation.h"
#include "PEConstant.h"
#include "AVReference.h"
#include "MOReference.h"
#include "AuxiliaryVariable.h"

#include "TradeLeg.h"
#include "TLPayment.h"
#include "TLPricerRef.h"

#include "PricingInstruction.h"
#include "PIPricerInit.h"
#include "PIPricerAdd.h"
#include "PIPricerMult.h"
#include "PIPricerCond.h"
#include "PIPricerChoice.h"
#include "PIPricerRemove.h"
#include "PIPricerRmAVDeps.h"

#include "PricingInstructions.h"
#include "TradeRepresentation.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

namespace qi  = boost::spirit::qi;
namespace phx = boost::phoenix;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
class TRParserBase
{
protected:
    typedef boost::shared_ptr<PayoutExpression>               PEPtr;
    typedef boost::shared_ptr<TradeLeg>                       TLPtr;
    typedef boost::shared_ptr<const PricingInstruction>       PIPtr;
    typedef boost::shared_ptr<const AuxiliaryVariable>        AVPtr;
    typedef boost::shared_ptr<const AuxiliaryVariables>       AVsPtr;
    typedef boost::shared_ptr<const PricingInstructions>      PISPtr;
    typedef boost::shared_ptr<const DatedPricingInstructions> DPISPtr;
    typedef boost::shared_ptr<const TradeRepresentation>      TRPtr;
    
public:
    TRParserBase()
    {
        // Operators
        mBinOp   =   qi::string("Add")[qi::_val = GenPDE::Add]
                   | qi::string("Sub")[qi::_val = GenPDE::Sub]
                   | qi::string("Mul")[qi::_val = GenPDE::Mul]
                   | qi::string("Div")[qi::_val = GenPDE::Div]
                   | qi::string("Min")[qi::_val = GenPDE::Min]
                   | qi::string("Max")[qi::_val = GenPDE::Max];
        mUnOp    =   qi::string("Neg")[qi::_val = GenPDE::Neg]
                   | qi::string("Id")[ qi::_val = GenPDE::Id ];
        mCmpOp   =   qi::string("LT")[ qi::_val = GenPDE::LT ]
                   | qi::string("LE")[ qi::_val = GenPDE::LE ]
                   | qi::string("GT")[ qi::_val = GenPDE::GT ]
                   | qi::string("GE")[ qi::_val = GenPDE::GE ]
                   | qi::string("EQ")[ qi::_val = GenPDE::EQ ]
                   | qi::string("NE")[ qi::_val = GenPDE::NE ];
        mDateAsStr = qi::repeat(8)[qi::char_("0-9")];
        mDate    = mDateAsStr[qi::_val = phx::bind(&GenPDE::dateFromString, qi::_1)];
        mChooser =   qi::string("Us")[qi::_val = Choice::Chooser_Us]
                   | qi::string("Client")[qi::_val = Choice::Chooser_Client];
        mChoice  =   qi::string("None")[qi::_val = Choice::Choice_None]
                         | qi::string("Leg0")[qi::_val = Choice::Choice_Leg0]
                         | qi::string("Leg1")[qi::_val = Choice::Choice_Leg1];
        mChoiceId = +(~qi::char_('"'));
        
        // PayoutExpression
        mAVRef   = (
            qi::lexeme["<AVRef uid=\"" >> qi::ulong_ >> "\"/>"]
        )[qi::_val = phx::construct<PEPtr>(phx::new_<AVReference>(qi::_1))];
        
        mMORef   = (
            qi::lexeme["<MORef uid=\"" >> qi::uint_ >> "\"/>"]
        )[qi::_val = phx::construct<PEPtr>(phx::new_<MOReference>(qi::_1))];
        
        mPEConst = (
            qi::lexeme["<Const value=\"" >> qi::double_ >> "\"/>"]
        )[qi::_val = phx::construct<PEPtr>(phx::new_<PEConstant>(qi::_1))];
        
        mPEBinOp = (
            qi::lexeme["<BinOp op=\"" >> mBinOp >> "\">"] >>
            mPE >>
            mPE >>
            "</BinOp>"
        )[qi::_val = phx::construct<PEPtr>(phx::new_<PEBinaryOperation>(qi::_1, qi::_2, qi::_3))];
        
        mPEUnOp  = (
            qi::lexeme["<UnOp op=\"" >> mUnOp >> "\">"] >>
            mPE >>
            "</UnOp>"
        )[qi::_val = phx::construct<PEPtr>(phx::new_<PEUnaryOperation>(qi::_1, qi::_2))];
        
        mPECmpOp = (
            qi::lexeme["<CmpOp op=\"" >> mCmpOp >> "\">"] >>
            mPE >>
            mPE >>
            "</CmpOp>"
        )[qi::_val = phx::construct<PEPtr>(phx::new_<PEComparisonOperation>(qi::_1, qi::_2, qi::_3))];
        
        mPECond = (
            "<Conditional>" >>
            mPE >>
            mPE >>
            mPE >>
            "</Conditional>"
        )[qi::_val = phx::construct<PEPtr>(phx::new_<PEConditional>(qi::_1, qi::_2, qi::_3))];
        
        mPE      =   mPEConst
                   | mAVRef
                   | mMORef
                   | mPEBinOp
                   | mPEUnOp
                   | mPECmpOp
                   | mPECond;
        
        mTLPrc   = (
            qi::lexeme["<PricerRef uid=\"" >> qi::uint_ >> "\"/>"]
        )[qi::_val = phx::construct<TLPtr>(phx::new_<TLPricerRef>(qi::_1))];
        
        mTLPmt   = (
            qi::lexeme["<Payment date=\"" >> mDate >> "\">"] >>
            mPE >>
            "</Payment>"
        )[qi::_val = phx::construct<TLPtr>(phx::new_<TLPayment>(qi::_1, qi::_2))];
        
        mTL      =   mTLPrc
                   | mTLPmt;
        
        mPIInit  = (
            qi::lexeme["<PricerInit uid=\"" >> qi::uint_ >> "\">"] >>
            mTLPmt >>
            "</PricerInit>"
        )[qi::_val = phx::construct<PIPtr>(phx::new_<PIPricerInit>(qi::_1, qi::_2))];
        
        mPIAdd   = (
            qi::lexeme["<PricerAdd uid=\"" >> qi::uint_ >> "\">"] >>
            mTL >>
            mTL >>
            "</PricerAdd>"
        )[qi::_val = phx::construct<PIPtr>(phx::new_<PIPricerAdd>(qi::_1, qi::_2, qi::_3))];
        
        mPIMult  = (
            qi::lexeme["<PricerMult uid=\"" >> qi::uint_ >> "\">"] >>
            mPE >>
            mTL >>
            "</PricerMult>"
        )[qi::_val = phx::construct<PIPtr>(phx::new_<PIPricerMult>(qi::_1, qi::_2, qi::_3))];
        
        mPICond  = (
            qi::lexeme["<PricerCond uid=\"" >> qi::uint_ >> "\">"] >>
            mPE >>
            mTL >>
            mTL >>
            "</PricerCond>"
        )[qi::_val = phx::construct<PIPtr>(phx::new_<PIPricerCond>(qi::_1, qi::_2, qi::_3, qi::_4))];
        
        mPIChoice = (
            "<PricerChoice uid=\"" >> qi::uint_ >> "\" chooser=\"" >> mChooser >>
            "\" choiceID=\"" >> mChoiceId >> "\" default=\"" >> mChoice >> "\">" >>
            mTL >>
            mTL >>
            "</PricerChoice>"
        )[qi::_val = phx::construct<PIPtr>(phx::new_<PIPricerChoice>(qi::_1, qi::_2, qi::_3, qi::_4, qi::_5, qi::_6))];
        
        mPIRmAVs = (
            "<PricerRmAVDeps uid=\"" >> qi::uint_ >> "\" AVs=\"" >> (qi::ulong_ % ',') >> "\"/>"
        )[qi::_val = phx::construct<PIPtr>(phx::new_<PIPricerRmAVDeps>(qi::_1, qi::_2))];
        
        mPIRm    = (
            qi::lexeme["<PricerRm uid=\"" >> qi::uint_ >> "\"/>"]
        )[qi::_val = phx::construct<PIPtr>(phx::new_<PIPricerRemove>(qi::_1))];
        
        mPI      =   mPIInit
                   | mPIAdd
                   | mPIMult
                   | mPICond
                   | mPIChoice
                   | mPIRmAVs
                   | mPIRm;
        
        mAV      = (
            "<AV uid=\"" >> qi::uint_ >> "\" date=\"" >> mDate >> "\">" >>
            mPE >>
            "</AV>"
        )[qi::_val = phx::construct<AVPtr>(phx::new_<AuxiliaryVariable>(qi::_1, qi::_2, qi::_3))];
        
        mAVs     = (
            "<AuxiliaryVariables>" >>
            *mAV >>
            "</AuxiliaryVariables>"
        )[qi::_val = phx::construct<AVsPtr>(phx::new_<AuxiliaryVariables>(qi::_1))];
        
        mPIS     = (
            qi::lexeme["<PricingInstructions date=\"" >> mDate >> "\">"] >>
            *mPI >>
            "</PricingInstructions>"
        )[qi::_val = phx::construct<PISPtr>(phx::new_<PricingInstructions>(qi::_1, qi::_2))];
        
        mDPIS    = (
            "<DatedPricingInstructions>" >>
            *mPIS >>
            "</DatedPricingInstructions>"
        )[qi::_val = phx::construct<DPISPtr>(phx::new_<DatedPricingInstructions>(qi::_1))];
        
        mTR      = (
            "<TradeRepresentation>" >>
            mDPIS >>
            "<MainTradeLeg>" >>
            mTL >>
            "</MainTradeLeg>" >>
            mAVs >>
            "</TradeRepresentation>"
        )[qi::_val = phx::construct<TRPtr>(phx::new_<TradeRepresentation>(qi::_1, qi::_2, qi::_3))];
    }
    
    PEPtr parsePE(Iterator first, Iterator last)
    {
        PEPtr result;
        bool r = qi::phrase_parse(
            first,
            last,
            mPE,
            qi::space,
            result
        );
        if( !r )
        {
            std::cerr << "Stopped at:" << std::endl << std::string(first, last) << std::endl;
            Exception::raise("GenPDEParser::parsePE", "Could not parse PayoutExpression");
        }
        return result;
    }
    
    TLPtr parseTL(Iterator first, Iterator last)
    {
        TLPtr result;
        bool r = qi::phrase_parse(
            first,
            last,
            mTL,
            qi::space,
            result
        );
        if( !r )
            Exception::raise("GenPDEParser::parseTL", "Could not parse TradeLeg");
        return result;
    }
    
    PIPtr parsePI(Iterator first, Iterator last)
    {
        PIPtr result;
        bool r = qi::phrase_parse(
            first,
            last,
            mPI,
            qi::space,
            result
        );
        if( !r )
        {
            std::cerr << "Stopped at:" << std::endl << std::string(first, last) << std::endl;
            Exception::raise("GenPDEParser::parsePI", "Could not parse PricingInstruction");
        }
        return result;
    }
    
    AVsPtr parseAVs(Iterator first, Iterator last)
    {
        AVsPtr result;
        bool r = qi::phrase_parse(
            first,
            last,
            mAVs,
            qi::space,
            result
        );
        if( !r )
            Exception::raise("GenPDEParser::parseAVs", "Could not parse AVContext");
        return result;
    }
    
    TRPtr parseTR(Iterator first, Iterator last)
    {
        TRPtr result;
        bool r = qi::phrase_parse(
            first,
            last,
            mTR,
            qi::space,
            result
        );
        if( !r )
        {
            std::cerr << "Stopped at:" << std::endl << std::string(first, last) << std::endl;
            Exception::raise("GenPDEParser::parseTR", "Could not parse TradeRepresentation");
        }
        return result;
    }
    
protected:
protected:
    qi::rule<Iterator, GenPDE::BinaryOperator()>      mBinOp;
    qi::rule<Iterator, GenPDE::UnaryOperator()>       mUnOp;
    qi::rule<Iterator, GenPDE::ComparisonOperator()>  mCmpOp;
    qi::rule<Iterator, std::string()>             mDateAsStr;
    qi::rule<Iterator, GenPDE::Date()>            mDate;
    qi::rule<Iterator, Choice::Chooser()>         mChooser;
    qi::rule<Iterator, Choice::Choice()>          mChoice;
    qi::rule<Iterator, Choice::Uid()>             mChoiceId;
    
    qi::rule<Iterator, PEPtr()>                   mPEConst;
    qi::rule<Iterator, PEPtr()>                   mAVRef;
    qi::rule<Iterator, PEPtr()>                   mMORef;
    qi::rule<Iterator, PEPtr(),   qi::space_type> mPEBinOp;
    qi::rule<Iterator, PEPtr(),   qi::space_type> mPEUnOp;
    qi::rule<Iterator, PEPtr(),   qi::space_type> mPECmpOp;
    qi::rule<Iterator, PEPtr(),   qi::space_type> mPECond;
    qi::rule<Iterator, PEPtr(),   qi::space_type> mPE;
    
    qi::rule<Iterator, TLPtr(),   qi::space_type> mTLPmt;
    qi::rule<Iterator, TLPtr()>                   mTLPrc;
    qi::rule<Iterator, TLPtr(),   qi::space_type> mTL;
    
    qi::rule<Iterator, PIPtr(),   qi::space_type> mPIInit;
    qi::rule<Iterator, PIPtr(),   qi::space_type> mPIAdd;
    qi::rule<Iterator, PIPtr(),   qi::space_type> mPIMult;
    qi::rule<Iterator, PIPtr(),   qi::space_type> mPICond;
    qi::rule<Iterator, PIPtr(),   qi::space_type> mPIChoice;
    qi::rule<Iterator, PIPtr()>                   mPIRm;
    qi::rule<Iterator, PIPtr()>                   mPIRmAVs;
    qi::rule<Iterator, PIPtr(),   qi::space_type> mPI;
    
    qi::rule<Iterator, AVPtr(),   qi::space_type> mAV;
    qi::rule<Iterator, AVsPtr(),  qi::space_type> mAVs;
    
    qi::rule<Iterator, PISPtr(),  qi::space_type> mPIS;
    qi::rule<Iterator, DPISPtr(), qi::space_type> mDPIS;
    
    qi::rule<Iterator, TRPtr(),   qi::space_type> mTR;
};

#endif // TRPARSERBASE_H_
