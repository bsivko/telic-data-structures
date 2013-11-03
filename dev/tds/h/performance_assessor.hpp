#if !defined( _TDS__PERFORMANCE_ASSESSOR_HPP__INCLUDED )
#define _TDS__PERFORMANCE_ASSESSOR_HPP__INCLUDED

#include <ace/Date_Time.h>

#include <deque>

#include <gtest/gtest_prod.h>

namespace tds {

namespace performance_assessor {

//! ��������� ���� assessor'��.
enum performance_assessor_type_t
{
	dummy,
	simple
};

};

//! ���� ������� ���������� ��������� performance_assessor_t.
struct executed_task_t
{
	//! �����, ����� ��������� ������� ��������.
	ACE_Time_Value m_time_in;
	//! ������ ������ (���� � �������, ������ ������).
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

//! ����� ��������� assessor'��.
class performance_assessor_interface_t
{
	public:
		virtual
		~performance_assessor_interface_t() {}

		//! �������� � ��������� �������.
		virtual void
		add( 
			//! ������ ������������ ������.
			unsigned int size ) = 0;

		//! ���������� ������� ���������.
		virtual void
		cleanup() = 0;

		//! ������ ��������� �������� � ��������.
		virtual float
		get_assess_performance_in_size() const = 0;

		//! ������ ��������� �������� � �������.
		virtual float
		get_assess_performance_in_tasks() const = 0;

		//! ������� ��� ��� ���������.
		virtual bool
		active() const = 0;
};

//! ��������� ������ ��� ������ ����������� ������������������ 
//! ������� �� ���������� ���������� �����.
class performance_assessor_t : public performance_assessor_interface_t
{
	public:

		performance_assessor_t( 
			//! ������ ������� (����� ����� ���������).
			unsigned int period_analysis,
			//! ���������� �������������� ����� ������������.
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

		//! ���������� �������� �������� ����������.
		void
		assess();

		typedef std::deque< executed_task_t > executed_tasks_t;

		//! �������� ��������� �������.
		executed_tasks_t m_executed_tasks;

		//! ����� ������� ����� � �������� ��������.
		unsigned long m_sum_size;

		//! ������ ������� (����� ����� ���������, ��).
		const unsigned int m_period_analysis;

		//! ��������� ��������� �������� � �������.
		float m_assess_performance_in_tasks;
		//! ��������� ��������� �������� � ��������.
		float m_assess_performance_in_size;

		//! �������� ��������� ���������.
		/*!
			��� ����� ����� ���������� � ��������� ������������ ������������.
		*/
		const unsigned int m_power;

		//! ���������� ������� power'��.
		/*!
			������������� ��� ���������� ������.
			������ m_power �������� �������� ����������.
		*/
		unsigned int m_power_pool_counter;

		//! ������� �������� power'��.
		/*!
			������ m_power ����� ��������� �������� ���������.
			���������� ����������� ������ �����, ����� ������ m_power 
			�� ������������ ��������� ������ power.
		*/
		unsigned int m_power_outgoing_counter;
};

//! ������ �� ������ � ���������� 0.
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
