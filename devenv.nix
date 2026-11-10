{
  config,
  lib,
  pkgs,
  ...
}:

{
  # The npm workerd executable needs the Nix dynamic linker on Linux.
  env = lib.optionalAttrs pkgs.stdenv.isLinux {
    MINIFLARE_WORKERD_PATH = "${pkgs.writeShellScript "qualpal-workerd" ''
      cd "${config.devenv.root}/web"
      binary=$(node -e 'const {createRequire}=require("node:module"); const wrangler=createRequire(require.resolve("wrangler/package.json")); const miniflare=createRequire(wrangler.resolve("miniflare/package.json")); process.stdout.write(miniflare("workerd").default);')
      exec "${pkgs.stdenv.cc.bintools.dynamicLinker}" "$binary" "$@"
    ''}";
  };

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
    python3
    tailwindcss
    autoprefixer
    prettierd
    vite
  ];

  # https://devenv.sh/languages/
  languages = {
    cplusplus.enable = true;

    javascript = {
      enable = true;
    };

    typescript = {
      enable = true;
    };
  };
}
