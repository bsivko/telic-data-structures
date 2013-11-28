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

//! Counts facts that events already happened (errors, successful actions, ... ). 
/*!
	Stores facts about last N events, no more.

	Thread-safe.
*/
class event_counter_t
{
	public:
		event_counter_t( 
			//! Count of maximum events under control (buffer size).
			unsigned int number );

		//! Event was happened (true/false).
		/*!
			true - positive (happened)
			false - negative.
		*/
		void
		event( bool what );

		//! Get count of true-events.
		unsigned int 
		count() const;

		//! Total count of happened events.
		unsigned int 
		total() const;

		//! Get percentage value of true-events.
		/*!
			If there were 0 events, result of this function will be 0.
		*/
		float
		percentage() const;

	private:

		//! Moves pointer to the next event.
		void
		next_pointer();

		//! Count of the happened events.
		unsigned int m_count;

		//! Total count of happened events.
		unsigned int m_total;

		//! Pointer to the moving through buffer. 
		//! Point out to the element which will be changed next time.
		int m_pointer;

		//! Saves successfulness of all (N) previous events.
		std::vector <bool> m_store;

		ACE_Mutex m_store_locker;
};

} /* namespace tds */

#endif
