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

#include <tds/h/event_counter.hpp>

#include <stdexcept>

#include "ace/Guard_T.h"

namespace tds {

event_counter_t::event_counter_t( 
	unsigned int number ) : 
	m_store( number, false ), m_pointer( 0 ), m_count( 0 ), m_total( 0 )
{
	if ( number == 0 )
		throw std::runtime_error( 
			"Null number is detected at event_counter c'tor. Must be more than 0." );
}

void
event_counter_t::event( bool what )
{
	ACE_Guard<ACE_Mutex> guard( m_store_locker );

	// Change had place or not?
	if ( m_store[m_pointer] != what )
	{
		if ( what )
		{
			// Change from false to true.
			++m_count;
		}
		else
		{
			// Change from true to false.
			--m_count;
		}

		m_store[m_pointer] = what;
	}

	next_pointer();
}

unsigned int 
event_counter_t::count() const
{
	return m_count;
}

unsigned int 
event_counter_t::total() const
{
	return m_total;
}

float
event_counter_t::percentage() const
{
	if ( m_total == 0 )
		return 0;

	return m_count * 100.0 / m_total;
}

void
event_counter_t::next_pointer() 
{
	if ( m_pointer == 0 )
		m_pointer = m_store.size() - 1;
	else
		--m_pointer;

	if ( m_total < m_store.size() ) 
		++m_total;
}

} /* namespace tds */
