#include "FS.h"
#include <filesystem>

namespace Orchis {

	std::unordered_map<std::string, std::string> FS::m_MountedPaths;

	std::ifstream FS::Open(const std::string& path, std::ios_base::openmode openMode)
	{
		int i = path.find('\\');
		if (i == path.npos)
			i = path.find('/');
		
		if (i != path.npos)
		{
			std::string vPath(path.substr(0, i));
			if (m_MountedPaths.contains(vPath))
			{
				std::string physPath = m_MountedPaths[vPath] + path.substr(i);
				std::ifstream file(physPath, openMode);
				return file;
			}
		}

		return std::ifstream(path, openMode);
	}

	void FS::Init()
	{
		std::filesystem::create_directories("C:\\Orchis\\Data\\Shaders");
		FS::Mount("C:\\Orchis\\Data\\Shaders", "Shaders");
	}

	void FS::Mount(const std::string& path, const std::string& vPath)
	{
		if (std::filesystem::exists(path))
		{
			FS::m_MountedPaths.insert({ vPath, path });
		}
	}

}
