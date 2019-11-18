#include <Python.h>

#include <stdio.h>
#include <fcntl.h>

#include <emscripten.h>

#include <libtar.h>

int main(int argc, char** argv) {
	TAR* tar;
	if (tar_open(&tar, "./media/pydata.tar", NULL, O_RDONLY, 0, 0) != 0) {
		fprintf(stderr, "Error: failed to open pydata.tar\n");
		exit(1);
	}
	if (tar_extract_all(tar, (char*) "/") != 0) {
		fprintf(stderr, "Error: failed to extract pydata.tar\n");
		exit(1);
	}
	tar_close(tar);


	setenv("PYTHONHOME", "/", 0);

	Py_InitializeEx(0);
	PyRun_SimpleString("print \"Hello from Python\"");
	Py_Finalize();

	emscripten_exit_with_live_runtime();
	return 0;
}

