cmake_minimum_required(VERSION 3.21)
project(chip8 LANGUAGES CXX VERSION 1.0)

add_executable(chip8
	"emulator/mapper.cpp"
	"emulator/screen.cpp"
	"emulator/keyboard.cpp"
	"emulator/processor.cpp"
	"emulator/main.cpp")

add_executable(chip8disasm
	"disasm/main.cpp"
)

target_include_directories(chip8
	PUBLIC "include/"
	PRIVATE "src/")

target_link_directories(chip8
	PUBLIC "lib/")

target_link_libraries(chip8
	sfml-graphics
	sfml-audio
	sfml-window
	sfml-system)

add_custom_command(TARGET chip8 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/share"
	$<TARGET_FILE_DIR:chip8>)

add_custom_command(TARGET chip8 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/resources"
	$<TARGET_FILE_DIR:chip8>/resources)