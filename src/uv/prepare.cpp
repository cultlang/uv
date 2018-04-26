#include "common.h"

#include "loop.h"
#include "prepare.h"

#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::types;

using namespace cultlang::uv;

CRAFT_DEFINE(cultlang::uv::Prepare)
{
	_.defaults();
}


Prepare::Prepare(instance<Loop> l, instance<craft::lisp::PSubroutine> p)
	: _prepare(std::make_unique<uv_prepare_t>())
	, _p(p)
{
	uv_prepare_init(l->_loop.get(), _prepare.get());
	_prepare->data = this;
	uv_prepare_start(_prepare.get(), [](uv_prepare_t* handle) {
		auto self = (Prepare*)handle->data;
		self->_p->execute(self->_p, { self->craft_instance()});
	});

}

Prepare::Prepare(Prepare&& other) noexcept
{
	_prepare = std::move(other._prepare);
	other._prepare = nullptr;
}

Prepare& Prepare::operator=(Prepare&& other) noexcept
{
	if (this == &other) return *this;
	_prepare = std::move(other._prepare);
	other._prepare = nullptr;
	return *this;
}


Prepare::~Prepare()
{
	if (_prepare.get()) uv_prepare_stop(_prepare.get());
}

void Prepare::stop()
{
	if (_prepare.get())
	{
		uv_prepare_stop(_prepare.get());
	}
}