#pragma once
#include "common.h"

#include "prelude.h"
namespace cultlang{
namespace uv
{
	class Prepare
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::Prepare)
	private:
		std::unique_ptr<uv_prepare_t> _prepare;
		
	public:
		craft::types::instance<craft::lisp::PSubroutine> _p;

		CULTLANG_UV_EXPORTED Prepare(craft::types::instance<Loop> l, craft::types::instance<craft::lisp::PSubroutine> p);
		CULTLANG_UV_EXPORTED Prepare(const Prepare& other) = delete;
		CULTLANG_UV_EXPORTED Prepare(Prepare&& other) noexcept;
		CULTLANG_UV_EXPORTED ~Prepare() noexcept;
		CULTLANG_UV_EXPORTED Prepare& operator= (const Prepare& other) = delete;
		CULTLANG_UV_EXPORTED Prepare& operator= (Prepare&& other) noexcept;


		CULTLANG_UV_EXPORTED void stop();
	};
}}