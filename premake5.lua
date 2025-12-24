workspace "Futura"
  architecture "x64"
  startproject "Futura"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

------------------------------------------------------------
-- FuturaLibrary (Shared Library)
------------------------------------------------------------
project "FuturaLibrary"
  location "FuturaLibrary"
  kind "SharedLib"
  language "C++"
  characterset ("Unicode")
  buildoptions { "/utf-8" }

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  pchheader "pch.h"
  pchsource "FuturaLibrary/src/pch.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",

    -- GLAD
    "%{prj.name}/vendor/GLAD/src/glad.c",

    -- stb_image implementation
    "%{prj.name}/vendor/stb_image/stb_image.cpp"
  }

  filter "files:**/glad.c"
    flags { "NoPCH" }

  filter "files:**/stb_image.cpp"
    flags { "NoPCH" }

  filter {}

  includedirs
  {
    "%{prj.name}/src",

    "%{prj.name}/vendor/spdlog/include",
    "%{prj.name}/vendor/GLAD/include",
    "%{prj.name}/vendor/GLFW/include",
    "%{prj.name}/vendor/glm",
    "%{prj.name}/vendor/stb_image"
  }

  defines
  {
    "GLFW_INCLUDE_NONE"
  }

  filter "system:windows"
    toolset "v143"
    cppdialect "C++20"
    staticruntime "Off"
    systemversion "latest"

    defines
    {
      "FT_PLATFORM_WINDOWS",
      "FT_BUILD_DLL"
    }

    links
    {
      "glfw3",
      "opengl32"
    }

    libdirs
    {
      "%{prj.name}/vendor/GLFW/lib"
    }

    postbuildcommands
    {
      "{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Futura"

      -- "{COPYDIR} ../Futura/assets %{cfg.targetdir}/assets"
    }

  filter "system:linux"
    cppdialect "C++20"
    buildoptions { "-fPIC" }
    linkoptions { "-std=c++20" }

    defines
    {
      "FT_PLATFORM_LINUX",
      "FT_BUILD_DLL"
    }

    links
    {
      "glfw",
      "GL"
    }

  filter "configurations:Debug"
    defines "FT_DEBUG"
    symbols "On"

  filter "configurations:Release"
    defines "FT_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    defines "FT_DIST"
    optimize "On"

------------------------------------------------------------
-- Futura (Application)
------------------------------------------------------------
project "Futura"
  location "Futura"
  kind "ConsoleApp"
  language "C++"
  characterset ("Unicode")
  buildoptions { "/utf-8" }

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "FuturaLibrary/src",
    "FuturaLibrary/vendor/spdlog/include",
    "FuturaLibrary/vendor/GLAD/include",
    "FuturaLibrary/vendor/GLFW/include",
    "FuturaLibrary/vendor/glm",
    "FuturaLibrary/vendor/stb_image"
  }

  links
  {
    "FuturaLibrary"
  }

  filter "system:windows"
    toolset "v143"
    cppdialect "C++20"
    staticruntime "Off"
    systemversion "latest"

    defines
    {
      "FT_PLATFORM_WINDOWS"
    }

  filter "system:linux"
    cppdialect "C++20"
    linkoptions { "-std=c++20" }
    buildoptions { "-fPIC" }

    defines
    {
      "FT_PLATFORM_LINUX"
    }

  filter "configurations:Debug"
    defines "FT_DEBUG"
    symbols "On"

  filter "configurations:Release"
    defines "FT_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    defines "FT_DIST"
    optimize "On"
