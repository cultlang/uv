#include "common.h"

#include "loop.h"
#include "idle.h"

#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::types;

using namespace cultlang::uv;

CRAFT_DEFINE(cultlang::uv::Idle)
{
	_.defaults();
}


Idle::Idle(instance<Loop> l, instance<craft::lisp::PSubroutine> p)
	: _idle(std::make_unique<uv_idle_t>())
	, _p(p)
{
	uv_idle_init(l->_loop.get(), _idle.get());
	_idle->data = this;
	uv_idle_start(_idle.get(), [](uv_idle_t* handle) {
		auto self = (Idle*)handle->data;
		self->_p->execute(self->_p, { self->craft_instance()});
	});

}

Idle::Idle(Idle&& other) noexcept
{
	_idle = std::move(other._idle);
	other._idle = nullptr;
}

Idle& Idle::operator=(Idle&& other) noexcept
{
	if (this == &other) return *this;
	_idle = std::move(other._idle);
	other._idle = nullptr;
	return *this;
}


Idle::~Idle()
{
	if (_idle.get()) uv_idle_stop(_idle.get());
}

void Idle::stop()
{
	if (_idle.get())
	{
		uv_idle_stop(_idle.get());
	}
}