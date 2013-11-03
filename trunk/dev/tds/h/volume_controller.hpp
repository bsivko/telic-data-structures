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

#if !defined( _TDS__VOLUME_CONTROLLER_HPP__INCLUDED )
#define _TDS__VOLUME_CONTROLLER_HPP__INCLUDED

#include <limits.h>

namespace tds {

//! Типы возможных контроллеров.
enum volume_controller_type_t
{
	dummy,
	constant
};

//! Класс интерфейса контроля состояния канала с точки зрения его нагрузки.
class volume_controller_interface_t
{
	public:
		virtual
		~volume_controller_interface_t() {}

		//! В канал отправлено заданное число нагрузки.
		virtual void
		loaded( unsigned int size ) = 0;

		//! Канал был разгружен на заданную нагрузку.
		virtual void
		unloaded( unsigned int size ) = 0;

		//! Какая нагрузка разрешена каналом.
		virtual unsigned int
		how_much_size_allowed() const = 0;

		//! Какое количество задач разрешено в канале.
		virtual unsigned int
		how_much_tasks_allowed() const = 0;

		//! Активен контроллер или нет.
		virtual bool
		active() const = 0;
};

//! Класс контроля состояния канала с точки зрения его нагрузки.
/*!
	Ему передается информация о событиях, 
	а он сообщает, сколько можно отправить в канал.
*/
class volume_constant_controller_t : public volume_controller_interface_t
{
	public:

		volume_constant_controller_t( 
			//! Максимальная нагрузка канала (объем).
			unsigned int max_size_in_volume,
			//! Максимальная нагрузка канала (задачи).
			unsigned int max_tasks_in_volume );

		virtual void
		loaded( unsigned int size );

		virtual void
		unloaded( unsigned int size );

		virtual unsigned int
		how_much_size_allowed() const;

		virtual unsigned int
		how_much_tasks_allowed() const;

		virtual bool
		active() const;

	private:
		//! Насколько канал загружен (объем).
		unsigned int m_total_size_loaded;
		//! Насколько канал загружен (задачи).
		unsigned int m_total_tasks_loaded;
		//! Сколько максимум может быть в канале (объем).
		unsigned int m_max_size_in_volume;
		//! Сколько максимум может быть в канале (задачи).
		unsigned int m_max_tasks_in_volume;
};

//! Контроллер, который не накладывает ограничений (пустышка).
class volume_dummy_controller_t : public volume_controller_interface_t
{
	public:

		volume_dummy_controller_t();

		virtual void
		loaded( unsigned int size );

		virtual void
		unloaded( unsigned int size );

		virtual unsigned int
		how_much_size_allowed() const;

		virtual unsigned int
		how_much_tasks_allowed() const;

		virtual bool
		active() const;
};

//! Фабрика для производства контроллеров.
volume_controller_interface_t *
volume_controller_factory( 
	const volume_controller_type_t & volume_controller_type,
	unsigned int max_size_in_volume,
	unsigned int max_packets_in_volume );

} /* namespace tds */

#endif
