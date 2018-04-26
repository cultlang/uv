#include "common.h"

#include "loop.h"
#include "check.h"

#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::types;

using namespace cultlang::uv;

CRAFT_DEFINE(cultlang::uv::Check)
{
	_.defaults();
}


Check::Check(instance<Loop> l, instance<craft::lisp::PSubroutine> p)
	: _check(std::make_unique<uv_check_t>())
	, _p(p)
{
	uv_check_init(l->_loop.get(), _check.get());
	_check->data = this;
	uv_check_start(_check.get(), [](uv_check_t* handle) {
		auto self = (Check*)handle->data;
		self->_p->execute(self->_p, { self->craft_instance()});
	});

}

Check::Check(Check&& other) noexcept
{
	_check = std::move(other._check);
	other._check = nullptr;
}

Check& Check::operator=(Check&& other) noexcept
{
	if (this == &other) return *this;
	_check = std::move(other._check);
	other._check = nullptr;
	return *this;
}


Check::~Check()
{
	if (_check.get()) uv_check_stop(_check.get());
}

void Check::stop()
{
	if (_check.get())
	{
		uv_check_stop(_check.get());
	}
}