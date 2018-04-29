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

	//auto loop = instance<cultlang::uv::Loop>::make();
	instance<Module> live_module = global_env->ns_user->requireModule("repl:console");
	

	auto semantics = live_module->require<CultSemantics>();

	cultlang::uv::make_uv_bindings(live_module);
	
	live_module->initialize();
	std::string timer = (R"f(
(define foo (variable 10))
(uv/timer 0 1000 (function () 
	(do
		(print "ff" foo)
		(set foo (- foo 1))
		(cond (== foo 0) false true)
	)
))
(uv/loop)
)f");

	instance<Module> statement;
	try
	{
		auto statement_loader = instance<AnonLoader>::make();
		statement_loader->setContent(instance<std::string>::make(timer));
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