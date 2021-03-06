﻿// \file f9tws/ExgTradingLineMgr.hpp
// \author fonwinz@gmail.com
#ifndef __f9tws_ExgTradingLineMgr_hpp__
#define __f9tws_ExgTradingLineMgr_hpp__
#include "f9tws/ExgTradingLineFix.hpp"
#include "fon9/framework/IoManagerTree.hpp"

namespace f9tws {

fon9_WARN_DISABLE_PADDING;
class f9tws_API ExgTradingLineMgr
   : public f9fmkt::TradingLineManager // 負責管理: 可用線路、下單時尋找適當線路.
   , public f9fix::IoFixManager        // 負責處理: Fix線路可用時通知 f9fmkt::TradingLineManager
   , public fon9::IoManagerTree        // 負責管理: 通訊設定、狀態log、通訊物件的生死.
{        
   fon9_NON_COPY_NON_MOVE(ExgTradingLineMgr);
   using baseIo = fon9::IoManagerTree;
   using baseFix = f9fix::IoFixManager;

public:
   const f9fmkt_TradingMarket Market_;

   ExgTradingLineMgr(const fon9::IoManagerArgs& ioargs, fon9::TimeInterval afterOpen, f9fmkt_TradingMarket mkt)
      : baseIo(ioargs, afterOpen)
      , Market_(mkt) {
   }
   void OnFixSessionApReady(f9fix::IoFixSession& fixses) override;
   void OnFixSessionDisconnected(f9fix::IoFixSession& fixses, f9fix::FixSenderSP&& fixSender) override;

   /// 轉呼叫 this->OnBeforeDestroy(); 讓衍生者可以有機會清除排隊中的下單要求.
   void OnParentSeedClear() override;
};
fon9_WARN_POP;

} // namespaces
#endif//__f9tws_ExgTradingLineMgr_hpp__
