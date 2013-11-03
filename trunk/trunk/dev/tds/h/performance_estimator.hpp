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

//! Возможные типы estimator'ов.
enum performance_estimator_type_t
{
	dummy,
	simple
};

};

//! Один элемент внутренней структуры performance_estimator_t.
struct solved_task_t
{
	//! Время, когда произошла вставка элемента.
	ACE_Time_Value m_time_in;
	//! Сколько времени задача выполнялось, мс.
	/*!
		Сколько был пакет в пути, сколько выполнялся HTTP запрос.
	*/
	unsigned int m_time_in_progress;
	//! Размер задачи (байт в посылке, размер пакета).
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

//! Общий интерфейс estimator'ов.
class performance_estimator_interface_t
{
	public:
		virtual
		~performance_estimator_interface_t() {}

		//! Добавить в хранилище событие.
		virtual void
		add( 
			//! Сколько времени задача выполнялось, мс.
			unsigned int time_in_progress, 
			//! Размер задачи (байт в посылке, размер пакета).
			unsigned int size ) = 0;

		//! Произвести очистку хранилища.
		virtual void
		cleanup() = 0;

		//! Узнать расчетную мощность в размерах.
		virtual float
		get_estimate_performance_in_size() const = 0;

		//! Узнать расчетную мощность в задачах.
		virtual float
		get_estimate_performance_in_tasks() const = 0;

		//! Активен или выключен.
		virtual bool
		active() const = 0;
};

//! Структура данных для оценки производительности 
//! системы по выполнению однотипных задач.
class performance_estimator_t : public performance_estimator_interface_t
{
	public:

		performance_estimator_t( 
			//! Период анализа (время жизни элементов).
			unsigned int period_analysis,
			//! Стартовая расчетная мощность в задачах.
			float start_estimate_performance_in_tasks,
			//! Стартовая расчетная мощность в размерах.
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

		//! Произвести пересчет выходных параметров.
		void
		estimate();

		typedef std::deque< solved_task_t > solved_tasks_t;

		//! Хранимые последние выполненные задачи.
		solved_tasks_t m_solved_tasks;

		//! Сумма времени в хранимых событиях.
		unsigned long m_sum_time_in_progress;
		//! Сумма сообщений в хранимых событиях.
		unsigned long m_sum_size;

		//! Период анализа (время жизни элементов, мс).
		const unsigned int m_period_analysis;

		//! Последная расчетная мощность в задачах.
		float m_estimate_performance_in_tasks;
		//! Последная расчетная мощность в размерах.
		float m_estimate_performance_in_size;
};

//! Дает бесконечную производительность.
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
