
# Only things that need changing here.

set(projectName tests)

set(sourceFiles
main.cpp
)


# No need to modify these lines.
add_executable(${projectName} ${sourceFiles})
set_target_properties(${projectName} ${projectProperties})
target_link_libraries(${projectName} ${projectLibs})


