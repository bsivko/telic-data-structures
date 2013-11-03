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

#include <tds_1/h/event_counter.hpp>

#include <limits.h>
#include "gtest/1.6.0/include/gtest/gtest.h"

#include <stdexcept>

TEST( Start, Simple ) 
{
	tds_1::event_counter_t event_counter( 10 );

	EXPECT_EQ( event_counter.total(), 0 );
	EXPECT_EQ( event_counter.count(), 0 );
	EXPECT_FLOAT_EQ( event_counter.percentage(), 0 );
}

TEST( Start, Null )
{	
	EXPECT_THROW( tds_1::event_counter_t event_counter( 0 ), std::exception );
}

TEST( Overload, AllTrue )
{
	const unsigned int number = 10;
	tds_1::event_counter_t event_counter( number );
	ASSERT_EQ( event_counter.total(), 0 );

	for( unsigned int i = 0; i < 2*number; ++i )
	{
		event_counter.event( true );
		EXPECT_FLOAT_EQ( event_counter.percentage(), 100 );
	}
	ASSERT_EQ( event_counter.total(), number );
}


TEST( Overload, AllFalse )
{
	const unsigned int number = 10;
	tds_1::event_counter_t event_counter( number );
	ASSERT_EQ( event_counter.total(), 0 );

	for( unsigned int i = 0; i < 2*number; ++i )
	{
		event_counter.event( false );
		EXPECT_FLOAT_EQ( event_counter.percentage(), 0 );
	}
	ASSERT_EQ( event_counter.total(), number );
}


TEST( Run, Fidelity )
{
	const unsigned int number = 10;
	tds_1::event_counter_t event_counter( number );

	for( unsigned int i = 0; i < number; ++i )
		event_counter.event( false );

	for( unsigned int i = 0; i < number; ++i )
	{
		event_counter.event( true );
		EXPECT_FLOAT_EQ( event_counter.percentage(), 100.0*(i+1)/10.0 );
	}
}

TEST( Run, Values )
{
	const unsigned int number = 10;
	tds_1::event_counter_t event_counter( number );

	for( unsigned int i = 0; i < number; ++i )
	{
		event_counter.event( true );
		EXPECT_EQ( event_counter.total(), i+1  );
		EXPECT_EQ( event_counter.count(), i+1  );
	}

	for( unsigned int i = 0; i < number; ++i )
	{
		event_counter.event( false );
		EXPECT_EQ( event_counter.total(), number  );
		EXPECT_EQ( event_counter.count(), number-i-1  );
	}
}

int 
main( int argc, char ** argv ) 
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
