{
  description = "Little 3D Vulkan C++ Engine . . .";

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
          ];

          shellHook = ''
            export VK_LAYER_PATH="${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d"
            # Launching CLion and then closing the terminal
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
