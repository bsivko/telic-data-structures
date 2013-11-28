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

#if !defined( _TDS__VOLUME_CONTROLLER_HPP__INCLUDED )
#define _TDS__VOLUME_CONTROLLER_HPP__INCLUDED

#include <limits.h>
#include <memory>

namespace tds {

//! Types of possible controllers.
enum volume_controller_type_t
{
	dummy,
	constant
};

//! Interface of volume controller.
class volume_controller_interface_t
{
	public:
		virtual
		~volume_controller_interface_t() {}

		//! Some size has loaded into volume.
		virtual void
		loaded( unsigned int size ) = 0;

		//! Some size has unloaded into volume.
		virtual void
		unloaded( unsigned int size ) = 0;

		//! How much size is free in volume.
		virtual unsigned int
		how_much_is_allowed() const = 0;
};

//! Volume controller of the volume with constant size.
class volume_constant_controller_t : public volume_controller_interface_t
{
	public:

		volume_constant_controller_t( 
			//! Max in volume.
			unsigned int max_in_volume );

		virtual void
		loaded( unsigned int size );

		virtual void
		unloaded( unsigned int size );

		virtual unsigned int
		how_much_is_allowed() const;

	private:
		//! How much totally is loaded.
		/*!
			 loaded() minus unloaded() in all lifetime.
		*/
		unsigned int m_totally_loaded;
		//! How much may be in volume.
		unsigned int m_max_in_volume;
};

//! Volume controller with no limits (dummy).
class volume_dummy_controller_t : public volume_controller_interface_t
{
	public:

		volume_dummy_controller_t();

		virtual void
		loaded( unsigned int size );

		virtual void
		unloaded( unsigned int size );

		virtual unsigned int
		how_much_is_allowed() const;
};

//! Factory of the volume controllers.
std::unique_ptr<volume_controller_interface_t>
volume_controller_factory( 
	const volume_controller_type_t & volume_controller_type,
	unsigned int max_in_volume );

} /* namespace tds */

#endif
