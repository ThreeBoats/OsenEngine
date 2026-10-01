# Osen Engine

OsenEngine is a game engine I am writing to get better at programming.

## Goals
note: I will often update this paragraph, to reflect my newest discoveries.

- first priority is a sort of event system
- after that i want to start with rendering. I think I will use Vulkan as an API (I will use Vulkan, I decided), 
but abstracted at a high level. So i can switch to DX12 or metal or something else. 

## "features"
As of now the engine cannot do much. But I hope that sometime, it will be great (or in a few years I stop programming, and this project is abandoned). 
OsenEngine already has a few "features", such as: 
- an entry point
- a main loop in the engine
- a simple logging system
- a window that prints out details like the position of your mouse.
- a simple Doxyfile I did not change much from normal. You can use this to learn about the engine internals.

## technical details
- OsenEngine is written in C++, using CMake as build system.
- In CMake I am currently using some MSVC specific things. To be precise /Zi as compiler option. This will eventually be changed.
- The engine gets "added" to the game as a static library. I do not know yet if this will change.

## building

requirements
1. a C++ compiler, I think only MSVC works, not sure though. (because of CMake-file)
2. CMake

1. first clone the project and sub-modules with: git clone https://github.com/ThreeBoats/OsenEngine.git --recursive
2. second move into the engine directory: cd OsenEngine
3. build or generate the project using CMake.  (use your own favourite way, I have not much experience. Personally I open the folder with the root CMake file in VS22, and then compile from there)

## license
OsenEngine does currently not have an license, because I am still discovering all the options available.
Until I add a license, you cannot freely use, or redistribute the code. Although in this state it probably isn't even useful.

## some other things
- English is not my native language, so I hope it is somewhat understandable.
- I am always open to feedback
I hope that some people will get some useful information out of this project. Have a nice day!
