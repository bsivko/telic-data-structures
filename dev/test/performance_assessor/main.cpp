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

#include <tds_1/h/performance_assessor.hpp>

#include <limits.h>
#include "gtest/1.6.0/include/gtest/gtest.h"

#include <stdexcept>

#include <cstdlib>
#include <ace/OS_NS_time.h>
#include <ace/Time_Value.h>

using tds_1::performance_assessor_t;

namespace tds_1 {
	
TEST( PerformanceAssessor, Empty ) 
{
	{
		performance_assessor_t performance_assessor( 200 );

		EXPECT_NEAR( performance_assessor.get_assess_performance_in_size(), 0, 1e-5 );
		EXPECT_NEAR( performance_assessor.get_assess_performance_in_tasks(), 0, 1e-5 );
	}
}

TEST( PerformanceAssessorTest, Add ) 
{
	const unsigned int period = 200;
	performance_assessor_t performance_assessor( period );

	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );

	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), (5+5)*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), (1+1)*1000/period );

	performance_assessor.add( 2 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), (5+5+2)*1000.0/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), (1+1+1)*1000.0/period );
}

TEST( PerformanceAssessor, TimeCleanup ) 
{
	const unsigned int period = 200;
	performance_assessor_t performance_assessor( period );
	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );

	ACE_OS::sleep( ACE_Time_Value(0, 100*1000 ) );

	performance_assessor.cleanup();
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );

	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), (5+5)*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), (1+1)*1000/period );

	ACE_OS::sleep( ACE_Time_Value( 0, 150*1000 ) );

	performance_assessor.cleanup();
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );

	ACE_OS::sleep( ACE_Time_Value( 0, 150*1000 ) );
	performance_assessor.cleanup();

	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 0 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 0 );
}

TEST( PerformanceAssessor, TimeLowerBound )
{
	performance_assessor_t performance_assessor( 290 );
	for( unsigned int i = 0; i < 10; ++i )
	{
		performance_assessor.add( 5 );
		ACE_OS::sleep( ACE_Time_Value( 0, 50*1000 ) );
	}	
	
	performance_assessor.cleanup();
	EXPECT_EQ( performance_assessor.m_executed_tasks.size(), 5 );
}

TEST( PerformanceAssessor, Power ) 
{
	const unsigned int period = 200;
	performance_assessor_t performance_assessor( period, 3 );
	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 0 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 0 );
	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 0 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 0 );
	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );

	ACE_OS::sleep( ACE_Time_Value(0, 100*1000 ) );

	performance_assessor.cleanup();
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );

	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );
	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );
	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), (5+5)*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), (1+1)*1000/period );

	ACE_OS::sleep( ACE_Time_Value( 0, 150*1000 ) );

	performance_assessor.cleanup();
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );

	ACE_OS::sleep( ACE_Time_Value( 0, 150*1000 ) );
	performance_assessor.cleanup();

	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 0 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 0 );
}


TEST( PerformanceAssessor, PowerAtomic ) 
{
	const unsigned int period = 200;
	performance_assessor_t performance_assessor( period, 3 );
	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 0 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 0 );

	ACE_OS::sleep( ACE_Time_Value( 0, 100*1000 ) );

	performance_assessor.cleanup();
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 0 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 0 );

	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 0 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 0 );
	performance_assessor.add( 5 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );

	ACE_OS::sleep( ACE_Time_Value( 0, 150*1000 ) );

	performance_assessor.cleanup();
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 5*1000/period );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 1*1000/period );

	ACE_OS::sleep( ACE_Time_Value( 0, 100*1000 ) );
	performance_assessor.cleanup();

	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_size(), 0 );
	EXPECT_FLOAT_EQ( performance_assessor.get_assess_performance_in_tasks(), 0 );
}

} /* namespace tds_1 */

int main( int argc, char ** argv ) 
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
