
# Only things that need changing here.

set(projectName config)

set(sourceFiles
main.cpp
)


# No need to modify these lines.
add_executable(${projectName} ${sourceFiles})
if (projectProperties)
	set_target_properties(${projectName} ${projectProperties})
endif (projectProperties)
target_link_libraries(${projectName} ${projectLibs})


