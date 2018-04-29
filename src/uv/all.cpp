#include "uv/common.h"

#include "all.h"

#include "lisp/semantics/cult/calling.h"

#include "uvw.hpp"

using namespace craft;
using namespace craft::lisp;

CRAFT_DEFINE(uvw::AsyncHandle)
{
	_.defaults();
}

CRAFT_DEFINE(uvw::Loop)
{
	_.defaults();
}

CRAFT_DEFINE(uvw::PrepareEvent)
{
	_.defaults();
}
CRAFT_DEFINE(uvw::PrepareHandle)
{
	_.defaults();
}

void cultlang::uv::make_uv_bindings(instance<craft::lisp::Module> m)
{
	auto semantics = m->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("uv/loop",
		[]()
	{
		auto& loop = uvw::Loop::getDefault();

		loop->run();

		return instance<>();
	});

	semantics->builtin_implementMultiMethod("uv/check",
		[](instance<craft::lisp::PSubroutine> s)
	{
		auto loop = uvw::Loop::getDefault();
		instance<uvw::CheckHandle> handle = loop->resource<uvw::CheckHandle>();

		handle->data(s);


		handle->on<uvw::CheckEvent>([](const auto &, auto &hndl) {
			auto a = hndl.data();
			a.asType<craft::lisp::PSubroutine>()->execute(a, {});
		});
	});
}
#include "types/dll_entry.inc"
