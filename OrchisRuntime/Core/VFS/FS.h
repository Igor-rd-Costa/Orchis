#pragma once
#include "Macros.h"
#include <unordered_map>
#include <string>


namespace Orchis {

	class OC_API FS {
	public:
		FS() = delete;
		FS(const FS&) = delete;
		FS(FS&&) = delete;

		static std::ifstream Open(const std::string& path, std::ios_base::openmode openMode);

		static std::string GetPath(const std::string& vPath);
		static void Init();
		static void Mount(const std::string& path, const std::string& vPath);
	private:
		static std::unordered_map<std::string, std::string> m_MountedPaths;



	};

}