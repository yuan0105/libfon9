﻿/// \file fon9/crypto/Sha256.hpp
///
/// 您應該使用 OpenSSL or Windows Bcrypt.dll
/// 這裡提供 Sha256 僅是為了降低與外部的關聯性,
/// 並提供使用者密碼加密的範例.
///
/// \author fonwinz@gmail.com
#ifndef __fon9_crypto_Sha256_hpp__
#define __fon9_crypto_Sha256_hpp__
#include "fon9/crypto/CryptoTools.hpp"

namespace fon9 { namespace crypto {

fon9_WARN_DISABLE_PADDING;
struct fon9_API Sha256 {
   enum : size_t {
      kOutputSize = 32,
      kBlockSize = 64,
   };

   static void Hash(const void* data, size_t len, byte output[kOutputSize]);

   static void Hmac(const void *text, size_t textLen,
                    const void *key, size_t keyLen,
                    byte output[kOutputSize]);

   /// 使用 PBKDF2 演算法.
   static bool CalcSaltedPassword(const void* pass, size_t passLen,
                                  const void* salt, size_t saltLen,
                                  size_t iter,
                                  size_t outLen, void* out);
};

struct fon9_API ContextSha256 : public Sha256 {
   enum : size_t {
      kStateSize = kOutputSize / sizeof(uint32_t),
   };

   void Init();
   void Update(const void* data, size_t len);
   void Final(byte output[kOutputSize]);

private:
   uint64_t Count_;
   uint32_t State_[kStateSize];
   byte     Buffer_[kBlockSize];

   template <class AlgContext>
   friend void HashBlockUpdate(AlgContext& alg, const void* data, size_t len);

   template <class AlgContext>
   friend void HashBlockFinal(AlgContext& alg, byte output[AlgContext::kOutputSize]);

   static void Transform(uint32_t state[kStateSize], const byte buffer[kBlockSize]);
};
fon9_API_TEMPLATE_CLASS(Sha256HmacContext, HmacContext, ContextSha256);
fon9_WARN_POP;

} } // namespace
#endif//__fon9_crypto_Sha256_hpp__
