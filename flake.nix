{
  description = "ComputerImaging";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in {
        devShells.default = pkgs.mkShell {
          name = "graphics-dev-shell";

          # Tools needed for C++ + CMake
          buildInputs = with pkgs; [
            cmake
            gcc
            gnumake
            pkg-config
            vscode
            # GL
            glfw
            glew
            mesa
            mesa.drivers
            libGL
            libGLU
            freeglut

            # DirectX headers (mingw)
            # mingw_w64_headers
            # mingw_w64_winpthreads
            # mingw_w64_gcc

            # Optional: DXVK runtime DLLs (if running through Wine)
            # dxvk

            # Optional build tools
            ninja
            clang
            clang-tools  # clangd for VSCode
          ];

          # Ensures CMake finds Vulkan + OpenGL
          CMAKE_PREFIX_PATH = pkgs.lib.makeSearchPath "lib/cmake" [
            pkgs.vulkan-loader
            pkgs.vulkan-headers
            pkgs.vulkan-validation-layers
            pkgs.glfw
            pkgs.glew
          ];

          # Vulkan SDK env vars
          VULKAN_SDK = "${pkgs.vulkan-headers}";

          # Helps CMake find DirectX via MinGW
          #PKG_CONFIG_PATH = pkgs.lib.makeSearchPath "lib/pkgconfig" [
          #  #pkgs.mingw_w64_headers
          #];

          shellHook = ''
            echo "🌋 Graphics development shell loaded!"
            echo " - OpenGL, Vulkan, DirectX available"
            echo " - clangd ready for VSCode"
            echo " - Compile commands: cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
          '';
        };

        # Optional: actual nix build of your project (not required)
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "myproject";
          version = "1.0";

          src = ./.;

          nativeBuildInputs = [ pkgs.cmake pkgs.pkg-config ];
          buildInputs = with pkgs; [
            glfw
            glew
            libGL
            vulkan-loader
            vulkan-headers
          ];

          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
          ];
        };
      });
}
