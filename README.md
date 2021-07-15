1. 	From inside the build folder, run cmake ../
2. 	Open BaseCode.sln with Visual Studio
3. 	In Solution Explorer, right lick on BaseCode, open Properties -> Configuration Manager -> 
	Active solution platform -> New, Select platform x64 -> OK -> close
	On the properties panel, open Linker-> Command Line, and remove the Additional options text 
	about the x86 machine
4. 	Build BaseCode (Right click -> Set as startup project -> F5)
5. 	Copy glew32.dll from the glew-2.1.0 folder, freeglut.dll from the freeglut folder, freetype.dll
	from the freetype folder, 64 bit versions and paste inside the build folder
6. 	Copy shaders, fonts and models inside the build folder
