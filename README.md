# Description
A bare bones DearImgui/ImPlot implementation using GLFW + GLEW. Intended to be cross platform with fallbacks for different openGL versions.

--- 
# Usage
```
/create project files, examples: 
// vs2022 for visual studio 2022
// gmake2 for makefiles
//use ./premake.exe on windows

./premake5 [target] 

//Download, setup and compile all required libraries aka GLFW,ImGui etc
./premake5 setup 

//remove all non essential assets
./premake5 clean
```
