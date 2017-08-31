/*! \file pseudorandomnumbercompare.cpp
    \brief 乱数生成エンジンの速度を比較する

    Copyright © 2017 @dc1394 All Rights Reserved.
    This software is released under the BSD 2-Clause License.
*/

#include "../checkpoint/checkpoint.h"
#include "goexit/goexit.h"
#include "myrandom/myrand.h"
#include "myrandom/myrandsfmt.h"
#if defined(__INTEL_COMPILER) && defined(__AVX512F__)
	#include "myrandom/myrandavx512.h"
#endif
#include <cstdint>					// for std::int32_t
#include <iomanip>					// for std::setprecision
#include <iostream>					// for std::cout			
#include <limits>					// for std::numeric_limits	

namespace {
    template <typename MyRandom>
	double randomnumberavg(MyRandom & mr);
}

int main()
{
    checkpoint::CheckPoint cp;

    cp.checkpoint("処理開始", __LINE__);

	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout << std::setprecision(1);

	myrandom::MyRand mr;
	std::cout << "average = " << randomnumberavg(mr) << std::endl;

	cp.checkpoint("C++11標準の乱数生成エンジン", __LINE__);

	myrandom::MyRandSfmt mrsfmt;
	std::cout << "average = " << randomnumberavg(mrsfmt) << std::endl;

	cp.checkpoint("SFMT乱数生成エンジン", __LINE__);

	//myrandom::MyRandAvx512 mravx512;
	//std::cout << "average = " << randomnumberavg(mravx512) << std::endl;

	cp.checkpoint("AVX-512乱数生成エンジン", __LINE__);

    cp.checkpoint_print();

	goexit::goexit();

    return 0;
}

namespace {
	template <typename MyRandom>
	double randomnumberavg(MyRandom & mr)
	{
		auto sum = 0.0;
		for (auto n = 0U; n < std::numeric_limits<std::uint32_t>::max(); n++) {
			sum += static_cast<double>(mr.myrand());
		}

		return sum / static_cast<double>(std::numeric_limits<std::uint32_t>::max());
	}
}
