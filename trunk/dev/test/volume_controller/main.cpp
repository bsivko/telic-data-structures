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

#include <tds/h/volume_controller.hpp>

#include <limits.h>
#include "gtest/1.6.0/include/gtest/gtest.h"

#include <stdexcept>

#include <cstdlib>

namespace tds {
	
TEST( VolumeController, Empty ) 
{
	volume_constant_controller_t volume_controller( 200 );

	EXPECT_EQ( volume_controller.how_much_is_allowed(), 200 );
}

TEST( VolumeController, Factory ) 
{
	std::unique_ptr<volume_controller_interface_t> dummy_controller = 
		volume_controller_factory( dummy, 100 );

	EXPECT_EQ( dummy_controller->how_much_is_allowed(), INT_MAX );

	std::unique_ptr<volume_controller_interface_t> constant_controller = 
		volume_controller_factory( constant, 100 );

	EXPECT_EQ( constant_controller->how_much_is_allowed(), 100 );
}

TEST( VolumeController, LoadUnload ) 
{
	volume_constant_controller_t volume_controller( 200 );

	volume_controller.loaded( 100 );
	EXPECT_EQ( volume_controller.how_much_is_allowed(), 100 );

	volume_controller.loaded( 100 );
	EXPECT_EQ( volume_controller.how_much_is_allowed(), 0 );

	volume_controller.unloaded( 50 );
	EXPECT_EQ( volume_controller.how_much_is_allowed(), 50 );

	volume_controller.unloaded( 100 );
	EXPECT_EQ( volume_controller.how_much_is_allowed(), 150 );

	volume_controller.unloaded( 100 );
	EXPECT_EQ( volume_controller.how_much_is_allowed(), 200 );
}

} /* namespace tds */

int main( int argc, char ** argv ) 
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
