require 'rubygems'

gem 'Mxx_ru', '>= 1.4.7'

require 'mxx_ru/cpp'

MxxRu::Cpp::lib_target {

	include_path( "gtest/1.6.0/include", Mxx_ru::Cpp::Target::OPT_UPSPREAD )
	include_path( "gtest/1.6.0", Mxx_ru::Cpp::Target::OPT_UPSPREAD )

	implib_path 'lib'

        if "unix" == toolset.tag( "target_os" )
	    lib 'pthread'
	end

	target 'gtest'

	cpp_source '1.6.0/src/gtest-all.cc'
}
