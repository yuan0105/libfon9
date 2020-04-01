﻿// \file f9tws/ExgMdSymbs.hpp
// \author fonwinz@gmail.com
#ifndef __f9tws_ExgMdSymbs_hpp__
#define __f9tws_ExgMdSymbs_hpp__
#include "f9tws/Config.h"
#include "fon9/fmkt/MdSymbs.hpp"
#include "fon9/fmkt/SymbRef.hpp"
#include "fon9/fmkt/SymbBS.hpp"
#include "fon9/fmkt/SymbDeal.hpp"
#include "fon9/fmkt/SymbHL.hpp"
#include "fon9/fmkt/SymbTws.hpp"
#include "fon9/fmkt/MdRtStream.hpp"

namespace f9tws {

class f9tws_API ExgMdSymb : public fon9::fmkt::SymbTws {
   fon9_NON_COPY_NON_MOVE(ExgMdSymb);
   using base = fon9::fmkt::SymbTws;
public:
   fon9::fmkt::SymbRef     Ref_;
   fon9::fmkt::SymbBS      BS_;
   fon9::fmkt::SymbDeal    Deal_;
   fon9::fmkt::SymbHigh    High_;
   fon9::fmkt::SymbLow     Low_;
   fon9::fmkt::MdRtStream  MdRtStream_;

   ExgMdSymb(const fon9::StrView& symbid, fon9::fmkt::MdRtStreamInnMgr& innMgr)
      : base{symbid}
      , MdRtStream_{innMgr} {
      this->TDayYYYYMMDD_ = innMgr.TDayYYYYMMDD();
      this->MdRtStream_.OpenRtStorage(*this);
   }

   fon9::fmkt::SymbData* GetSymbData(int tabid) override;
   fon9::fmkt::SymbData* FetchSymbData(int tabid) override;

   void DailyClear(fon9::fmkt::SymbTree& tree, unsigned tdayYYYYMMDD);
   void SessionClear(fon9::fmkt::SymbTree& tree, f9fmkt_TradingSessionId tsesId);
   /// 移除商品, 通常是因為商品下市.
   void BeforeRemove(fon9::fmkt::SymbTree& tree, unsigned tdayYYYYMMDD);

   static fon9::seed::LayoutSP MakeLayout();
};
//--------------------------------------------------------------------------//
class f9tws_API ExgMdSymbs : public fon9::fmkt::MdSymbs<ExgMdSymb> {
   fon9_NON_COPY_NON_MOVE(ExgMdSymbs);
   using base = fon9::fmkt::MdSymbs<ExgMdSymb>;

public:
   ExgMdSymbs(std::string rtiPathFmt)
      : base(ExgMdSymb::MakeLayout(), std::move(rtiPathFmt)) {
   }
   fon9::fmkt::SymbSP MakeSymb(const fon9::StrView& symbid) override;
};
using ExgMdSymbsSP = fon9::intrusive_ptr<ExgMdSymbs>;

} // namespaces
#endif//__f9tws_ExgMdSymbs_hpp__
