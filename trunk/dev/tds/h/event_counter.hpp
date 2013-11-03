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

#if !defined( _TDS__EVENT_COUNTER_HPP__INCLUDED )
#define _TDS__EVENT_COUNTER_HPP__INCLUDED

#include <vector>

#include "ace/Mutex.h"

namespace tds {

//! Count facts of happaened events (errors, successful actions, ... ) 
//! of last N of them.
class event_counter_t
{
	public:
		event_counter_t( 
			//! Count of events under control (buffer size).
			unsigned int number );

		//! Event was happened (true/false).
		void
		event( bool what );

		//! Get to know count of true-events.
		unsigned int 
		count() const;

		//! Total counted events.
		unsigned int 
		total() const;

		//! Get percentage value.
		float
		percentage() const;

	private:

		//! Move pointer to the next event.
		void
		next_pointer();

		ACE_Mutex m_event_locker;

		//! Count of the happened events.
		unsigned int m_count;

		//! Total counted events.
		unsigned int m_total;

		//! Pointer to the moving through buffer. 
		//! Point out to the element which will be changed next time.
		int m_pointer;

		//! Saves successfulness of all previous events.
		std::vector <bool> m_store;
};

} /* namespace tds */

#endif
