#pragma once
#include "uv/common.h"

#include "uv/prelude.h"

#include "stream.h"
namespace cultlang
{
namespace uv
{
	class Tcp
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::Tcp)
	private:
		
	public:
		std::unique_ptr<uv_tcp_t> _tcp;
		craft::types::instance<StreamHandler> _p;

		CULTLANG_UV_EXPORTED Tcp(craft::types::instance<Loop> l, craft::types::instance<StreamHandler> p);
		CULTLANG_UV_EXPORTED Tcp(const Tcp& other) = delete;
		CULTLANG_UV_EXPORTED Tcp(Tcp&& other) noexcept;
		CULTLANG_UV_EXPORTED ~Tcp() noexcept;
		CULTLANG_UV_EXPORTED Tcp& operator= (const Tcp& other) = delete;
		CULTLANG_UV_EXPORTED Tcp& operator= (Tcp&& other) noexcept;


		// Control stuff
		CULTLANG_UV_EXPORTED void nodelay(bool yes);
		CULTLANG_UV_EXPORTED void keepalive(bool yes, int64_t delay);
		CULTLANG_UV_EXPORTED void simultaneous_accepts(bool yes);

		// Interesting stuff
		CULTLANG_UV_EXPORTED void bind(uint16_t port);
		CULTLANG_UV_EXPORTED craft::types::instance<uv_connect_t> connect(std::string address, int16_t port);
	};

	void make_tcp_ops(craft::types::instance<craft::lisp::Module> ret);
}}