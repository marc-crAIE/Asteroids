project "PineconeGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"%{wks.location}/Pinecone/Pinecone/vendor/spdlog/include",
		"%{wks.location}/Pinecone/Pinecone/src",
		"%{wks.location}/Pinecone/Pinecone/vendor",
		"%{wks.location}/Pinecone/Pinecone/vendor/glm",
		"%{wks.location}/Pinecone/Pinecone/vendor/entt/include"
	}

	links
	{
		"Pinecone"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "PC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PC_DIST"
		runtime "Release"
		optimize "on"
