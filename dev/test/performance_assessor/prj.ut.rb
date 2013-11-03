require 'mxx_ru/binary_unittest'

path = 'test/performance_assessor'

MxxRu::setup_target(
	MxxRu::BinaryUnittestTarget.new(
		"#{path}/prj.ut.rb",
		"#{path}/prj.rb" ) ) 
