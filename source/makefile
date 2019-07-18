#       _              _____  _____   _____ 
#      | |            |  __ \|  __ \ / ____|
#      | | ___   ___  | |__) | |__) | |     
#  _   | |/ _ \ / _ \ |  _  /|  ___/| |     
# | |__| | (_) |  __/ | | \ \| |    | |____ 
#  \____/ \___/ \___| |_|  \_\_|     \_____|
                                           
# joerpc.o & joerpcl.o -> joerpc.exe & joerpcl.exe
all: builds/joerpc.exe builds/joel.exe

# joerpc.o -> joerpc.exe
builds/joerpc.exe: objects/joerpc.o builds/discord-rpc/discord-rpc.lib
	mkdir -p builds
	clang++ objects/joerpc.o -o builds/joerpc.exe -Lbuilds/discord-rpc/ -ldiscord-rpc -lAdvapi32 -lUser32

# joerpcl.o -> joel.exe
builds/joel.exe: objects/joerpcl.o
	mkdir -p builds
	clang++ objects/joerpcl.o -o builds/joel.exe -lAdvapi32 -User32

# joerpc.cxx -> joerpc.o
objects/joerpc.o: joerpc.cxx
	mkdir -p objects
	clang++ -c joerpc.cxx -o objects/joerpc.o -std=c++17 -Idiscord-rpc/include/ # -lAdvapi32 -lUser32

# joerpcl.cxx -> joerpcl.o
objects/joerpcl.o: joerpcl.cxx
	mkdir -p objects
	clang++ -c joerpcl.cxx -o objects/joerpcl.o -std=c++17

#  _____  _                       _   _____  _____   _____ 
# |  __ \(_)                     | | |  __ \|  __ \ / ____|
# | |  | |_ ___  ___ ___  _ __ __| | | |__) | |__) | |     
# | |  | | / __|/ __/ _ \| '__/ _` | |  _  /|  ___/| |     
# | |__| | \__ \ (_| (_) | | | (_| | | | \ \| |    | |____ 
# |_____/|_|___/\___\___/|_|  \__,_| |_|  \_\_|     \_____|

discord_rpc_objects = $(addprefix objects/discord-rpc/, discord_register_win.o connection_win.o rpc_connection.o serialization.o discord_rpc.o dllmain.o)
                                                         
# Discord-RPC Static library creation.
builds/discord-rpc/discord-rpc.lib: $(discord_rpc_objects)
	mkdir -p builds
	mkdir -p builds/discord-rpc/
	llvm-ar rcs builds/discord-rpc/discord-rpc.lib $(discord_rpc_objects)

# Discord-RPC Object file compilation
# vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

# discord_register_win.cpp > discord_register_win.o
objects/discord-rpc/discord_register_win.o: discord-rpc/source/discord_register_win.cpp
	mkdir -p objects
	mkdir -p objects/discord-rpc/
	clang++ -c discord-rpc/source/discord_register_win.cpp -o objects/discord-rpc/discord_register_win.o -Idiscord-rpc/include -std=c++17

# connection_win.cpp > connection_win.o
objects/discord-rpc/connection_win.o: discord-rpc/source/connection_win.cpp
	mkdir -p objects
	mkdir -p objects/discord-rpc/
	clang++ -c discord-rpc/source/connection_win.cpp -o objects/discord-rpc/connection_win.o -Idiscord-rpc/include -std=c++17
		
# rpc_connection.cpp > rpc_connection.o
objects/discord-rpc/rpc_connection.o: discord-rpc/source/rpc_connection.cpp
	mkdir -p objects
	mkdir -p objects/discord-rpc/	
	clang++ -c discord-rpc/source/rpc_connection.cpp -o objects/discord-rpc/rpc_connection.o -Idiscord-rpc/include -std=c++17
	
# serialization.cpp > serialization.o
objects/discord-rpc/serialization.o: discord-rpc/source/serialization.cpp
	mkdir -p objects
	mkdir -p objects/discord-rpc/	
	clang++ -c discord-rpc/source/serialization.cpp -o objects/discord-rpc/serialization.o -Idiscord-rpc/include -std=c++17
	
# discord_rpc.cpp > discord_rpc.o
objects/discord-rpc/discord_rpc.o: discord-rpc/source/discord_rpc.cpp
	mkdir -p objects
	clang++ -c discord-rpc/source/discord_rpc.cpp -o objects/discord-rpc/discord_rpc.o -Idiscord-rpc/include -std=c++17

# dllmain.cpp > dllmain.o
objects/discord-rpc/dllmain.o: discord-rpc/source/dllmain.cpp
	mkdir -p objects
	clang++ -c discord-rpc/source/dllmain.cpp -o objects/discord-rpc/dllmain.o -Idiscord-rpc/include -std=c++17