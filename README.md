# J.O.E -- Discord Rich Presence
This project adds Discord rich presence support to the J.O.E text editor. This started as a personal project, and the code was too poor to publish, however I revisited it after a while and heavily improved the code, and its features. For those of you who don't know, [JOE is a pretty underrated text editor](https://joe-editor.sourceforge.io/), and is one that I use all the time for basic text editing. It's a pretty good Emacs substitute. 

## Usage 
When built, two executables are created. `joel.exe` _(joe launcher)_ and `joerpc.exe`, both of which should be moved to the same directory as the `joe.exe` executable. The former executable simply launches `joe.exe` together with `joerpc.exe` assuming an instance isn't already running. The latter is the actual program that monitors for any running instances of `joe.exe` and scans the active file, and updates Discord with a rich presence matching the file. You can run `joerpc.exe` once separately, as it remains open and will continue monitoring for changes. 

Arguments passed to `joel.exe` are passed to `joe.exe` however there are some cases where doing so can cause a segfault for unknown reasons. For instance: `joel ..\.gitignore` will cause it to segfault. 

`joel.exe` Is simply a convenience, so that when you know you have no running instance of `joerpc.exe` but you want to open a new Joe buffer, you can do both at once by opening said buffer via `joel.exe` unstead of `joe.exe`. 

## Building
By default, only `joerpc.exe` is built when running make. To build `joel.exe` either do `make all` or `make builds/joel.exe` to built both/it. That's it! The binaries provided are built with Clang on Windows, how other compilers react to this code is unknown to me. You may also have to edit the makefile if you're integrating it into a different project structure.

## How It Works
I thought I should summarize how `joerpc.exe` gets the information that it does from `joe.exe` which is by reading its process memory. A while ago when reverse engineering `joe.exe` I found that the module address of `joewin.dll` + `0xCBA08` = The start address of a c-string containing the active file. I then used the Windows API to offset the address one byte at a time to read the string. The code is under `joerpc.cxx` if you want more details.

## Dependancies
Bundled with the repository comes two (MIT open source) libraries necessary for communicating with Discord, one of which is actually a dependancy of the other library. You can find the respective libraries here if you wish. 

* [Discord-RPC](https://github.com/discordapp/discord-rpc)
* [Rapid-JSON](https://github.com/Tencent/rapidjson)

## Preview
![](https://i.imgur.com/EAWZ6Va.png)
