


# Only things that need changing here.

set(projectName PuzzleGame)

set(sourceFiles
main.cpp
Level.cpp
Map.cpp
Events.cpp
Editor.cpp
#MonsterCharacterController.cpp
#MainMenu.cpp
Positioner.cpp
#RoundedGUISkin.cpp
MainState.cpp
StartScreen.cpp
SimpleMenu.cpp
EndLevelScreen.cpp
level_stats.cpp
volume.cpp
options.cpp
)


# No need to modify these lines.
add_executable(${projectName}_FullVersion ${sourceFiles})
if (projectProperties)
	set_target_properties(${projectName}_FullVersion ${projectProperties})
endif (projectProperties)
target_link_libraries(${projectName}_FullVersion ${projectLibs})


# Special for this project: Trial version!
add_executable(${projectName}_TrialVersion ${sourceFiles})
if (projectProperties)
	set_target_properties(${projectName}_TrialVersion ${projectProperties} COMPILE_FLAGS -DTRIAL)
else (projectProperties)
	set_target_properties(${projectName}_TrialVersion PROPERTIES COMPILE_FLAGS -DTRIAL)
endif (projectProperties)
target_link_libraries(${projectName}_TrialVersion ${projectLibs})
