
require 'rubygems'

gem 'Mxx_ru', '>= 1.4.7'

require 'mxx_ru/cpp'

MxxRu::Cpp::exe_target {

	implib_path 'lib'

	target 'test.event_counter'
	required_prj 'tds/prj.rb'

#	required_prj 'ace/ace_lib_unpacker.rb'
#	required_prj 'ace/dll.rb'

	required_prj 'gtest/prj.rb'

	cpp_source 'main.cpp'
}
