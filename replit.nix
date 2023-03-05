{ pkgs }: {
	deps = [
  pkgs.python38Full
  pkgs.python38Packages.flask
  pkgs.sudo
  pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
    pkgs.cmake
	];
}