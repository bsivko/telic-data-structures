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

#if !defined( _TDS__SUM_COUNTER_HPP__INCLUDED )
#define _TDS__SUM_COUNTER_HPP__INCLUDED

#include <vector>

namespace tds {

//! Counts sums of special number events (traffic, middle value, ... ) 
/*!
	Stores sums about last N events, no more.

	Not thread-safe.
*/
class sum_counter_t
{
	public:
		sum_counter_t( 
			//! Count of maximum events under control (buffer size).
			unsigned int number );

		//! Event was happened (true/false).
		/*!
			value - number which describes the event.
		*/
		void
		event( unsigned int value );

		//! Get sum of happened events.
		unsigned int 
		sum() const;

		//! Total counted events.
		unsigned int 
		total() const;

		//! Get arithmetic mean.
		/*!
			(value1 + value2 + .. + valueN) / N
		*/
		float
		mean() const;

	private:

		//! Moves pointer to the next event.
		void
		next_pointer();

		//! Sum of all happened events.
		unsigned int m_sum;

		//! Total counted events.
		unsigned int m_total;

		//! Pointer to the moving through buffer. 
		//! Point out to the element which will be changed next time.
		int m_pointer;

		//! Saves values of all (N) previous events.
		std::vector <unsigned int> m_store;
};

} /* namespace tds */

#endif
