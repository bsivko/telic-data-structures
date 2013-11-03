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

#include <tds/h/performance_estimator.hpp>

#include <algorithm>
#include <vector>

#include <limits>

#include <stdexcept>

#include <cpp_util_2/h/lexcast.hpp>

namespace tds {

bool
operator < (const solved_task_t & left, const solved_task_t & right)
{
	return (left.m_time_in < right.m_time_in);
}

//
// performance_estimator_t
//

performance_estimator_t::performance_estimator_t( 
	unsigned int period_analysis,
	float start_estimate_performance_in_tasks,
	float start_estimate_performance_in_size ) :
	m_period_analysis( period_analysis ),
	m_sum_time_in_progress( 0 ), 
	m_sum_size( 0 ),
	m_estimate_performance_in_tasks( start_estimate_performance_in_tasks ),
	m_estimate_performance_in_size( start_estimate_performance_in_size )
{
}

void
performance_estimator_t::add( 
	unsigned int time_in_way, 
	unsigned int size )
{
	m_solved_tasks.push_back( solved_task_t( time_in_way, size ) );
	m_sum_time_in_progress += time_in_way;
	m_sum_size += size;
	
	estimate();
}

void
performance_estimator_t::cleanup()
{
	if( m_solved_tasks.empty() )
		return;

	const ACE_Time_Value board_time ( 
		ACE_OS::gettimeofday() - ACE_Time_Value( 0, 1000 * m_period_analysis ) );

	const solved_tasks_t::iterator board = 
		std::lower_bound( 
			m_solved_tasks.begin(), 
			m_solved_tasks.end(), 
			solved_task_t( board_time ) );
	
	for( solved_tasks_t::iterator it = m_solved_tasks.begin(); it != board; ++it )
	{
		m_sum_time_in_progress -= it->m_time_in_progress;
		m_sum_size -= it->m_size;
	}

	m_solved_tasks.erase( m_solved_tasks.begin(), board );
}

float
performance_estimator_t::get_estimate_performance_in_size() const 
{
	return m_estimate_performance_in_size;
}

float
performance_estimator_t::get_estimate_performance_in_tasks() const 
{
	return m_estimate_performance_in_tasks;
}

void
performance_estimator_t::estimate()
{
	if ( 
		( !m_solved_tasks.empty() ) &&
		( m_sum_time_in_progress != 0 ) )
	{
		m_estimate_performance_in_size = 
			static_cast<float>( m_sum_size ) / m_sum_time_in_progress * 1000;
	}

	if ( 
		( !m_solved_tasks.empty() ) && 
		( m_sum_time_in_progress != 0 ) )
	{
		m_estimate_performance_in_tasks = 
			static_cast<float>( m_solved_tasks.size() ) / m_sum_time_in_progress * 1000;
	}
}

bool
performance_estimator_t::active() const
{
	return true;
}

//
// performance_estimator_dummy_t
//

void
performance_estimator_dummy_t::add( 
	unsigned int time_in_progress, 
	unsigned int size )
{}

void
performance_estimator_dummy_t::cleanup()
{}

float
performance_estimator_dummy_t::get_estimate_performance_in_size() const
{
	return FLT_MAX;
}

float
performance_estimator_dummy_t::get_estimate_performance_in_tasks() const
{
	return FLT_MAX;
}

bool
performance_estimator_dummy_t::active() const
{
	return false;
}

performance_estimator_interface_t *
performance_estimator_factory( 
	const performance_estimator::performance_estimator_type_t & 
		performance_estimator_type,
	unsigned int period_analysis,
	float start_estimate_performance_in_tasks,
	float start_estimate_performance_in_size )
{
	switch( performance_estimator_type )
	{
		case performance_estimator::dummy:
			return new performance_estimator_dummy_t();
		case performance_estimator::simple:
			return new performance_estimator_t( 
				period_analysis,
				start_estimate_performance_in_tasks,
				start_estimate_performance_in_size );
		default:
			throw std::runtime_error( 
				"Incorrect performance_estimator_type: " + 
				cpp_util_2::slexcast( performance_estimator_type ) + ";" );
	}
}

} /* namespace tds */
