#pragma once
#include <string_view>
#include "../Src/Core/Macros.h"

namespace Orchis {

	class OC_API OutputBase
	{
	public:

		virtual ~OutputBase() = default;
	protected:
		friend class Logger;
		virtual void WriteLineImpl(std::string_view msg) const = 0;
		virtual void WriteImpl(std::string_view msg) const = 0;
		OutputBase() = default;
		OutputBase(const OutputBase&) = default;
		OutputBase(OutputBase&&) = default;
	};

}