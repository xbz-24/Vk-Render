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
        devShell = pkgs.mkShell {
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
          ];

          shellHook = ''
            export VK_LAYER_PATH="${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d"
            export Vulkan_INCLUDE_DIR="${pkgs.vulkan-headers}/include"
            export Vulkan_LIBRARY="${pkgs.vulkan-loader}/lib"
            if [ -z "$CLION_AUTO_LAUNCHED" ]; then
              export CLION_AUTO_LAUNCHED=1
              clion . &
              exit
            fi
          '';
        };
      }
    );
}
