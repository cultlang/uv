#pragma once
#include "common.h"

#include "prelude.h"
namespace cultlang{
namespace uv
{
	class Check
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::Check)
	private:
		std::unique_ptr<uv_check_t> _check;
		
	public:
		craft::types::instance<craft::lisp::PSubroutine> _p;

		CULTLANG_UV_EXPORTED Check(craft::types::instance<Loop> l, craft::types::instance<craft::lisp::PSubroutine> p);
		CULTLANG_UV_EXPORTED Check(const Check& other) = delete;
		CULTLANG_UV_EXPORTED Check(Check&& other) noexcept;
		CULTLANG_UV_EXPORTED ~Check() noexcept;
		CULTLANG_UV_EXPORTED Check& operator= (const Check& other) = delete;
		CULTLANG_UV_EXPORTED Check& operator= (Check&& other) noexcept;


		CULTLANG_UV_EXPORTED void stop();
	};
}}