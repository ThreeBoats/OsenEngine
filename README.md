# OsenEngine

OsenEngine is a game engine I am writing to get better at programming.

## Goals

Note: I will often update this paragraph to reflect my newest discoveries.

- I want to get a triangle on screen as soon as possible, following good architecture. I want to use Vulkan.
- I also need an event system. But I find that difficult to build, and for now I do not need it.

## “Features”

As of now, the engine cannot do much. But I hope that someday it will be great—or that, in a few years, I will stop programming and abandon this project.

OsenEngine already has a few “features,” such as:

- an entry point
- a main loop in the engine
- a simple logging system
- a window that prints out details like the position of your mouse
- a simple Doxyfile, which I have not changed much from the default. You can use it to learn about the engine internals.

## Technical details

- OsenEngine is written in C++, using CMake as a build system.
- I am currently using some MSVC-specific options in CMake. To be precise, I am using `/Zi` as a compiler option. This will eventually be changed.
- The engine is added to the game as a static library. I do not know yet if this will change.

## Building

Requirements:

1. A C++ compiler. I think only MSVC works, but I am not sure, because of the CMake file.
2. A Slang compiler.
3. CMake.
4. The Vulkan SDK and a compatible GPU with a driver. You can download it from the [official download page](https://vulkan.lunarg.com/sdk/home).

1. First, clone the project and its submodules with: `git clone https://github.com/ThreeBoats/OsenEngine.git --recursive`
2. Second, move into the engine directory: `cd OsenEngine`
3. Build or generate the project using CMake.
4. Compile the Slang shader. It is in `OsenEngine/source/OsenEngine/Platform/Vulkan/shaders-tmp`. There is a `compile-shader.bat` file for Windows users. Copy the `slang.spv` file it produces to the working directory. Run the project once, it will print out the working directory. (and crash because it could not find the `slang.spv` file, but you are about to fix that;)).

## License

OsenEngine does not currently have a license, because I am still discovering all the options available. Until I add a license, you cannot freely use or redistribute the code, although in its current state it probably is not very useful.

## Some other things

- English is not my native language, so I hope it is understandable.
- I am always open to feedback.

I hope that some people will get useful information out of this project. Have a nice day!