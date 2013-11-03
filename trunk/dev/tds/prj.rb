require 'mxx_ru/cpp'

require 'tds/version.rb'

MxxRu::Cpp::lib_target {
#	required_prj 'ace/dll.rb'

	rtl_mode( Mxx_ru::Cpp::RTL_SHARED )

	required_prj 'gtest/prj.rb'

	target( "tds." + Tds::VERSION )
	implib_path 'lib'
	
	define( "TDS_PRJ" )

#	cpp_source 'performance_assessor.cpp' 
#	cpp_source 'performance_estimator.cpp' 
	cpp_source 'volume_controller.cpp' 
#	cpp_source 'event_counter.cpp' 
	cpp_source 'sum_counter.cpp' 
}
