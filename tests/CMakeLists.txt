cmake_minimum_required(VERSION 3.10)

function(create_test name library)
	add_executable(${name} ${name}.cpp)
	
	target_include_directories(${name}
		PUBLIC
			${CONAN_LIBS}
	)
	target_link_libraries(${name}
		PUBLIC
			${CONAN_LIBS}
			${library}
	)
	add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}>)

	cotire(${name})
endfunction()

create_test(PhoneBookTests libpb)