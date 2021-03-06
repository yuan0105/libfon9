﻿// \file fon9/seed/SeedSubr.cpp
// \author fonwinz@gmail.com
#include "fon9/seed/Tree.hpp"
#include "fon9/CountDownLatch.hpp"

namespace fon9 { namespace seed {

SeedNotifyArgs::~SeedNotifyArgs() {
}
void SeedNotifyArgs::MakeGridView() const {
   if (this->NotifyKind_ == SeedNotifyKind::TableChanged) {
      CountDownLatch waiter{1};
      this->Tree_.OnTreeOp([this, &waiter](const TreeOpResult&, TreeOp* op) {
         if (!op)
            waiter.ForceWakeUp();
         else
            op->GridView(GridViewRequestFull{*this->Tab_},
                         [this, &waiter](GridViewResult& res) {
               this->CacheGV_ = std::move(res.GridView_);
               waiter.ForceWakeUp();
            });
      });
      waiter.Wait();
      return;
   }
   if (this->Rd_ == nullptr || this->Tab_ == nullptr)
      return;
   RevBufferList rbuf{128};
   FieldsCellRevPrint0NoSpl(this->Tab_->Fields_, *this->Rd_, rbuf, *fon9_kCSTR_CELLSPL);
   this->CacheGV_ = BufferTo<std::string>(rbuf.MoveOut());
}
//--------------------------------------------------------------------------//
SeedNotifySubscribeOK::SeedNotifySubscribeOK(TreeOp& opTree, Tab& tab)
   : base(opTree.Tree_, &tab, TextBegin(), nullptr, SeedNotifyKind::SubscribeOK)
   , OpTree_(&opTree) {
}
void SeedNotifySubscribeOK::MakeGridView() const {
   if (this->Rd_)
      base::MakeGridView();
   else if (this->OpTree_) {
      this->OpTree_->GridView(GridViewRequestFull{*this->Tab_},
                              [this](GridViewResult& res) {
         this->CacheGV_ = std::move(res.GridView_);
      });
   }
}
BufferList SeedNotifySubscribeOK::GetGridViewBuffer(const std::string** gv) const {
   if (gv)
      *gv = &this->GetGridView();
   return BufferList{};
}
//--------------------------------------------------------------------------//
BufferList SeedNotifyStreamRecoverArgs::GetGridViewBuffer(const std::string** gv) const {
   if (gv)
      *gv = &this->GetGridView();
   return BufferList{};
}
//--------------------------------------------------------------------------//
fon9_API void SeedSubj_Notify(UnsafeSeedSubj& subj, const SeedNotifyArgs& args) {
   subj.Publish(args);
}
fon9_API void SeedSubj_NotifyPodRemoved(UnsafeSeedSubj& subj, Tree& tree, StrView keyText) {
   if (!subj.IsEmpty())
      SeedSubj_Notify(subj, SeedNotifyArgs(tree, nullptr, keyText, nullptr, SeedNotifyKind::PodRemoved));
}
fon9_API void SeedSubj_NotifySeedRemoved(UnsafeSeedSubj& subj, Tree& tree, Tab& tab, StrView keyText) {
   if (!subj.IsEmpty())
      SeedSubj_Notify(subj, SeedNotifyArgs(tree, &tab, keyText, nullptr, SeedNotifyKind::SeedRemoved));
}
fon9_API void SeedSubj_ParentSeedClear(UnsafeSeedSubj& subj, Tree& tree) {
   if (!subj.IsEmpty()) {
      SeedSubj_Notify(subj, SeedNotifyArgs(tree, nullptr, TextBegin(), nullptr, SeedNotifyKind::ParentSeedClear));
      subj.Clear();
   }
}
fon9_API void SeedSubj_TableChanged(UnsafeSeedSubj& subj, Tree& tree, Tab& tab) {
   if (!subj.IsEmpty())
      SeedSubj_Notify(subj, SeedNotifyArgs(tree, &tab, TextEnd(), nullptr, SeedNotifyKind::TableChanged));
}

} } // namespaces
