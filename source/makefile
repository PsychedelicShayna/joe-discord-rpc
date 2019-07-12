joerpc_flags += -std=c++17
joerpc_flags += -Idiscord-rpc/include/
joerpc_flags += -Idiscord-rpc/include/
joerpc_flags += -lAdvapi32
joerpc_flags += -lUser32

joel_flags += -std=c++17
joel_flags += -lAdvapi32
joel_flags += -lUser32

deps += discord_register_win
deps += connection_win
deps += rpc_connection
deps += serialization
deps += discord_rpc
deps += dllmain

builds/joerpc.exe: objects/main.o $(addprefix objects/, $(addsuffix .o, $(deps)))
	mkdir -p builds
	clang++ objects/main.o $(addprefix objects/, $(addsuffix .o, $(deps))) -o builds/joerpc.exe $(joerpc_flags)

builds/joel.exe: objects/joerpcl.o
	mkdir -p builds
	clang++ objects/joerpcl.o -o builds/joel.exe $(joel_flags)

objects/joerpcl.o: joerpcl.cxx
	mkdir -p objects
	clang++ -c joerpcl.cxx -o objects/joerpcl.o $(joel_flags)

objects/main.o: joerpc.cxx
	mkdir -p objects
	clang++ -c joerpc.cxx -o objects/main.o $(joerpc_flags)
	
objects/discord_register_win.o: discord-rpc/source/discord_register_win.cpp
	mkdir -p objects
	clang++ -c discord-rpc/source/discord_register_win.cpp -o objects/discord_register_win.o $(joerpc_flags)
    
objects/connection_win.o: discord-rpc/source/connection_win.cpp
	mkdir -p objects
	clang++ -c discord-rpc/source/connection_win.cpp -o objects/connection_win.o $(joerpc_flags)
	
objects/rpc_connection.o: discord-rpc/source/rpc_connection.cpp
	mkdir -p objects
	clang++ -c discord-rpc/source/rpc_connection.cpp -o objects/rpc_connection.o $(joerpc_flags)
	
objects/serialization.o: discord-rpc/source/serialization.cpp
	mkdir -p objects
	clang++ -c discord-rpc/source/serialization.cpp -o objects/serialization.o $(joerpc_flags)
	
objects/discord_rpc.o: discord-rpc/source/discord_rpc.cpp
	mkdir -p objects
	clang++ -c discord-rpc/source/discord_rpc.cpp -o objects/discord_rpc.o $(joerpc_flags)

objects/dllmain.o: discord-rpc/source/dllmain.cpp
	mkdir -p objects
	clang++ -c discord-rpc/source/dllmain.cpp -o objects/dllmain.o $(joerpc_flags)