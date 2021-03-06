﻿/*! \file myrandavx512.h
	\brief AVX-512命令を使った自作乱数クラスの宣言

	Copyright © 2017 @dc1394 All Rights Reserved.
	This software is released under the BSD 2-Clause License.
*/

#ifndef _MYRANDAVX512_H_
#define _MYRANDAVX512_H_

#pragma once

#include <algorithm>    // for std::generate
#include <array>		// for std::array
#include <cstdint>      // for std::int32_t, std::uint_least32_t
#include <functional>   // for std::ref
#include <memory>       // for std::unique_ptr
#include <limits>		// for std::numeric_limits
#include <random>		// for std::random_device
#include <vector>       // for std::vector
#include <immintrin.h>	// for _mm512_store_si512
#include <svrng.h>		// for svrng_new_uniform_distribution_int, svrng_new_mt19937_engine, svrng_generate16_int

namespace myrandom {
   	//! A lambda expression.
	/*!
		svrng_distribution_tへのポインタを開放するラムダ式
        \param pdistribution svrng_distribution_tへのポインタ
	*/
    static auto const distribution_deleter = [](auto pdistribution) {
        svrng_delete_distribution(*pdistribution);
    };

   	//! A lambda expression.
	/*!
		svrng_engine_tへのポインタを開放するラムダ式
        \param pengine svrng_engine_tへのポインタ
	*/
    static auto const svrng_engine_deleter = [](auto pengine) {
		svrng_delete_engine(*pengine);
    };
    
    //! A class.
	/*!
		自作乱数クラス
	*/
	class MyRandAvx512 final {
		// #region コンストラクタ・デストラクタ

	public:
		//! A constructor.
		/*!
			唯一のコンストラクタ
		*/
		MyRandAvx512();

		//! A destructor.
		/*!
			デフォルトデストラクタ
		*/
		~MyRandAvx512() = default;

		// #endregion コンストラクタ・デストラクタ

		// #region メンバ関数

		//!  A public member function.
		/*!
			[0, UINT32_MAX)の区間で一様乱数を生成する
		*/
		std::uint32_t myrand()
        {
			if (!cnt_) {
                _mm512_store_si512(rnd_.data(), svrng_generate16_int(*prandengine_, *pdistribution_));			
				return static_cast<std::uint32_t>(rnd_[cnt_++]);
			}
			else if (cnt_ == AVXREGBYTE) {
				cnt_ = 0;
				_mm512_store_si512(rnd_.data(), svrng_generate16_int(*prandengine_, *pdistribution_));
				return static_cast<std::uint32_t>(rnd_[cnt_++]);
			}
			else {
				return static_cast<std::uint32_t>(rnd_[cnt_++]);
			}
		}

		// #endregion メンバ関数

		// #region メンバ変数

	private:
		//! A private member variable.
		/*!
			AVX-512レジスタのバイト数
		*/
		static auto constexpr AVXREGBYTE = 16;

		//! A private member variable.
		/*!
			乱数のカウント
		*/
		std::int32_t cnt_ = 0;

		//! A private member variable.
		/*!
			乱数の分布へのスマートポインタ
		*/
        std::unique_ptr<svrng_distribution_t, decltype(distribution_deleter)> pdistribution_;
		
		//! A private member variable.
		/*!
			乱数エンジンへのスマートポインタ
		*/
        std::unique_ptr<svrng_engine_t, decltype(svrng_engine_deleter)> prandengine_;

		//! A private member variable.
		/*!
			乱数が格納されたstd::array
		*/
		alignas(AVXREGBYTE) std::array<std::int32_t, AVXREGBYTE> rnd_;
		
		// #region 禁止されたコンストラクタ・メンバ関数

		//! A private copy constructor (deleted).
		/*!
			コピーコンストラクタ（禁止）
		*/
		MyRandAvx512(MyRandAvx512 const &) = delete;

		//! A private member function (deleted).
		/*!
			operator=()の宣言（禁止）
			\param コピー元のオブジェクト（未使用）
			\return コピー元のオブジェクト
		*/
		MyRandAvx512 & operator=(MyRandAvx512 const &) = delete;

		// #endregion 禁止されたコンストラクタ・メンバ関数
	};

	MyRandAvx512::MyRandAvx512()
        : pdistribution_(new svrng_distribution_t(svrng_new_uniform_distribution_int(std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max())), distribution_deleter),
		  prandengine_(nullptr, svrng_engine_deleter)
	{
        // ランダムデバイス
		std::random_device rnd;

		// 初期化用ベクタ
		std::vector<std::uint_least32_t> v(1);

		// ベクタの初期化
		// 非決定的な乱数でシード列を構築する
		std::generate(v.begin(), v.end(), std::ref(rnd));
        
        // 乱数エンジン
		prandengine_.reset(new svrng_engine_t(svrng_new_mt19937_engine(v[0])));
	}
}

#endif  // _MYRANDAVX512_H_
