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

#include <tds/h/performance_assessor.hpp>

#include <algorithm>

#include <vector>

#include <cpp_util_2/h/lexcast.hpp>

#include <stdexcept>

namespace tds {

bool
operator < (const executed_task_t & left, const executed_task_t & right)
{
	return (left.m_time_in < right.m_time_in);
}

//
// performance_assessor_t
//

performance_assessor_t::performance_assessor_t( 
	unsigned int period_analysis,
	unsigned int power ) :
	m_sum_size( 0 ),
	m_period_analysis( period_analysis ),
	m_power( power ),
	m_power_pool_counter( 0 ),
	m_power_outgoing_counter( 0 )
{
	assess();
}

void
performance_assessor_t::add( 
	unsigned int size )
{
	m_executed_tasks.push_back( executed_task_t( size ) );
	m_sum_size += size;

	++m_power_pool_counter;
	if ( m_power_pool_counter == m_power )
	{
		m_power_pool_counter = 0;
		m_power_outgoing_counter = 0;
		assess();
	}
}

void
performance_assessor_t::cleanup()
{
	if( m_executed_tasks.empty() )
		return;

	const ACE_Time_Value board_time ( 
		ACE_OS::gettimeofday() - ACE_Time_Value( 0, 1000 * m_period_analysis ) );

	const auto board = 
		std::lower_bound( 
			m_executed_tasks.begin(), 
			m_executed_tasks.end(), 
			executed_task_t( board_time ) );
	
	for( auto it = m_executed_tasks.begin(); 
		it != board; ++it )
	{
		m_sum_size -= it->m_size;
	}

	for( auto it = m_executed_tasks.cbegin(); it != board; ++it )
	{
		++m_power_outgoing_counter;
	}
	m_executed_tasks.erase( m_executed_tasks.begin(), board );

	if ( m_executed_tasks.empty() )
	{
		assess();
	}

	if ( m_power_outgoing_counter >= m_power )
	{
		m_power_outgoing_counter = 0;
		assess();
	}
}

float
performance_assessor_t::get_assess_performance_in_size() const 
{
	return m_assess_performance_in_size;
}

float
performance_assessor_t::get_assess_performance_in_tasks() const 
{
	return m_assess_performance_in_tasks;
}

void
performance_assessor_t::assess()
{
	m_assess_performance_in_size = 
		static_cast<float>( m_sum_size ) / m_period_analysis * 1000 / m_power;

	m_assess_performance_in_tasks = 
		static_cast<float>( m_executed_tasks.size() ) / m_period_analysis * 1000 / m_power;
}

bool
performance_assessor_t::active() const
{
	return true;
}

//
// performance_assessor_dummy_t
//

void
performance_assessor_dummy_t::add( unsigned int size )
{
}

void
performance_assessor_dummy_t::cleanup()
{
}

float
performance_assessor_dummy_t::get_assess_performance_in_size() const
{
	return 0;
}

float
performance_assessor_dummy_t::get_assess_performance_in_tasks() const
{
	return 0;
}

bool
performance_assessor_dummy_t::active() const
{
	return false;
}

performance_assessor_interface_t *
performance_assessor_factory( 
	const performance_assessor::performance_assessor_type_t & 
		performance_assessor_type,
	unsigned int period_analysis,
	unsigned int assess_power )
{
	switch( performance_assessor_type )
	{
		case performance_assessor::dummy:
			return new performance_assessor_dummy_t();
		case performance_assessor::simple:
			return new performance_assessor_t( 
				period_analysis,
				assess_power );
		default:
			throw std::runtime_error( 
				"Incorrect performance_assessor_type: " + 
				cpp_util_2::slexcast( performance_assessor_type ) + ";" );
	}
}

} /* namespace tds */
