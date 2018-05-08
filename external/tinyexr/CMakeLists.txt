cmake_minimum_required(VERSION 2.8.8)

include(ExternalProject)

ExternalProject_ADD(tinyexr
	GIT_REPOSITORY		https://github.com/syoyo/tinyexr.git
	GIT_TAG				master
	CONFIGURE_COMMAND	"" 	
	BUILD_COMMAND		"" 	
	INSTALL_COMMAND		""
	LOG_DOWNLOAD		ON)

ExternalProject_Get_Property(tinyexr source_dir)
set(TINYEXR_PATH ${source_dir} PARENT_SCOPE)
