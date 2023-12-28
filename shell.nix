{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
	buildInputs = [
		pkgs.vulkan-tools
		pkgs.vulkan-headers
		pkgs.vulkan-loader
		pkgs.vulkan-validation-layers
		pkgs.glfw
		pkgs.shaderc
		pkgs.pkg-config
		pkgs.xorg.libX11
		pkgs.xorg.libXau
		pkgs.xorg.libXdmcp
		pkgs.pkg-config
		pkgs.glm
	];

  shellHook =
	''
	    export VK_LAYER_PATH="${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d"
#	    export Vulkan_INCLUDE_DIR="${pkgs.vulkan-headers}/include"
#	    export Vulkan_LIBRARY="${pkgs.vulkan-loader}/lib"
	'';
}
