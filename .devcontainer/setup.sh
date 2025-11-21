GLEW_DEPS="libxmu-dev libxi-dev libgl-dev"
GLFW_DEPS="libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev pkg-config"
UTILS="lf"
apt-get update
DEBIAN_FRONTEND=noninteractive apt-get -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" install -y build-essential curl gdb clang clangd clang-tools zip unzip tar git cmake ${GLEW_DEPS} ${GLFW_DEPS} ${VULKAN_DEPS} ${VULKAN_SDK} ${UTILS}
# vcpkg install --x-install-root="libs"
export VCPKG_ROOT=$(whereis -b vcpkg | awk '{print $2}')
