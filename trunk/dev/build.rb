require 'rubygems'

gem 'Mxx_ru', '>= 1.5.2'

require 'mxx_ru/cpp'

MxxRu::Cpp::composite_target( MxxRu::BUILD_ROOT ) {

		global_include_path( "." )
		default_runtime_mode( MxxRu::Cpp::RUNTIME_RELEASE )
		MxxRu::enable_show_brief

		implib_path 'lib'
		required_prj "tds/prj.rb" 

		required_prj "gtest/prj.rb" 
#		required_prj "test/event_counter/prj.ut.rb" 
		required_prj "test/sum_counter/prj.ut.rb" 
		required_prj "test/volume_controller/prj.ut.rb" 
#		required_prj "test/performance_assessor/prj.ut.rb" 
#		required_prj "test/performance_estimator/prj.ut.rb" 
}
