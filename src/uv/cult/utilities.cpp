#include "uv/common.h"

#include "uv/all.h"
#include "lisp/library/system/prelude.h"
#include "lisp/semantics/cult/calling.h"

#include "uv/all.h"

using namespace craft;
using namespace craft::types;
using namespace cultlang;
using namespace cultlang::uv;

typedef instance<uvw::Loop> t_lop;
typedef instance<StreamContext> t_sC;
typedef instance<std::string> t_str;
typedef instance<uint32_t> t_u32;
typedef instance<uint64_t> t_i64;
typedef instance<craft::lisp::PSubroutine> t_sub;
typedef instance<uvw::PipeHandle> t_pH;

#define lMM semantics->builtin_implementMultiMethod
#define uVP "uv"
void cultlang::uv::make_util_bindings(craft::types::instance<craft::lisp::Module> m)
{
	auto semantics = m->require<lisp::CultSemantics>();

	semantics->builtin_implementMultiMethod("uv/check",
		[](t_lop loop, t_sub f, t_sub s)
	{
		auto handle = loop->resource<uvw::CheckHandle>();

		handle->data(instance<PromisePair>::make(f, s));

		handle->on<uvw::ErrorEvent>([](uvw::ErrorEvent &ev, auto &hndl) {
			auto sub = hndl.template data<PromisePair>()->fail;
			sub->execute(sub, { instance<uvw::ErrorEvent>::make(ev.code()), hndl.craft_instance() });
		});

		handle->on<uvw::CheckEvent>([](auto &ev, auto &hndl) {
			auto sub = hndl.template data<PromisePair>()->success;
			sub->execute(sub, { instance<uvw::CheckEvent>::make(), hndl.craft_instance() });
		});

		return handle;
	});

	semantics->builtin_implementMultiMethod("uv/idle",
		[](t_lop loop, t_sub f, t_sub s)
	{
		auto handle = loop->resource<uvw::IdleHandle>();

		handle->data(instance<PromisePair>::make(f, s));

		handle->on<uvw::ErrorEvent>([](uvw::ErrorEvent &ev, auto &hndl) {
			auto sub = hndl.template data<PromisePair>()->fail;
			sub->execute(sub, { instance<uvw::ErrorEvent>::make(ev.code()), hndl.craft_instance() });
		});

		handle->on<uvw::IdleEvent>([](auto &ev, auto &hndl) {
			auto sub = hndl.template data<PromisePair>()->success;
			sub->execute(sub, { instance<uvw::IdleEvent>::make(), hndl.craft_instance() });
		});

		return handle;
	});

	semantics->builtin_implementMultiMethod("uv/prepare",
		[](t_lop loop, t_sub f, t_sub s)
	{
		auto handle = loop->resource<uvw::PrepareHandle>();

		handle->data(instance<PromisePair>::make(f, s));

		handle->on<uvw::ErrorEvent>([](uvw::ErrorEvent &ev, auto &hndl) {
			auto sub = hndl.template data<PromisePair>()->fail;
			sub->execute(sub, { instance<uvw::ErrorEvent>::make(ev.code()), hndl.craft_instance() });
		});

		handle->on<uvw::PrepareEvent>([](auto &ev, auto &hndl) {
			auto sub = hndl.template data<PromisePair>()->success;
			sub->execute(sub, { instance<uvw::PrepareEvent>::make(), hndl.craft_instance() });
		});

		return handle;
	});

	semantics->builtin_implementMultiMethod("uv/timer",
		[](t_lop loop, t_i64 a, t_i64 b, t_sub f, t_sub s)
	{
		auto handle = loop->resource<uvw::TimerHandle>();

		handle->data(instance<PromisePair>::make(f, s));

		handle->on<uvw::ErrorEvent>([](uvw::ErrorEvent &ev, auto &hndl) {
			auto sub = hndl.template data<PromisePair>()->fail;
			sub->execute(sub, { instance<uvw::ErrorEvent>::make(ev.code()), hndl.craft_instance() });
		});

		handle->on<uvw::TimerEvent>([](auto &ev, auto &hndl) {
			auto sub = hndl.template data<PromisePair>()->success;
			sub->execute(sub, { instance<uvw::TimerEvent>::make(), hndl.craft_instance() });
		});

		handle->start(uvw::TimerHandle::Time{ *a }, uvw::TimerHandle::Time{ *b });

		return handle;
	});

	lMM(uVP"/stdin", [](t_lop l) mutable
	{
		uv_handle_type t = uv_guess_handle(0);
		switch (t)
		{
		case UV_TTY:
		{
			instance<uvw::TTYHandle> res = lisp::Execution::exec_fromCurrentModule("uv/tty", { l, t_u32::make(0) });
			return (instance<>)res;
		}
		case UV_NAMED_PIPE:
		{
			instance<uvw::PipeHandle> res = lisp::Execution::exec_fromCurrentModule("uv/pipe", { l });
			res->open(0);
			return (instance<>)res;
		}
		default:
			throw stdext::exception("Unable to Detect STDIN type");
			break;
		}
	});
	lMM(uVP"/stdout", [](t_lop l) mutable
	{
		uv_handle_type t = uv_guess_handle(0);
		switch (t)
		{
		case UV_TTY:
		{

			instance<uvw::TTYHandle> res = lisp::Execution::exec_fromCurrentModule("uv/tty", { l, t_u32::make(1) });
			return (instance<>)res;
		}
		case UV_NAMED_PIPE:
		{
			instance<uvw::PipeHandle> res = lisp::Execution::exec_fromCurrentModule("uv/pipe", { l });
			res->open(1);
			return (instance<>)res;
		}
		default:
			throw stdext::exception("Unable to Detect STOUT type");
			break;
		}
	});

	semantics->builtin_implementMultiMethod("uv/close",
		[](instance<uvw::CheckHandle> handle)
	{
		handle->close();
	});
	
	semantics->builtin_implementMultiMethod("uv/close",
		[](instance<uvw::PrepareHandle> handle)
	{
		handle->close();
	});

	semantics->builtin_implementMultiMethod("uv/close",
		[](instance<uvw::IdleHandle> handle)
	{
		handle->close();
	});

	semantics->builtin_implementMultiMethod("uv/close",
		[](instance<uvw::TimerHandle> handle)
	{
		handle->close();
	});
}


