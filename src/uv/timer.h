#pragma once
#include "common.h"

#include "prelude.h"
namespace cultlang{
namespace uv
{
	class Timer
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::Timer)
	private:
		std::unique_ptr<uv_timer_t> _timer;
		int64_t _delay; 
		int64_t _repeat;

	public:
		craft::types::instance<> _p;

		CULTLANG_UV_EXPORTED Timer(craft::types::instance<Loop> l, craft::types::instance<int64_t> delay, craft::types::instance<int64_t> repeat, craft::types::instance<> p);
		CULTLANG_UV_EXPORTED void craft_setupInstance();

		CULTLANG_UV_EXPORTED Timer(const Timer& other) = delete;
		CULTLANG_UV_EXPORTED Timer(Timer&& other) noexcept;
		CULTLANG_UV_EXPORTED ~Timer() noexcept;
		CULTLANG_UV_EXPORTED Timer& operator= (const Timer& other) = delete;
		CULTLANG_UV_EXPORTED Timer& operator= (Timer&& other) noexcept;


		CULTLANG_UV_EXPORTED void stop();
	};
}}