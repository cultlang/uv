#include "common.h"

#include "all.h"

#include "loop.h"

using namespace craft;
using namespace craft::types;

using namespace cultlang::uv;

CRAFT_DEFINE(cultlang::uv::Loop)
{
	_.defaults();
}

Loop::Loop()
 : _loop(std::make_unique<uv_loop_t>())
{
	uv_loop_init(_loop.get());
}

Loop::~Loop()
{
	if (_loop.get()) uv_loop_close(_loop.get());
}

Loop::Loop(Loop&& other) noexcept
{
	_loop = std::move(other._loop);
	other._loop = nullptr;
}

Loop& Loop::operator=(Loop&& other) noexcept
{
	if (this == &other) return *this;
	_loop = std::move(other._loop);
	other._loop = nullptr;
	return *this;
}


void Loop::run()
{
	uv_run(_loop.get(), UV_RUN_DEFAULT);
}

CRAFT_MULTIMETHOD_DEFINE(cultlang::uv::uv_loop)
{
	_.add_method([]()
	{
		return instance<cultlang::uv::Loop>::make();
	});
}