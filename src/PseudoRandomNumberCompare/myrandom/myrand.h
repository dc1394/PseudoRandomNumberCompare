﻿/*! \file myrand.h
    \brief 自作乱数クラスの宣言

    Copyright © 2015 @dc1394 All Rights Reserved.
    This software is released under the BSD 2-Clause License.
*/

#ifndef _MYRAND_H_
#define _MYRAND_H_

#pragma once

#include <algorithm>    // for std::generate
#include <cstdint>      // for std::int32_t, std::uint_least32_t
#include <functional>   // for std::ref
#include <limits>		// for std::numeric_limits
#include <random>       // for std::mt19937, std::random_device
#include <vector>       // for std::vector

namespace myrandom {
    //! A class.
    /*!
        自作乱数クラス
    */
    class MyRand final {
        // #region コンストラクタ・デストラクタ

    public:
        //! A constructor.
        /*!
            唯一のコンストラクタ
        */
        MyRand();

        //! A destructor.
        /*!
            デフォルトデストラクタ
        */
        ~MyRand() = default;

        // #endregion コンストラクタ・デストラクタ

        // #region メンバ関数

        //!  A public member function.
        /*!
            [min, max]の閉区間で一様乱数を生成する
        */
        std::uint32_t myrand()
        {
            return distribution_(randengine_);
        }

        // #endregion メンバ関数

        // #region メンバ変数

    private:
        //! A private static member variable (constant expression).
        /*!
            初期乱数生成用のstd::vectorのサイズ
        */
        static std::vector<std::uint_least32_t>::size_type const SIZE = 8;

        //! A private member variable.
        /*!
            乱数の分布
        */
        std::uniform_int_distribution<std::uint32_t> distribution_;
        
        //! A private member variable.
        /*!
            乱数エンジン
        */
        std::mt19937 randengine_;

        // #region 禁止されたコンストラクタ・メンバ関数

        //! A private copy constructor (deleted).
        /*!
            コピーコンストラクタ（禁止）
        */
        MyRand(const MyRand &) = delete;

        //! A private member function (deleted).
        /*!
            operator=()の宣言（禁止）
            \param コピー元のオブジェクト（未使用）
            \return コピー元のオブジェクト
        */
        MyRand & operator=(const MyRand &) = delete;

        // #endregion 禁止されたコンストラクタ・メンバ関数
    };

    MyRand::MyRand() :
        distribution_(0, std::numeric_limits<std::uint32_t>::max())
    {
        // ランダムデバイス
        std::random_device rnd;

        // 初期化用ベクタ
        std::vector<std::uint_least32_t> v(SIZE);

        // ベクタの初期化
        // 非決定的な乱数でシード列を構築する
        std::generate(v.begin(), v.end(), std::ref(rnd));

        // シード列を生成
        std::seed_seq seq(v.begin(), v.end());

        // 乱数エンジン
        randengine_ = std::mt19937(seq);
    }
}

#endif  // _MYRAND_H_
