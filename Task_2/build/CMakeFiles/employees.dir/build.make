# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/artem/Документы/453502/ОАиП/LR8/Task_2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/artem/Документы/453502/ОАиП/LR8/Task_2/build

# Include any dependencies generated for this target.
include CMakeFiles/employees.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/employees.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/employees.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/employees.dir/flags.make

CMakeFiles/employees.dir/main.cpp.o: CMakeFiles/employees.dir/flags.make
CMakeFiles/employees.dir/main.cpp.o: /home/artem/Документы/453502/ОАиП/LR8/Task_2/main.cpp
CMakeFiles/employees.dir/main.cpp.o: CMakeFiles/employees.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/artem/Документы/453502/ОАиП/LR8/Task_2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/employees.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/employees.dir/main.cpp.o -MF CMakeFiles/employees.dir/main.cpp.o.d -o CMakeFiles/employees.dir/main.cpp.o -c /home/artem/Документы/453502/ОАиП/LR8/Task_2/main.cpp

CMakeFiles/employees.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/employees.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/artem/Документы/453502/ОАиП/LR8/Task_2/main.cpp > CMakeFiles/employees.dir/main.cpp.i

CMakeFiles/employees.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/employees.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/artem/Документы/453502/ОАиП/LR8/Task_2/main.cpp -o CMakeFiles/employees.dir/main.cpp.s

# Object files for target employees
employees_OBJECTS = \
"CMakeFiles/employees.dir/main.cpp.o"

# External object files for target employees
employees_EXTERNAL_OBJECTS =

employees: CMakeFiles/employees.dir/main.cpp.o
employees: CMakeFiles/employees.dir/build.make
employees: /usr/lib/x86_64-linux-gnu/libcurl.so
employees: CMakeFiles/employees.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/artem/Документы/453502/ОАиП/LR8/Task_2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable employees"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/employees.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/employees.dir/build: employees
.PHONY : CMakeFiles/employees.dir/build

CMakeFiles/employees.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/employees.dir/cmake_clean.cmake
.PHONY : CMakeFiles/employees.dir/clean

CMakeFiles/employees.dir/depend:
	cd /home/artem/Документы/453502/ОАиП/LR8/Task_2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/artem/Документы/453502/ОАиП/LR8/Task_2 /home/artem/Документы/453502/ОАиП/LR8/Task_2 /home/artem/Документы/453502/ОАиП/LR8/Task_2/build /home/artem/Документы/453502/ОАиП/LR8/Task_2/build /home/artem/Документы/453502/ОАиП/LR8/Task_2/build/CMakeFiles/employees.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/employees.dir/depend

