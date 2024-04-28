#include "Macros.h"
#include "VFS/FS.h"


OC_EXPORT void OrchisFileSystemMount(const char* path, const char* vPath)
{
	Orchis::FS::Mount(path, vPath);
}