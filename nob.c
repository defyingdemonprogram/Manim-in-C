#define NOB_IMPLEMENTATION
#include "./src/nob.h"

#define BUILD_DIR "./build/"
#define SRC_DIR "./src"

void cc(Nob_Cmd *cmd) {
    nob_cmd_append(cmd, "cc");
    nob_cmd_append(cmd, "-Wall", "-Wextra", "-ggdb");
    nob_cmd_append(cmd, "-I./raylib/raylib-5.5_linux_amd64/include");
}

void libs(Nob_Cmd *cmd) {
	nob_cmd_append(cmd, "-Wl,-rpath=./raylib/raylib-5.5_linux_amd64/lib");
	nob_cmd_append(cmd, "-Wl,-rpath="BUILD_DIR);
	nob_cmd_append(cmd, "-L./raylib/raylib-5.5_linux_amd64/lib");
	nob_cmd_append(cmd, "-l:libraylib.so", "-lm", "-ldl", "-lpthread");
}

bool build_plug(Nob_Cmd *cmd, const char *source_path, const char *output_path) {
	cmd->count = 0;
	cc(cmd);
	nob_cmd_append(cmd, "-fPIC", "-shared");
	nob_cmd_append(cmd, "-o", output_path);
    nob_cmd_append(cmd, source_path, SRC_DIR"/tasks.c");
	libs(cmd);
	return nob_cmd_run_sync(*cmd);
}

bool build_panim(Nob_Cmd *cmd) {
	cmd->count = 0;
	cc(cmd);
	nob_cmd_append(cmd, "-o", BUILD_DIR"panim");
	nob_cmd_append(cmd, SRC_DIR"/panim.c", SRC_DIR"/ffmpeg_linux.c");
	libs(cmd);

	return nob_cmd_run_sync(*cmd);
}

int main(int argc, char **argv) {
	NOB_GO_REBUILD_URSELF(argc, argv);
	Nob_Cmd cmd = {};

	if (!nob_mkdir_if_not_exists(BUILD_DIR)) return 1;

	if (!build_plug(&cmd, SRC_DIR"/tm.c", BUILD_DIR"libtm.so")) return 1;
	if (!build_plug(&cmd, SRC_DIR"/template.c", BUILD_DIR"libtemplate.so")) return 1;
	if (!build_plug(&cmd, SRC_DIR"/squares.c", BUILD_DIR"libsquares.so")) return 1;
	if (!build_panim(&cmd)) return 1;

	// cmd.count = 0;
	// nob_cmd_append(&cmd, BUILD_DIR"panim", BUILD_DIR"libtm.so");
	// if (nob_cmd_run_sync(cmd)) return 1;
	return 0;
}
