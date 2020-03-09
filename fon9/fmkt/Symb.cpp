﻿// \file fon9/fmkt/Symb.cpp
// \author fonwinz@gmail.com
#include "fon9/fmkt/Symb.hpp"
#include "fon9/seed/FieldMaker.hpp"

namespace fon9 { namespace fmkt {

SymbData::~SymbData() {
}

//--------------------------------------------------------------------------//

SymbData* Symb::GetSymbData(int tabid) {
   return tabid <= 0 ? this : nullptr;
}
SymbData* Symb::FetchSymbData(int tabid) {
   return tabid <= 0 ? this : nullptr;
}
seed::Fields Symb::MakeFields() {
   seed::Fields flds;
   flds.Add(fon9_MakeField(Symb, TDayYYYYMMDD_,     "TDay"));
   flds.Add(fon9_MakeField(Symb, TradingMarket_,    "Market"));
   flds.Add(fon9_MakeField(Symb, TradingSessionId_, "Session"));
   flds.Add(seed::FieldSP{new seed::FieldIntHx<underlying_type_t<f9fmkt_TradingSessionSt>>(
      Named("SessionSt"), fon9_OffsetOfRawPointer(Symb, TradingSessionSt_))});

   flds.Add(fon9_MakeField2(Symb, ShUnit));
   flds.Add(fon9_MakeField2(Symb, FlowGroup));
   flds.Add(fon9_MakeField2(Symb, PriceOrigDiv));
   flds.Add(fon9_MakeField2(Symb, StrikePriceDiv));
   flds.Add(fon9_MakeField2(Symb, ExgSymbSeq));
   flds.Add(fon9_MakeField (Symb, EndYYYYMMDD_, "EndDate"));
   return flds;
}

} } // namespaces
