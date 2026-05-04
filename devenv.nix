{
  pkgs,
  ...
}:

{
  packages = with pkgs; [
    git
    bashInteractive
    cli11
    go-task
    liteparse
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
    emscripten
    emscriptenStdenv
    nodejs
    tailwindcss
    autoprefixer
    prettierd
    vite
  ];

  # https://devenv.sh/languages/
  languages = {
    cplusplus.enable = true;

    # python = {
    #   enable = true;
    #
    #   directory = "./tools";
    #
    #   venv.enable = true;
    #   uv = {
    #     enable = true;
    #     sync = {
    #       enable = true;
    #       allGroups = true;
    #     };
    #   };
    # };

    javascript = {
      enable = true;
    };

    typescript = {
      enable = true;
    };
  };
}
