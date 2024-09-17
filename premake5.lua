local PROJECT_NAME = "ImguiBase"

-- premake5.lua
workspace "MyWorkspace"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    architecture "x86_64"
    

project (PROJECT_NAME)
   kind "StaticLib"
   language "C++"
   cppdialect "c++17"
   targetdir "bin/%{cfg.buildcfg}"
   flags{"MultiProcessorCompile"}

   files {"src/**.cpp","lib/imgui-docking/backends/imgui_impl_glfw.cpp","lib/imgui-docking/backends/imgui_impl_opengl3.cpp" }
   --files { }
   includedirs {"lib/imgui-docking","lib/glfw/include","lib/glew/include","lib/implot","lib/glm"}
   libdirs{"lib/glfw/lib","lib/glew/lib"}
    links{"ImGui","ImPlot","glfw3","glew32s"}

    filter "action:gmake2"
        links{"X11","GL"}
    filter "action:vs2022" 
        links{"opengl32"}
   filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"
   filter "configurations:Release"
      optimize "On"


project "ImGui"
   kind "StaticLib"
   language "C++"
   cppdialect "c++17"
   targetdir "bin/%{cfg.buildcfg}"
   flags{"MultiProcessorCompile"}

   files { "lib/imgui-docking/*.h","lib/imgui-docking/*.cpp",
    "lib/imgui-docking/backends/imgui_impl_opengl3.cpp",
   "lib/imgui-docking/backends/imgui_impl_glfw.cpp"  }
   includedirs {"lib/imgui-docking","lib/glfw/include","lib/glew/include"}

   filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"
   filter "configurations:Release"
      optimize "On"

project "ImPlot"
    kind "StaticLib"
    language "C++"
    cppdialect "c++17"
    targetdir "bin/%{cfg.buildcfg}"
    flags{"MultiProcessorCompile"}

    files { "lib/implot/*.h","lib/implot/*.cpp"  }
    includedirs {"lib/implot","lib/imgui-docking"}

    filter "configurations:Debug"
       defines { "_DEBUG" }
       symbols "On"
    filter "configurations:Release"
       optimize "On"


function setup()
    

    
    --GLFW
    --git clone
    os.execute("git clone https://github.com/glfw/glfw lib/glfw") 
    --cd to the folder,create makefiles
    os.execute("cd lib/glfw && cmake -S . -B build -D  GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF")
    
    local glfwBuildCommand 
    local glfwLibFileName
    local glfwLibFileDir
    local OSMoveCommand

    if os.target() == "windows" then
        
        print("Compiling for windows")
        glfwLibFileName = "glfw3.lib"
        glfwLibFileDir = ".\\lib\\glfw\\build\\src\\Release\\"
        glfwBuildCommand = "msbuild GLFW.sln /t:Build /p:Configuration=Release"
        OSMoveCommand = "move"
    else
          glfwLibFileName = "libglfw3.a"
          glfwBuildCommand = "make"
          glfwLibFileDir = "lib/glfw/build/src/"
          OSMoveCommand = "mv"
    end
    --if windows then compile using msbuild, otherwise use make
    os.execute(string.format("cd lib/glfw/build && %s",glfwBuildCommand))
    os.mkdir("lib/glfw/lib")
    
    glfwLibFileFUllDir = string.format("%s%s",glfwLibFileDir,glfwLibFileName)
    glfwFullMoveCommand = string.format("%s %s lib/glfw/lib",OSMoveCommand,glfwLibFileFUllDir)
    --move lib file to the lib/glfw/lib folder
    os.execute(glfwFullMoveCommand)
    --delete everything we dont need
    os.chdir("lib/glfw")
    os.rmdir(".github")
    os.rmdir("build")
    os.rmdir("CMake")
    os.rmdir("deps")
    os.rmdir("docs")
    os.rmdir("examples")
    os.rmdir("tests")
    os.rmdir("src")
    RemoveAllFilesThatMatch("*")
    os.chdir("../../")

    
    --IMGUI
    os.execute("git clone https://github.com/ocornut/imgui --branch docking lib/imgui-docking")
    --IMPLOT
    os.execute("git clone https://github.com/epezent/implot lib/implot")
    --GLM
    os.execute("git clone https://github.com/g-truc/glm lib/glm")
    --GLEW
     local glewFileName = "glew-2.2.0"
    local winFileName = string.format("%s-win32",glewFileName)
    local glewFileUrl

    --address to corresponding file
    glewFileUrl = string.format("https://github.com/nigels-com/glew/releases/download/%s/%s%s"
    ,glewFileName,(os.target() == "windows") and winFileName or glewFileName,".zip")

    --download file
    http.download(glewFileUrl,"glew.zip")
    --unzip
    zip.extract("glew.zip","lib")

    if os.target() == "windows" then
        --rename from glew-X-X -> glew
        os.execute(string.format("ren .\\lib\\%s glew",glewFileName))
        --window's glew comes precompiled. so just move the static library
        os.execute("move .\\lib\\glew\\lib\\Release\\x64\\glew32s.lib .\\lib\\glew\\lib")
        --remove original directory
        os.rmdir(".\\lib\\glew\\lib\\Release")
    else
        --rename from glew-X-X -> glew
        os.execute(string.format("mv lib/%s lib/glew",glewFileName))
        --compile the static libraries
        os.execute("cd lib/glew && make glew.lib.static")
        -- move the library to lib/glew/lib
        os.execute("mv lib/glew/lib/libGLEW.a lib/glew/lib/libglew32s.a")
        --delete unnecesarry folders
        os.execute("cd lib/glew && rm -f -r auto bin build config doc src tmp")--remove anything not needed
        --delete original zip file
        
        --go inside and delete every file. the files we need are at include and lib
        os.chdir("lib/glew")
        RemoveAllFilesThatMatch("*")
        os.chdir("../../")
    end
    --delete the zip file
    os.remove("glew.zip")



end
function customClean()
    -- Specify the directories or files to be cleaned
    local dirsToRemove = {
        "lib",
        "bin","obj",
        ".vs"
    }

    local filesToRemove = {
        "Makefile",
        "imgui.ini",
    }
    RemoveAllFilesThatMatch("make")
    RemoveAllFilesThatMatch("sln")
    RemoveAllFilesThatMatch("vcxproj")
    RemoveAllFilesThatMatch("filters")
    -- Remove specified directories
    for _, dir in ipairs(dirsToRemove) do
        if os.isdir(dir) then
            os.rmdir(dir)
            print("Removed directory: " .. dir)
        end
    end

    -- Remove specified files
    for _, file in ipairs(filesToRemove) do
        if os.isfile(file) then
            os.remove(file)
            print("Removed file: " .. file)
        end
    end
end
function RemoveAllFilesThatMatch(extension)

    local files = os.matchfiles(string.format("*.%s",extension))
    for _, file in ipairs(files) do
        print(string.format("Deleting %s",file))
        os.remove(file)
    end
end
-- Add the custom clean function to the clean action
newaction {
    trigger = "clean",
    description = "Custom clean action",
    execute = function()
        customClean()
    end
}
newaction {
   trigger = "setup",
   description = "Setup required libraries",
   execute = function()
       setup()
   end
}