
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
set(node_modules_path  "${CMAKE_CURRENT_SOURCE_DIR}/node_modules")
set(styles_path  "${CMAKE_CURRENT_SOURCE_DIR}/styles")
set(index_file "${CMAKE_CURRENT_SOURCE_DIR}/index.html")

add_custom_target(${target} ALL)

add_custom_command(TARGET ${target}
	PRE_BUILD
	COMMAND npm install --prefix ${PROJECT_BINARY_DIR}/resources -g
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


#
# Deployment
#
#install(DIRECTORY ${resource_path} DESTINATION ${INSTALL_DATA} COMPONENT runtime)
file(COPY ${app_path} ${styles_path} ${index_file} DESTINATION ${PROJECT_BINARY_DIR}/resources)

# Executable
#install(TARGETS ${target}
#    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT runtime
#    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT runtime
#)
