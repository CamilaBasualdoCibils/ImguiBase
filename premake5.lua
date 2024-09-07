local PROJECT_NAME = "ImguiBase"

-- premake5.lua
workspace (PROJECT_NAME)
    configurations { "Debug", "Release" }

project (PROJECT_NAME)
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   flags{"MultiProcessorCompile"}

   files {"src/*.cpp" }
   --files { }
   includedirs {"lib/imgui-docking","lib/glfw/include","lib/glew/include","lib/implot"}
   libdirs{"lib/glfw/lib","lib/glew/lib"}
    links{"ImGui","ImPlot","GLEW","glfw3","X11","GL"}
   filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"
   filter "configurations:Release"
      optimize "On"


project "ImGui"
   kind "StaticLib"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   flags{"MultiProcessorCompile"}

   files { "lib/imgui-docking/*.h","lib/imgui-docking/*.cpp",
    "lib/imgui-docking/backends/imgui_impl_opengl3.cpp",
    "lib/imgui-docking/backends/imgui_impl_glfw.cpp"  }
   includedirs {"lib/imgui-docking"}

   filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"
   filter "configurations:Release"
      optimize "On"

project "ImPlot"
    kind "StaticLib"
    language "C++"
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
    os.execute("git clone https://github.com/glfw/glfw lib/glfw") --git clone
    os.execute("cd lib/glfw && cmake -S . -B build -D  GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF && cd build && make ")--move to the folder,create makefiles and make
    os.mkdir("lib/glfw/lib")
    os.copyfile("lib/glfw/build/src/libglfw3.a","lib/glfw/lib")
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

    --GLEW

    local glewFileName = "glew-2.2.0"
    local glewFileUrl = string.format("https://github.com/nigels-com/glew/releases/download/%s/%s%s",glewFileName,glewFileName,".zip")
    local glewGetCmd = string.format("wget -O GLEW.zip %s",glewFileUrl)--get zipfile, git doesnt have source files for some reason
    http.download(glewFileUrl,"glew.zip")
    --os.execute(glewGetCmd)
    zip.extract("glew.zip","lib")
    --os.execute("unzip glew.zip -d lib")--unzip zip file
    os.copyfile(string.format("mv lib/%s ",glewFileName),"lib/glew")
    os.execute(string.format("mv lib/%s lib/glew",glewFileName))--rename from GLFW_X.X.X to glfw TODO: FIND EQUIVALENT OS. OR SOMETHING
    os.execute("cd lib/glew && make glew.lib.static")--make static libraries
    os.execute("cd lib/glew && rm -f -r auto bin build config doc src tmp")--remove anything not needed
    os.execute("rm -f glew.zip")--delete original zip file
    os.chdir("lib/glew")
    RemoveAllFilesThatMatch("*")
    os.chdir("../../")
  
   --[[
    FOR WINDOWS/MAC SUPPORT USES os.target() WHICH TELLS WHICH OS THIS IS
   ]]


end
function customClean()
    -- Specify the directories or files to be cleaned
    local dirsToRemove = {
        "lib",
        "bin","obj"
    }

    local filesToRemove = {
        "Makefile",
        "myproject.sln",
    }
    
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