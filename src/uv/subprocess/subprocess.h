#pragma once
#include "uv/common.h"

#include "lisp/library/system/prelude.h"
#include "uv/prelude.h"

#include "uv/buffer.h"

namespace cultlang {
namespace uv
{
	class SubprocessHandle
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::SubprocessHandle)
	public:
		std::unique_ptr<uv_process_t> _handle;

	public:
		CULTLANG_UV_EXPORTED SubprocessHandle();
		CULTLANG_UV_EXPORTED SubprocessHandle(const Check& other) = delete;
		CULTLANG_UV_EXPORTED SubprocessHandle(SubprocessHandle&& other) noexcept;
		CULTLANG_UV_EXPORTED ~SubprocessHandle() noexcept;
		CULTLANG_UV_EXPORTED SubprocessHandle& operator= (const SubprocessHandle& other) = delete;
		CULTLANG_UV_EXPORTED SubprocessHandle& operator= (SubprocessHandle&& other) noexcept;

	};

	class Subprocess
		: public uv_process_options_s
		, public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::Subprocess)
	private:
		typedef craft::types::instance<std::string> i_str;

		i_str _command;
		std::vector<i_str> _args;
		//i_str _cwd;
		craft::types::instance<> _function;
	public:
		craft::types::instance<SubprocessHandle> _handle;

		CULTLANG_UV_EXPORTED Subprocess(i_str command, std::vector<i_str> args,craft::types::instance<> function);
		~Subprocess();
	};

	CULTLANG_UV_EXPORTED void make_subprocess_ops(craft::types::instance<craft::lisp::Module> ret);

}}