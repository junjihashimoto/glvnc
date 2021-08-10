{
  description = "VNC client of opengl-rendering";
  
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };
  
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        customOverrides = self: super: {
          # Overrides go here
        };

        app = pkgs.stdenv.mkDerivation {
          name = "glvnc";
          src = ./.;
          nativeBuildInputs = with pkgs; [
            gnumake
          ];
          buildInputs = with pkgs; [
            xorg.libX11
            xorg.libxcb
            xorg.libXau
            xorg.libXdmcp
            xorg.libXext
            xorg.libXi
            xorg.libXxf86vm
            xorg.libXrandr
            libGL
            libGLU
            zlib
            giflib
            libjpeg
            libpng
          ];
          buildPhase = ''
            cp Makefile.linux Makefile.common
            pushd libs
            make
            popd
            make
          '';
          makeFlags = [
            "DESTDIR=$(out)"
          ];
        };
        packageName = "glvnc";
      in {
        packages.${packageName} = app;

        defaultPackage = self.packages.${system}.${packageName};

        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [  ];
          inputsFrom = builtins.attrValues self.packages.${system};
        };
      }
    );
}
