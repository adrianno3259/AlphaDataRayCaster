	
workspace "AlphaDataRayCaster"
   configurations { "Debug", "Release", "FPGA" }


project "DarkRenderer"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   flags = { "-std=c++11" }

   includedirs {"include"}
   files { "include/**.h", "src/**.cpp", "main.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   filter "configurations:FPGA"
      defines { "FPGA" }
      optimize "On"
