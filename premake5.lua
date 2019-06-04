	
workspace "AlphaDataRayCaster"
   configurations { "Debug", "Release", "FPGA" }


project "DarkRenderer"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    buildoptions "-std=c++11"

    includedirs {"include", "deps"}
    files { "include/**.h", "src/**.cpp", "main.cpp", "deps/**.hpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "configurations:FPGA"
        defines { "FPGA" }
        linkoptions { "-ladmxrc3", "-lc" }
        includedirs {"/home/fpgadev/Documents/Adrianno/AlphaData/adb3_drv-1.4.18/include"}
        optimize "On"
