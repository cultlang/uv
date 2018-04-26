#include "common.h"

#include "loop.h"
#include "timer.h"

#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::types;

using namespace cultlang::uv;

CRAFT_DEFINE(cultlang::uv::Timer)
{
	_.defaults();
}


Timer::Timer(instance<Loop> l, instance<int64_t> delay, instance<int64_t> repeat, instance<> p)
	: _timer(std::make_unique<uv_timer_t>())
	, _delay(*delay)
	, _repeat(*repeat)
	, _p(p)
{
	uv_timer_init(l->_loop.get(), _timer.get());
	_timer->data = this;
	

}

void Timer::craft_setupInstance()
{
	Object::craft_setupInstance();

	uv_timer_start(_timer.get(), [](uv_timer_t* handle) {
		auto self = (Timer*)handle->data;
		self->_p.getFeature<craft::lisp::PSubroutine>()->execute(self->_p, { self->craft_instance() });
	}, uint64_t(_delay), uint64_t(_repeat));
}

Timer::Timer(Timer&& other) noexcept
{
	_timer = std::move(other._timer);
	other._timer = nullptr;
}

Timer& Timer::operator=(Timer&& other) noexcept
{
	if (this == &other) return *this;
	_timer = std::move(other._timer);
	other._timer = nullptr;
	return *this;
}


Timer::~Timer()
{
	if (_timer.get()) uv_timer_stop(_timer.get());
}

void Timer::stop()
{
	if (_timer.get())
	{
		uv_timer_stop(_timer.get());
	}
}