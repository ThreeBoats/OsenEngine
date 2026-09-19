#the main engine cmake file. 



#for now OsenEngine gets included in the GAME executable, for easier start-up
add_library(OsenEngine STATIC)

#list of "systems/modules" that should be in osen engine
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/source/OsenEngine/Core")


#the engine internals and the games can use all .h files. that is a #TODO
target_include_directories(OsenEngine PUBLIC 
	"${CMAKE_CURRENT_SOURCE_DIR}/source"
)


#define the preprocessor directives
target_compile_definitions(OsenEngine PUBLIC 
	$<$<CONFIG:Debug>:OSEN_DEBUG>
	$<$<CONFIG:Development>:OSEN_DEV>
	$<$<CONFIG:Release>:OSEN_RELEASE>
)

#make different build options. 
#the /Zi is msvc dependent, and thus to be removed 
#TODO
target_compile_options(OsenEngine PRIVATE
    $<$<CONFIG:Debug>:/Zi>
    $<$<CONFIG:Development>:/Zi>
    $<$<CONFIG:Release>:/Zi>
)