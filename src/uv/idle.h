#pragma once
#include "common.h"

#include "prelude.h"
namespace cultlang{
namespace uv
{
	class Idle
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::Idle)
	private:
		std::unique_ptr<uv_idle_t> _idle;
		
	public:
		craft::types::instance<craft::lisp::PSubroutine> _p;

		CULTLANG_UV_EXPORTED Idle(craft::types::instance<Loop> l, craft::types::instance<craft::lisp::PSubroutine> p);
		CULTLANG_UV_EXPORTED Idle(const Idle& other) = delete;
		CULTLANG_UV_EXPORTED Idle(Idle&& other) noexcept;
		CULTLANG_UV_EXPORTED ~Idle() noexcept;
		CULTLANG_UV_EXPORTED Idle& operator= (const Idle& other) = delete;
		CULTLANG_UV_EXPORTED Idle& operator= (Idle&& other) noexcept;


		CULTLANG_UV_EXPORTED void stop();
	};
}}