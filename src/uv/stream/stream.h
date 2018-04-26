#pragma once
#include "uv/common.h"

#include "uv/prelude.h"


CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uv_stream_t);
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uv_connect_t);
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uv_shutdown_t);
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uv_write_t);

namespace cultlang{
namespace uv
{
	class StreamHandler
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::StreamHandler)
	private:
		typedef craft::types::instance<craft::lisp::Function> t_function;
	public:
		t_function _read, _write, _connect, _shutdown, _connection, _close;

		CULTLANG_UV_EXPORTED StreamHandler() = default;
	
		inline t_function getRead() { return _read; };
		inline void setRead(t_function f) { _read = f; };

		inline t_function getWrite() { return _write; };
		inline void setWrite(t_function f) { _write = f; };

		inline t_function getConnect() { return _connect; };
		inline void setConnect(t_function f) { _connect = f; };
		
		inline t_function getShutdown() { return _shutdown; };
		inline void setShutdown(t_function f) { _shutdown = f; };

		inline t_function getConnection() { return _connection; };
		inline void setConnection(t_function f) { _connection = f; };

		inline t_function getClose() { return _close; };
		inline void setClose(t_function f) { _close = f; };
	};


	void make_stream_ops(craft::types::instance<craft::lisp::Module> ret);
}}