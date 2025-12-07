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
    #I tried to fix the lsp errors but ehh didn't work
    devShells.${system}.default = pkgs.mkShell {
      packages = with pkgs; [
        libgcc
        cmake
        gnumake
        gdb
        clang
        clang-tools
      ];
      shellHook = ''
        echo "C++ devShell"
      '';
    };
  };
}
