# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build"

# Include any dependencies generated for this target.
include src/CMakeFiles/pichat_voice.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/pichat_voice.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/pichat_voice.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/pichat_voice.dir/flags.make

src/CMakeFiles/pichat_voice.dir/codegen:
.PHONY : src/CMakeFiles/pichat_voice.dir/codegen

src/CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.o: src/CMakeFiles/pichat_voice.dir/flags.make
src/CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.o: /Users/orenarbel-wood/Desktop/Uni\ Classes/Year\ 4/Year\ 4\ Sem\ 2/CS3307B/pichat/src/voice/VoiceManager.cpp
src/CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.o: src/CMakeFiles/pichat_voice.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.o"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.o -MF CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.o.d -o CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.o -c "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/VoiceManager.cpp"

src/CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.i"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/VoiceManager.cpp" > CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.i

src/CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.s"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/VoiceManager.cpp" -o CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.s

src/CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.o: src/CMakeFiles/pichat_voice.dir/flags.make
src/CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.o: /Users/orenarbel-wood/Desktop/Uni\ Classes/Year\ 4/Year\ 4\ Sem\ 2/CS3307B/pichat/src/voice/SpeechRecognizer.cpp
src/CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.o: src/CMakeFiles/pichat_voice.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.o"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.o -MF CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.o.d -o CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.o -c "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/SpeechRecognizer.cpp"

src/CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.i"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/SpeechRecognizer.cpp" > CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.i

src/CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.s"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/SpeechRecognizer.cpp" -o CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.s

src/CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.o: src/CMakeFiles/pichat_voice.dir/flags.make
src/CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.o: /Users/orenarbel-wood/Desktop/Uni\ Classes/Year\ 4/Year\ 4\ Sem\ 2/CS3307B/pichat/src/voice/TextToSpeech.cpp
src/CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.o: src/CMakeFiles/pichat_voice.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.o"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.o -MF CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.o.d -o CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.o -c "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/TextToSpeech.cpp"

src/CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.i"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/TextToSpeech.cpp" > CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.i

src/CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.s"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/TextToSpeech.cpp" -o CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.s

src/CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.o: src/CMakeFiles/pichat_voice.dir/flags.make
src/CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.o: /Users/orenarbel-wood/Desktop/Uni\ Classes/Year\ 4/Year\ 4\ Sem\ 2/CS3307B/pichat/src/voice/CommandProcessor.cpp
src/CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.o: src/CMakeFiles/pichat_voice.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.o"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.o -MF CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.o.d -o CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.o -c "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/CommandProcessor.cpp"

src/CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.i"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/CommandProcessor.cpp" > CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.i

src/CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.s"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src/voice/CommandProcessor.cpp" -o CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.s

# Object files for target pichat_voice
pichat_voice_OBJECTS = \
"CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.o" \
"CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.o" \
"CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.o" \
"CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.o"

# External object files for target pichat_voice
pichat_voice_EXTERNAL_OBJECTS =

src/libpichat_voice.a: src/CMakeFiles/pichat_voice.dir/voice/VoiceManager.cpp.o
src/libpichat_voice.a: src/CMakeFiles/pichat_voice.dir/voice/SpeechRecognizer.cpp.o
src/libpichat_voice.a: src/CMakeFiles/pichat_voice.dir/voice/TextToSpeech.cpp.o
src/libpichat_voice.a: src/CMakeFiles/pichat_voice.dir/voice/CommandProcessor.cpp.o
src/libpichat_voice.a: src/CMakeFiles/pichat_voice.dir/build.make
src/libpichat_voice.a: src/CMakeFiles/pichat_voice.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libpichat_voice.a"
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/pichat_voice.dir/cmake_clean_target.cmake
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pichat_voice.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/pichat_voice.dir/build: src/libpichat_voice.a
.PHONY : src/CMakeFiles/pichat_voice.dir/build

src/CMakeFiles/pichat_voice.dir/clean:
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/pichat_voice.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/pichat_voice.dir/clean

src/CMakeFiles/pichat_voice.dir/depend:
	cd "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat" "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/src" "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build" "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src" "/Users/orenarbel-wood/Desktop/Uni Classes/Year 4/Year 4 Sem 2/CS3307B/pichat/build/src/CMakeFiles/pichat_voice.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : src/CMakeFiles/pichat_voice.dir/depend

