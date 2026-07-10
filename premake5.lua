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
    "%{prj.name}/vendor/stb_image/stb_image.cpp",

    -- Dear ImGui
    "%{prj.name}/vendor/imgui/imgui.cpp",
    "%{prj.name}/vendor/imgui/imgui_draw.cpp",
    "%{prj.name}/vendor/imgui/imgui_tables.cpp",
    "%{prj.name}/vendor/imgui/imgui_widgets.cpp",
    "%{prj.name}/vendor/imgui/backends/imgui_impl_glfw.cpp",
    "%{prj.name}/vendor/imgui/backends/imgui_impl_opengl3.cpp"
  }

  filter "files:**/glad.c"
    flags { "NoPCH" }

  filter "files:**/stb_image.cpp"
    flags { "NoPCH" }

  filter "files:**/imgui*.cpp"
    flags { "NoPCH" }

  filter {}

  includedirs
  {
    "%{prj.name}/src",

    "%{prj.name}/vendor/spdlog/include",
    "%{prj.name}/vendor/GLAD/include",
    "%{prj.name}/vendor/GLFW/include",
    "%{prj.name}/vendor/glm",
    "%{prj.name}/vendor/stb_image",
    "%{prj.name}/vendor/imgui",
    "%{prj.name}/vendor/imgui/backends",

    "F:/Development/vcpkg/installed/x64-windows/include"
  }

  defines
  {
    "GLFW_INCLUDE_NONE"
  }

  filter "system:windows"
    toolset "v145"
    cppdialect "C++20"
    staticruntime "Off"
    systemversion "latest"

    defines
    {
      "FT_PLATFORM_WINDOWS",
      "FT_BUILD_DLL",
      "FT_ENABLE_ASSIMP"
    }

    links
    {
      "glfw3",
      "opengl32",
      "assimp-vc145-mt"
    }

    libdirs
    {
      "%{prj.name}/vendor/GLFW/lib",
      "F:/Development/vcpkg/installed/x64-windows/lib"
    }

    postbuildcommands
    {
      "{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Futura",
      "{COPYFILE} F:/Development/vcpkg/installed/x64-windows/bin/assimp-vc145-mt.dll ../bin/" .. outputdir .. "/Futura",
      "{COPYFILE} F:/Development/vcpkg/installed/x64-windows/bin/kubazip.dll ../bin/" .. outputdir .. "/Futura",
      "{COPYFILE} F:/Development/vcpkg/installed/x64-windows/bin/minizip.dll ../bin/" .. outputdir .. "/Futura",
      "{COPYFILE} F:/Development/vcpkg/installed/x64-windows/bin/poly2tri.dll ../bin/" .. outputdir .. "/Futura",
      "{COPYFILE} F:/Development/vcpkg/installed/x64-windows/bin/pugixml.dll ../bin/" .. outputdir .. "/Futura",
      "{COPYFILE} F:/Development/vcpkg/installed/x64-windows/bin/z.dll ../bin/" .. outputdir .. "/Futura"

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
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/assets/scenes/**.scene"
  }

  removefiles
  {
    "%{prj.name}/assets/**.obj"
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
    toolset "v145"
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
