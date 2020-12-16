// https://github.com/mono/mono/blob/master/samples/embed/teste.c

#include <mono/jit/jit.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h> 
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/mono-gc.h>
// #include <mono/utils/mono-publib.h>
#include <stdlib.h>
#include <direct.h>

// https://docs.microsoft.com/ko-kr/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

static MonoString* cppFunction() 
{
	return mono_string_new(mono_domain_get(), "C++ MonoString");
}

static int malloc_count = 0;

static void* custom_malloc(size_t bytes)
{
	++malloc_count;
	return malloc(bytes);
}

int
main(int argc, char* argv[]) 
{
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int retval = 0;

	long long* a = malloc(sizeof(long long)); // <- FORCE LEAK

	const char *file = "..\\..\\csharp\\bin\\Debug\\csharp.dll";

	char lib[1024];
	getcwd(lib, sizeof(lib));

	char etc[1024];
	strcpy(etc, lib);

	strcat(lib, "\\..\\..\\mono\\lib");
	strcat(etc, "\\..\\..\\mono\\etc");

	mono_set_dirs(lib, etc);
	mono_debug_init(MONO_DEBUG_FORMAT_MONO);

	// ..\..\dll\dll\bin\Debug\dll.dll

	MonoDomain *domain = mono_jit_init("sample");

#define MONO_NAMESPACE "Lofle"
#define MONO_CLASS "MonoClass"

	mono_add_internal_call(MONO_NAMESPACE "." MONO_CLASS "::cppFunction", cppFunction);

	mono_config_parse(NULL);

	MonoAssembly *assembly = mono_domain_assembly_open(domain, file);
	MonoImage* image = mono_assembly_get_image(assembly);

	mono_gc_collect(mono_gc_max_generation());

	MonoClass* monoClass = mono_class_from_name(image, MONO_NAMESPACE, MONO_CLASS);
	MonoMethod* monoMethod = mono_class_get_method_from_name(monoClass, "MonoMethod", 1);

	void* args[1];
	args[0] = mono_string_new(domain, "[From C++, MonoString Parameter]");
	
	mono_runtime_invoke(monoMethod, monoClass, args, 0);

	mono_jit_cleanup(domain);

	
	_CrtDumpMemoryLeaks();

	return retval;
}

