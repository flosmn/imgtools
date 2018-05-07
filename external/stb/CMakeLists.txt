cmake_minimum_required(VERSION 2.8.8)

include(ExternalProject)

ExternalProject_ADD(stb
	GIT_REPOSITORY		https://github.com/nothings/stb.git
	GIT_TAG				master
	CONFIGURE_COMMAND	"" 	
	BUILD_COMMAND		"" 	
	INSTALL_COMMAND		""
	LOG_DOWNLOAD		ON)

ExternalProject_Get_Property(stb source_dir)
set(STB_PATH ${source_dir} PARENT_SCOPE)
