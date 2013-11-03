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

#if !defined( _TDS__PERFORMANCE_ESTIMATOR_HPP__INCLUDED )
#define _TDS__PERFORMANCE_ESTIMATOR_HPP__INCLUDED

#include <ace/Date_Time.h>

#include <deque>

#include <gtest/gtest_prod.h>

namespace tds {

namespace performance_estimator {

//! ��������� ���� estimator'��.
enum performance_estimator_type_t
{
	dummy,
	simple
};

};

//! ���� ������� ���������� ��������� performance_estimator_t.
struct solved_task_t
{
	//! �����, ����� ��������� ������� ��������.
	ACE_Time_Value m_time_in;
	//! ������� ������� ������ �����������, ��.
	/*!
		������� ��� ����� � ����, ������� ���������� HTTP ������.
	*/
	unsigned int m_time_in_progress;
	//! ������ ������ (���� � �������, ������ ������).
	unsigned int m_size;

	solved_task_t( unsigned int time_in_progress, unsigned int size ) : 
		m_size( size ), 
		m_time_in_progress( time_in_progress ),
		m_time_in( ACE_OS::gettimeofday() )
	{}

	explicit solved_task_t( const ACE_Time_Value & time ) : 
		m_size( 0 ), 
		m_time_in_progress( 0 ),
		m_time_in( time )
	{}
};

bool
operator < (const solved_task_t & left, const solved_task_t & right);

//! ����� ��������� estimator'��.
class performance_estimator_interface_t
{
	public:
		virtual
		~performance_estimator_interface_t() {}

		//! �������� � ��������� �������.
		virtual void
		add( 
			//! ������� ������� ������ �����������, ��.
			unsigned int time_in_progress, 
			//! ������ ������ (���� � �������, ������ ������).
			unsigned int size ) = 0;

		//! ���������� ������� ���������.
		virtual void
		cleanup() = 0;

		//! ������ ��������� �������� � ��������.
		virtual float
		get_estimate_performance_in_size() const = 0;

		//! ������ ��������� �������� � �������.
		virtual float
		get_estimate_performance_in_tasks() const = 0;

		//! ������� ��� ��������.
		virtual bool
		active() const = 0;
};

//! ��������� ������ ��� ������ ������������������ 
//! ������� �� ���������� ���������� �����.
class performance_estimator_t : public performance_estimator_interface_t
{
	public:

		performance_estimator_t( 
			//! ������ ������� (����� ����� ���������).
			unsigned int period_analysis,
			//! ��������� ��������� �������� � �������.
			float start_estimate_performance_in_tasks,
			//! ��������� ��������� �������� � ��������.
			float start_estimate_performance_in_size );

		virtual void
		add( 
			unsigned int time_in_progress, 
			unsigned int size );

		virtual void
		cleanup();

		virtual float
		get_estimate_performance_in_size() const;

		virtual float
		get_estimate_performance_in_tasks() const;

		virtual bool
		active() const;
	private:
		FRIEND_TEST( PerformanceEstimator, TimeLowerBound );

		//! ���������� �������� �������� ����������.
		void
		estimate();

		typedef std::deque< solved_task_t > solved_tasks_t;

		//! �������� ��������� ����������� ������.
		solved_tasks_t m_solved_tasks;

		//! ����� ������� � �������� ��������.
		unsigned long m_sum_time_in_progress;
		//! ����� ��������� � �������� ��������.
		unsigned long m_sum_size;

		//! ������ ������� (����� ����� ���������, ��).
		const unsigned int m_period_analysis;

		//! ��������� ��������� �������� � �������.
		float m_estimate_performance_in_tasks;
		//! ��������� ��������� �������� � ��������.
		float m_estimate_performance_in_size;
};

//! ���� ����������� ������������������.
class performance_estimator_dummy_t : public performance_estimator_interface_t
{
	public:
		virtual void
		add( unsigned int time_in_progress, 
			unsigned int size );

		virtual void
		cleanup();

		virtual float
		get_estimate_performance_in_size() const;

		virtual float
		get_estimate_performance_in_tasks() const;

		virtual bool
		active() const;
};

performance_estimator_interface_t *
performance_estimator_factory( 
	const performance_estimator::performance_estimator_type_t & 
		performance_estimator_type,
	unsigned int period_analysis,
	float start_estimate_performance_in_tasks,
	float start_estimate_performance_in_size );

} /* namespace tds */

#endif
