set(SOURCES ossource.cpp ../osinclude.h)

add_library(${GLSLANG_TARGET_PREFIX}OSDependent STATIC ${SOURCES})
set_property(TARGET ${GLSLANG_TARGET_PREFIX}OSDependent PROPERTY FOLDER glslang)
set_property(TARGET ${GLSLANG_TARGET_PREFIX}OSDependent PROPERTY POSITION_INDEPENDENT_CODE ON)

# MinGW GCC complains about function pointer casts to void*.
# Turn that off with -fpermissive.
if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    target_compile_options(${GLSLANG_TARGET_PREFIX}OSDependent PRIVATE -fpermissive)
endif()

if(WIN32)
    source_group("Source" FILES ${SOURCES})
endif(WIN32)

if(ENABLE_GLSLANG_INSTALL)
    install(TARGETS ${GLSLANG_TARGET_PREFIX}OSDependent EXPORT OSDependentTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
	install(EXPORT OSDependentTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)
endif(ENABLE_GLSLANG_INSTALL)
