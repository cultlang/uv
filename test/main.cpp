#include "uv/common.h"
#include "uv/all.h"

#include "lisp/semantics/cult/cult.h"

using namespace craft;
using namespace craft::lisp;

namespace cuv = cultlang::uv;

int main(int argc, char** argv) {
	types::boot();
	instance<Environment> global_env = instance<Environment>::make(spdlog::stdout_color_mt("environment"));
	instance<Namespace> ns = global_env->ns_user;

	auto loop = instance<cultlang::uv::Loop>::make();
	instance<Module> live_module = global_env->ns_user->requireModule("repl:console");
	

	auto semantics = live_module->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("uv/loop",
		[]() -> instance<cuv::Loop>
	{
		return instance<cuv::Loop>::make();
	});

	semantics->builtin_implementMultiMethod("uv/idle",
		[](instance<cuv::Loop> a , instance<PSubroutine> b) -> instance<cuv::Idle>
	{
		return instance<cuv::Idle>::make(a, b);
	});

	semantics->builtin_implementMultiMethod("uv/check",
		[](instance<cuv::Loop> a, instance<PSubroutine> b) -> instance<cuv::Check>
	{
		return instance<cuv::Check>::make(a, b);
	});

	semantics->builtin_implementMultiMethod("uv/prepare",
		[](instance<cuv::Loop> a, instance<PSubroutine> b) -> instance<cuv::Prepare>
	{
		return instance<cuv::Prepare>::make(a, b);
	});

	semantics->builtin_implementMultiMethod("uv/timer",
		[](instance<cuv::Loop> a, instance<int64_t> b, instance<int64_t> c, instance<PSubroutine> d)
	{
		return instance<cuv::Timer>::make(a, b, c, d);
	});


	semantics->builtin_implementMultiMethod("uv/stop",
		[](instance<cuv::Idle> i)
	{
		i->stop();
	});

	semantics->builtin_implementMultiMethod("uv/stop",
		[](instance<cuv::Check> i)
	{
		i->stop();
	});

	semantics->builtin_implementMultiMethod("uv/stop",
		[](instance<cuv::Prepare> i)
	{
		i->stop();
	});

	semantics->builtin_implementMultiMethod("uv/stop",
		[](instance<cuv::Timer> i)
	{
		i->stop();
	});


	semantics->builtin_implementMultiMethod("uv/run",
		[](instance<cuv::Loop> a)
	{
		a->run();
	});

	cultlang::uv::make_buffer_ops(live_module);
	cultlang::uv::make_fs_ops(live_module);

	live_module->initialize();
	std::string timer = (R"f(
(define loop (uv/loop))

(define timerf (function (tim) (print "Tick")))
(define timetime (uv/timer loop 0 1000 timerf))
(uv/run loop)
)f");
	//
	std::string read = (R"f(
(define loop (uv/loop))
(define buffers (list))
(define readf (function (buf) (list/push buffers buf)))
(define endf (function () (print (buffer/join buffers))))

(define errf (function (err) (print "Oops didn't work")))
(define readh (uv/read loop "foo.txt" errf readf endf))
(uv/run loop)
)f");

	std::string write = (R"f(
(define loop (uv/loop))
(define strings (list "One" "Two" "Three" "Four"))
(define writef (function (buf) (list/push buffers buf)))
(define endf (function () (print (buffer/join buffers))))
(define errf (function (err) (print "Oops didn't work")))
(define writeh (uv/write loop "bar.txt" errf writef endf))
(uv/run loop)
)f");

	std::string mkdir = (R"f(
(define loop (uv/loop))
(define mkdirf (function (i) (print i)))
(define writeh (uv/mkdir loop "fishtacos" mkdirf))
(uv/run loop)
)f");

	instance<Module> statement;
	try
	{
		auto statement_loader = instance<AnonLoader>::make();
		statement_loader->setContent(instance<std::string>::make(mkdir));
		statement_loader->setModule(live_module);
		statement = ns->requireModule("anon:repl", statement_loader);
	}
	catch (std::exception const& ex)
	{
		std::cout << "parser: " << ex.what() << '\n';
	}

	try
	{
		if (statement)
		{
			live_module->appendModule(statement);
			instance<> res = live_module->lastExecutedResult();

			std::cout << res.toString() << '\n';

			return 0;
		}
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << '\n';
	}
}