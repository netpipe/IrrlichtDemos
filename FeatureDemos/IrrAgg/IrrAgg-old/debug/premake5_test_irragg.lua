--[[
Premake5 debug file for file test_irragg.cpp

To use, you will need the program Premake5, which you can download here:
https://premake.github.io/download.html

To create a GCC make file, enter the following into the terminal:
premake5 --file=premake5_test_irragg.lua gmake

ATTN Users:
This file will not work straight out of the box.
You will need to set the variable v_aggdir to the relative path to your Anti-Grain
Geometry (AGG) files.
Linux users will need to have Irrlicht built and installed. The default install settings
will work. See the Irrlicht build instructions in its source folder for more information.

ATTN Windows users:
This premake file has not been fully completed for Windows.
To finish it, file inclusions will need to be added to the "Assume Windows" section.
--]]

-- AGG directory. You will need to set this yourself. For example:
-- Go up to the parent C++ folder: C++ <- Projects <-- IrrAgg <-- debug
local v_aggdir = "../../../Anti-Grain/agg-2.5/"

-- build for local platform or linux by default
local v_platform = iif ( os.is("windows"), "windows", "linux" )

workspace "test irragg"
--solution "agg first test"
	location "build"
	targetdir "bin"
	kind "ConsoleApp"
	configurations { "default" }

project "test irragg"
	buildoptions {
		"-Wall"
	}
	includedirs {
		v_aggdir ,
		v_aggdir .. "include/" ,
		v_aggdir .. "src/" ,
		"/usr/local/include/irrlicht"
	}
	files {
		"test_irragg.cpp",
		"../include/**.h",
		"../src/**.h",
		"../src/**.cpp",
		v_aggdir .. "include/*.h" ,
		v_aggdir .. "src/*.cpp" ,
	}

	if ( v_platform == "linux" ) then
		targetname "test_irragg.out"
		files {
			v_aggdir .. "src/ctrl/*.cpp" ,
		}
		includedirs {
			"/usr/local/include/irrlicht"
		}
		links {
			"X11",
			"Irrlicht"
		}
	else
		-- Assume Windows
		targetname "test_irragg.exe"

		-- Windows users will need to set the directory of Irrlicht header files
	end

