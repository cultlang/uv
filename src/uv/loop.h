#pragma once
#include "common.h"
#include "prelude.h"

namespace cultlang{
namespace uv
{
	class Loop
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::Loop);
	private:
		
	public:
		std::unique_ptr<uv_loop_t> _loop;

		CULTLANG_UV_EXPORTED Loop();
		CULTLANG_UV_EXPORTED Loop(const Loop& other) = delete;
		CULTLANG_UV_EXPORTED Loop(Loop&& other) noexcept;
		CULTLANG_UV_EXPORTED ~Loop() noexcept;
		CULTLANG_UV_EXPORTED Loop& operator= (const Loop& other) = delete;
		CULTLANG_UV_EXPORTED Loop& operator= (Loop&& other) noexcept;

		CULTLANG_UV_EXPORTED void run();
	};

	CRAFT_MULTIMETHOD_DECLARE(uv_loop, craft::types::ExpressionDispatcher);
	/*CRAFT_MULTIMETHOD_DECLARE(uv_loop_run, craft::types::ExpressionDispatcher);*/
}}