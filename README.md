# Description
A bare bones DearImgui/ImPlot implementation using GLFW + GLEW for use. Intended to be cross platform with fallbacks for different openGL versions.

--- 
# Usage
```
/create project files, examples: vs2022, gmake2
//see all available targets at https://premake.github.io/docs/Using-Premake/
//use ./premake.exe on windows

./premake5 [target] 


//Download, setup and compile all required libraries aka GLFW,ImGui etc
./premake5 setup 

//remove all non essential assets
./premake5 clean
```
