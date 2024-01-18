{
  description = "Little 3D Vulkan C++ Engine Flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable"; 
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... } @ inputs:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            vulkan-tools
            vulkan-headers
            vulkan-loader
            vulkan-validation-layers
            glfw
            shaderc
            pkg-config
            xorg.libX11
            xorg.libXau
            xorg.libXdmcp
            glm
            boost
            ninja
          ];
          shellHook = ''
            export VK_LAYER_PATH="${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d"
            export Vulkan_INCLUDE_DIR="${pkgs.vulkan-headers}/include"
            export Vulkan_LIBRARY="${pkgs.vulkan-loader}/lib"
            if [ -z "$CLION_AUTO_LAUNCHED" ]; then
              export CLION_AUTO_LAUNCHED=1
              nohup clion . > /dev/null 2>&1 &
              echo "CLion launched in background."
            fi
            BUILD_DIR="cmake-build-debug"
            sleep 5
            if [ -d "$BUILD_DIR" ]; then
              echo "Clearing existing build directory: $BUILD_DIR"
              rm -rf "$BUILD_DIR"
            fi
            mkdir -p "$BUILD_DIR"
            cd "$BUILD_DIR"
            cmake .. -DCMAKE_BUILD_TYPE=Debug -G Ninja  # Use Ninja for generating build files
            ninja  # Replace make with ninja for building the project
            cd ..
            echo "CMake project reconfigured."
          '';
        };
      }
    );
}
