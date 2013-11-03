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

#include <stdexcept>

#include <sstream>

namespace tds {

//
// volume_constant_controller_t
//

volume_constant_controller_t::volume_constant_controller_t( 
	unsigned int max_size_in_volume,
	unsigned int max_tasks_in_volume )
: 
	m_max_size_in_volume( max_size_in_volume ), 
	m_max_tasks_in_volume( max_tasks_in_volume ), 
	m_total_size_loaded( 0 ), 
	m_total_tasks_loaded( 0 ) 
{}

void
volume_constant_controller_t::loaded( unsigned int size )
{
	m_total_size_loaded += size;
	++m_total_tasks_loaded;
}

void
volume_constant_controller_t::unloaded( unsigned int size )
{
	if ( size > m_total_size_loaded )
		m_total_size_loaded = 0;
	else
		m_total_size_loaded -= size;

	if ( m_total_tasks_loaded > 0 )
		--m_total_tasks_loaded;
}

unsigned int
volume_constant_controller_t::how_much_size_allowed() const
{
	if ( m_max_size_in_volume < m_total_size_loaded )
	{
		return 0;
	}
	else
		return (m_max_size_in_volume - m_total_size_loaded);
}

unsigned int
volume_constant_controller_t::how_much_tasks_allowed() const
{
	if ( m_max_tasks_in_volume < m_total_tasks_loaded )
	{
		return 0;
	}
	else
		return (m_max_tasks_in_volume - m_total_tasks_loaded);
}

bool
volume_constant_controller_t::active() const
{
	return true;
}

//
// volume_dummy_controller_t
//

volume_dummy_controller_t::volume_dummy_controller_t()
{
}

void
volume_dummy_controller_t::loaded( unsigned int size )
{
}

void
volume_dummy_controller_t::unloaded( unsigned int size )
{
}

unsigned int
volume_dummy_controller_t::how_much_size_allowed() const
{
	return INT_MAX;
}

unsigned int
volume_dummy_controller_t::how_much_tasks_allowed() const
{
	return INT_MAX;
}

bool
volume_dummy_controller_t::active() const
{
	return false;
}

volume_controller_interface_t *
volume_controller_factory( 
	const volume_controller_type_t & volume_controller_type,
	unsigned int max_size_in_volume,
	unsigned int max_packets_in_volume )
{
	switch( volume_controller_type )
	{
		case dummy:
			return new volume_dummy_controller_t();
		case constant:
			return new volume_constant_controller_t( 
				max_size_in_volume,
				max_packets_in_volume );
		default:
		{
			std::stringstream s;
			s << "Incorrect volume_controller_type: " <<
				volume_controller_type << ";";

			throw s.str();
		}
	}
}

} /* namespace tds */
