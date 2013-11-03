/*
	Copyright (c) 2013, Boris Sivko
	All rights reserved.

	E-mail: bsivko@gmail.com (Boris Sivko)

	This file is part of Telic Data Structures Library.

	Redistribution and use in source and binary forms, with or without 
	modification, are permitted provided that the following conditions are met:

	Redistributions of source code must retain the above copyright notice, 
	this list of conditions and the following disclaimer.

	Redistributions in binary form must reproduce the above copyright notice, 
	this list of conditions and the following disclaimer in the documentation 
	and/or other materials provided with the distribution.

	Neither the name of the Intervale nor the names of its contributors 
	may be used to endorse or promote products derived from this software 
	without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
	THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
	USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <tds_1/h/performance_estimator.hpp>

#include <limits.h>
#include "gtest/1.6.0/include/gtest/gtest.h"

#include <stdexcept>

#include <cstdlib>
#include <ace/OS_NS_time.h>
#include <ace/Time_Value.h>

using tds_1::performance_estimator_t;

namespace tds_1 {
	
TEST( PerformanceEstimator, Empty ) 
{
	{
		performance_estimator_t performance_estimator( 200, 10, 10 );

		EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_size(), 10 );
		EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_tasks(), 10 );
	}
	{
		performance_estimator_t performance_estimator( 200, 0, 0 );

		EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_size(), 0 );
		EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_tasks(), 0 );
	}
}

TEST( PerformanceEstimatorTest, Add ) 
{
	performance_estimator_t performance_estimator( 200, 10, 10 );
	performance_estimator.add( 200, 5 );

	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_size(), 25 );
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_tasks(), 5 );

	performance_estimator.add( 200, 5 );

	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_size(), 25 );
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_tasks(), 5 );

	performance_estimator.add( 400, 2 );
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_size(), (5+5+2)*1000.0/(200+200+400) );
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_tasks(), (1+1+1)*1000.0/(200+200+400) );
}

TEST( PerformanceEstimator, TimeCleanup ) 
{
	performance_estimator_t performance_estimator( 200, 10, 10 );
	performance_estimator.add( 200, 5 );
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_size(), 25 );
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_tasks(), 5 );

	ACE_OS::sleep( ACE_Time_Value(0, 100*1000 ) );

	performance_estimator.cleanup();
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_size(), 25 );
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_tasks(), 5 );

	performance_estimator.add( 100, 3 );

	ACE_OS::sleep( ACE_Time_Value( 0, 150*1000 ) );

	performance_estimator.cleanup();
	const float estimate_size = performance_estimator.get_estimate_performance_in_size();
	const float estimate_tasks = performance_estimator.get_estimate_performance_in_tasks();
	EXPECT_NE( static_cast<unsigned int>(estimate_size), 25 );
	EXPECT_NE( static_cast<unsigned int>(estimate_tasks), 5 );

	ACE_OS::sleep( ACE_Time_Value( 0, 150*1000 ) );

	performance_estimator.cleanup();
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_size(), estimate_size );
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_tasks(), estimate_tasks );

	performance_estimator.add( 200, 5 );
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_size(), 25 );
	EXPECT_FLOAT_EQ( performance_estimator.get_estimate_performance_in_tasks(), 5 );
}

TEST( PerformanceEstimator, TimeLowerBound )
{
	tds_1::performance_estimator_t performance_estimator( 290, 10, 10 );
	for( unsigned int i = 0; i < 10; ++i )
	{
		performance_estimator.add( 200, 5 );
		ACE_OS::sleep( ACE_Time_Value( 0, 50*1000 ) );
	}	
	
	performance_estimator.cleanup();
	EXPECT_EQ( performance_estimator.m_solved_tasks.size(), 5 );
}

} /* namespace tds_1 */

int main( int argc, char ** argv ) 
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
