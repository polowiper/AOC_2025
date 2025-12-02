{
  description = "A basic C++ devshell for aoc";

  inputs.nixpkgs.url = "nixpkgs/nixpkgs-unstable";

  outputs = {
    self,
    nixpkgs,
    ...
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
    };
  in {
    devShells.${system}.default = pkgs.mkShell {
      packages = with pkgs; [
        libgcc
        cmake
        gnumake
        gdb
      ];
      shellHook = ''
        echo "C++ devShell"
      '';
    };
  };
}
