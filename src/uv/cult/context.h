#pragma once
#include "uv/common.h"

namespace cultlang {
namespace uv {

	class PromisePair
		: public virtual craft::types::Object
	{
	private:
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::PromisePair);
	public:
		craft::instance<craft::lisp::PSubroutine> fail, success;

		PromisePair(craft::instance<craft::lisp::PSubroutine> fail, craft::instance<craft::lisp::PSubroutine> success);
	};

	class FileRequestContext
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::FileRequestContext);
	public:
		size_t read;
		craft::instance<> ctx;

		
		craft::instance<craft::lisp::PSubroutine> onchmod;
		craft::instance<craft::lisp::PSubroutine> onerr;
		craft::instance<craft::lisp::PSubroutine> onopen;
		craft::instance<craft::lisp::PSubroutine> onclose;
		craft::instance<craft::lisp::PSubroutine> onread;
		craft::instance<craft::lisp::PSubroutine> onwrite;
		craft::instance<craft::lisp::PSubroutine> onsend;
		craft::instance<craft::lisp::PSubroutine> onstat;
		craft::instance<craft::lisp::PSubroutine> onfstat;
		craft::instance<craft::lisp::PSubroutine> onlstat;
		craft::instance<craft::lisp::PSubroutine> onsync;
		craft::instance<craft::lisp::PSubroutine> ontruncate;
		

		CULTLANG_UV_EXPORTED FileRequestContext();

	};


	class StreamContext
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::StreamContext);
	public:
		size_t read;
		craft::instance<> ctx;

		craft::instance<craft::lisp::PSubroutine> onerr;
		craft::instance<craft::lisp::PSubroutine> onclose;
		craft::instance<craft::lisp::PSubroutine> onconnect;
		craft::instance<craft::lisp::PSubroutine> onend;
		craft::instance<craft::lisp::PSubroutine> onlisten;
		craft::instance<craft::lisp::PSubroutine> onshutdown;
		craft::instance<craft::lisp::PSubroutine> onwrite;
		craft::instance<craft::lisp::PSubroutine> ondata;

		CULTLANG_UV_EXPORTED StreamContext();

	};

	CULTLANG_UV_EXPORTED void make_tcp_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_pipe_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_fs_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_tty_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_process_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_stream_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_util_bindings(craft::types::instance<craft::lisp::Module> m);
}}