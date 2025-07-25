{
  description = "A basic flake with a shell";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  inputs.systems.url = "github:nix-systems/default";
  inputs.flake-utils = {
    url = "github:numtide/flake-utils";
    inputs.systems.follows = "systems";
  };

  outputs =
    { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            bashInteractive
            cli11
            go-task
            catch2_3
            clang
            clang-tools
            cmake
            gnuplot
            doxygen
            gcc
            ghostscript
            gdb
            lcov
            lldb
            help2man
            llvmPackages.openmp
            valgrind
            graphviz
          ];
        };
      }
    );
}
