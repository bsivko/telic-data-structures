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

#include <tds/h/sum_counter.hpp>

#include <limits.h>
#include "gtest/1.6.0/include/gtest/gtest.h"

#include <stdexcept>

namespace tds {

TEST( Start, Simple ) 
{
	tds::sum_counter_t sum_counter( 10 );

	EXPECT_EQ( sum_counter.total(), 0 );
	EXPECT_EQ( sum_counter.sum(), 0 );
	EXPECT_FLOAT_EQ( sum_counter.medium(), 0 );
}

TEST( Start, Null )
{
	EXPECT_THROW( tds::sum_counter_t sum_counter( 0 ), std::exception );
}

TEST( Overload, AllTen )
{
	const unsigned int number = 10;
	tds::sum_counter_t sum_counter( number );
	ASSERT_EQ( sum_counter.total(), 0 );

	for( unsigned int i = 0; i < 2*number; ++i )
	{
		sum_counter.event( 10 );
		EXPECT_FLOAT_EQ( sum_counter.medium(), 10 );
	}
	ASSERT_EQ( sum_counter.total(), number );
}


TEST( Overload, AllZero )
{
	const unsigned int number = 10;
	tds::sum_counter_t sum_counter( number );
	ASSERT_EQ( sum_counter.total(), 0 );

	for( unsigned int i = 0; i < 2*number; ++i )
	{
		sum_counter.event( 0 );
		EXPECT_FLOAT_EQ( sum_counter.medium(), 0 );
	}
	ASSERT_EQ( sum_counter.total(), number );
}


TEST( Run, Fidelity )
{
	const unsigned int number = 10;
	tds::sum_counter_t sum_counter( number );

	for( unsigned int i = 0; i < number; ++i )
		sum_counter.event( 0 );

	for( unsigned int i = 0; i < number; ++i )
	{
		sum_counter.event( 1 );
		EXPECT_FLOAT_EQ( sum_counter.medium(), static_cast<float>(i+1)/number );
	}
}

TEST( Run, Values )
{
	const unsigned int number = 10;
	tds::sum_counter_t sum_counter( number );

	for( unsigned int i = 0; i < number; ++i )
	{
		sum_counter.event( 1 );
		EXPECT_EQ( sum_counter.total(), i+1  );
		EXPECT_EQ( sum_counter.sum(), i+1  );
	}

	for( unsigned int i = 0; i < number; ++i )
	{
		sum_counter.event( 0 );
		EXPECT_EQ( sum_counter.total(), number  );
		EXPECT_EQ( sum_counter.sum(), number-i-1  );
	}
}

} /* namespace tds */

int 
main( int argc, char ** argv ) 
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
