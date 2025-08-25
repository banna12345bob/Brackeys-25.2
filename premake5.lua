workspace "Brackeys 25.2"
	startproject "game"

	configurations
	{
		"Debug_x86_64",
		"Release_x86_64",
		"Dist_x86_64"
	}
	filter "configurations:*_x86_64"
		architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/engine/vendor/spdlog/include"
IncludeDir["SDL3"] = "%{wks.location}/engine/vendor/SDL3-3.2.10/include"
IncludeDir["GLFW"] = "%{wks.location}/engine/vendor/GLFW/include"
IncludeDir["ImGui"] = "%{wks.location}/engine/vendor/imgui"
IncludeDir["stb_image"] = "%{wks.location}/engine/vendor/stb_image"
IncludeDir["glad"] = "%{wks.location}/engine/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/engine/vendor/glm"
IncludeDir["json"] = "%{wks.location}/engine/vendor/json/include"
IncludeDir["zipFile"] = "%{wks.location}/engine/vendor/zipFile"

group "Dependencies"
	include "engine/vendor/imgui"
	include "engine/vendor/glad"
	include "engine/vendor/GLFW"

group ""
	include "engine"
	include "game"
	include "sandbox"