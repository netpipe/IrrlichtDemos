solution "irrDynamics"
   configurations { "Debug", "Release" }
 
   -- A project defines one build target
defines { "LINUX" }


includedirs { "include",
			  "/usr/include/irrlicht",
			  "/usr/include/bullet" }
 
libdirs {  }

project "irrdynamics"
  targetdir "lib/"
  kind "SharedLib"
  language "C++"
  files { "include/**.h",
	      "src/*.cpp"}

configuration "Debug"
  defines {"LINUX", "DEBUG", "_DEBUG" }
  flags { "Symbols"}
  buildoptions { "-ggdb", "-fPIC" }
  links { }
  objdir "obj"
  targetname "irrdynamicsD"




