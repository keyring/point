-- premake5.lua
workspace "point"
    configurations { "Debug", "Release" }
    platforms {"x64", "x86"}
    location "build"

    project "point"
        kind "ConsoleApp"
        language "C"
        location "build/point"

        targetdir "bin/%{cfg.buildcfg}"

        includedirs {
          "3rd",
          "3rd/ply-1.1",
        }
        files {
            "**.h",
            "**.c",
        }
        removefiles {"3rd/ply-1.1/plytest.c"}

        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"

        filter "configurations:Release"
            defines { "NDEBUG" }
            optimize "On"

    if _ACTION == "clean" then
        os.rmdir("bin")
        os.rmdir("build")
    end