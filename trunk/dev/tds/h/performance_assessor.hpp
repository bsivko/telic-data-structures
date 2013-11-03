#if !defined( _TDS__PERFORMANCE_ASSESSOR_HPP__INCLUDED )
#define _TDS__PERFORMANCE_ASSESSOR_HPP__INCLUDED

#include <ace/Date_Time.h>

#include <deque>

#include <gtest/gtest_prod.h>

namespace tds {

namespace performance_assessor {

//! Возможные типы assessor'ов.
enum performance_assessor_type_t
{
	dummy,
	simple
};

};

//! Один элемент внутренней структуры performance_assessor_t.
struct executed_task_t
{
	//! Время, когда произошла вставка элемента.
	ACE_Time_Value m_time_in;
	//! Размер задачи (байт в посылке, размер пакета).
	unsigned int m_size;

	explicit executed_task_t( unsigned int size ) : 
		m_size( size ), 
		m_time_in( ACE_OS::gettimeofday() )
	{}

	explicit executed_task_t( const ACE_Time_Value & time ) : 
		m_size( 0 ), 
		m_time_in( time )
	{}
};

bool
operator < (const executed_task_t & left, const executed_task_t & right);

//! Общий интерфейс assessor'ов.
class performance_assessor_interface_t
{
	public:
		virtual
		~performance_assessor_interface_t() {}

		//! Добавить в хранилище событие.
		virtual void
		add( 
			//! Размер поставленной задачи.
			unsigned int size ) = 0;

		//! Произвести очистку хранилища.
		virtual void
		cleanup() = 0;

		//! Узнать расчетную мощность в размерах.
		virtual float
		get_assess_performance_in_size() const = 0;

		//! Узнать расчетную мощность в задачах.
		virtual float
		get_assess_performance_in_tasks() const = 0;

		//! Активен или нет экземпляр.
		virtual bool
		active() const = 0;
};

//! Структура данных для оценки фактической производительности 
//! системы по выполнению однотипных задач.
class performance_assessor_t : public performance_assessor_interface_t
{
	public:

		performance_assessor_t( 
			//! Период анализа (время жизни элементов).
			unsigned int period_analysis,
			//! Количество обрабатываемых задач одновременно.
			unsigned int power = 1 );

		virtual void
		add( 
			unsigned int size );

		virtual void
		cleanup();

		virtual float
		get_assess_performance_in_size() const;

		virtual float
		get_assess_performance_in_tasks() const;

		virtual bool
		active() const;

	private:
		FRIEND_TEST( PerformanceAssessor, TimeLowerBound );

		//! Произвести пересчет выходных параметров.
		void
		assess();

		typedef std::deque< executed_task_t > executed_tasks_t;

		//! Хранимые последние события.
		executed_tasks_t m_executed_tasks;

		//! Сумма размера задач в хранимых событиях.
		unsigned long m_sum_size;

		//! Период анализа (время жизни элементов, мс).
		const unsigned int m_period_analysis;

		//! Последная расчетная мощность в задачах.
		float m_assess_performance_in_tasks;
		//! Последная расчетная мощность в размерах.
		float m_assess_performance_in_size;

		//! Мощность приемника обработки.
		/*!
			Как много задач обработчик в состоянии обрабатывать одновременно.
		*/
		const unsigned int m_power;

		//! Внутренний счетчик power'ов.
		/*!
			Увеличивается при постановке задачи.
			Каждые m_power пересчет выходных параметров.
		*/
		unsigned int m_power_pool_counter;

		//! Счетчик уходящих power'ов.
		/*!
			Каждые m_power нужно обновлять выходные параметры.
			Обновление происходить только тогда, когда уходит m_power 
			до формирования выходного пакета power.
		*/
		unsigned int m_power_outgoing_counter;
};

//! Ничего не делает и возвращает 0.
class performance_assessor_dummy_t : public performance_assessor_interface_t
{
	public:
		virtual void
		add( unsigned int size );

		virtual void
		cleanup();

		virtual float
		get_assess_performance_in_size() const;

		virtual float
		get_assess_performance_in_tasks() const;

		virtual bool
		active() const;
};

performance_assessor_interface_t *
performance_assessor_factory( 
	const performance_assessor::performance_assessor_type_t & 
		performance_assessor_type,
	unsigned int period_analysis,
	unsigned int assess_power );

} /* namespace tds */

#endif
