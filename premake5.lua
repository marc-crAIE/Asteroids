workspace "PineconeGame"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "PineconeGame"
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Pinecone/PineconeExternal.lua"

include "PineconeGame"