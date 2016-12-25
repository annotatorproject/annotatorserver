
#
# External dependencies
#

# find_package(THIRDPARTY REQUIRED)


#
# Library name and options
#

# Target name
set(target annotatorwebclient)

# Exit here if required dependencies are not met
message(STATUS "Exe ${target}")


# Set API export file and macro
string(TOUPPER ${target} target_upper)

#
# Sources
#

set(app_path "${CMAKE_CURRENT_SOURCE_DIR}/app/")
set(styles_path  "${CMAKE_CURRENT_SOURCE_DIR}/styles")
set(index_file "${CMAKE_CURRENT_SOURCE_DIR}/index.html")

add_custom_target(${target} ALL)

add_custom_command(TARGET ${target}
	PRE_BUILD
        COMMAND npm install ${CMAKE_CURRENT_SOURCE_DIR} --prefix ${PROJECT_BINARY_DIR}/resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${app_path} ${PROJECT_BINARY_DIR}/resources/app
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${styles_path} ${PROJECT_BINARY_DIR}/resources/styles
        COMMAND ${CMAKE_COMMAND} -E copy ${index_file} ${PROJECT_BINARY_DIR}/resources
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
