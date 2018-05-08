#include "uv/common.h"
#include "uv/all.h"

#include "lisp/semantics/cult/cult.h"


using namespace craft;
using namespace craft::lisp;

namespace cuv = cultlang::uv;

std::string test_read = (R"f(
(define loop (uv/loop))
(define fish (uv/read loop "CultlangUv.sln" 100
		(function (fail hndl) (print fail))
		(function (val hndl) (print val))
		(function (done) (print "Done"))
	)
)
(uv/run loop)
)f");

std::string test_write = (R"f(
(define loop (uv/loop))
(define data "The Quick Brown Fox Jumped Over The Lazy Dog")
(define isdone (variable false))
(define fish (uv/write loop "foo.txt"
		(function (fail hndl) (print fail))
		(function (val hndl) (do
			(cond isdone null (do (set isdone true) data))
		))
		(function (done) (print "Done"))
	)
)
(uv/run loop)
)f");

std::string test_tcp = (R"f(
(define loop (uv/loop))
(define server (uv/tcp loop))

(define pHandlers (uv/stream))
(uv/stream/error pHandlers (function (ev client)
  (do
    (print ev)
    (uv/close client)  
  )

))
(uv/stream/data pHandlers (function (ev client)
  (print ev)
))

(uv/stream/close pHandlers (function (ev client)
  (do
    (print "Peer Close")
    (uv/close server)  
  )
  
))
(uv/stream/end pHandlers (function (ev client)
  (do
    (print "Peer End")
    (uv/close client)  
  )
))

(define sHandlers (uv/stream))
(uv/stream/error sHandlers (function (ev server)
  (print ev)
))
(uv/stream/listen sHandlers (function (ev server)
  (do
    (print "listen")
    (define peer (uv/tcp loop))
    (uv/tcp/context peer pHandlers)
    (uv/tcp/accept server peer)
    (uv/tcp/read peer)
  )
))

(uv/tcp/context server sHandlers)
(uv/tcp/bind server "127.0.0.1" 80u32)

(define client (uv/tcp loop))

(define cHandlers (uv/stream))
(uv/stream/error cHandlers (function (ev client)
  (do
    (print ev)
    (uv/close client)  
  )

))
(uv/stream/connect cHandlers (function (ev client)
  (do
    (print "Client Connected")
    (uv/tcp/write client "Hello World")
    (uv/close client)    
  )
))
(uv/stream/close cHandlers (function (ev client)
  (do
    (print "Client Close")
  )
  
))
(uv/stream/end cHandlers (function (ev client)
  (do
    (print "Client End")
  )
))

(uv/tcp/context client cHandlers)
(uv/tcp/connect client "127.0.0.1" 80u32)
(uv/run loop)
)f");

int main(int argc, char** argv) {
	types::boot();
	instance<Environment> global_env = instance<Environment>::make(spdlog::stdout_color_mt("environment"));
	instance<Namespace> ns = global_env->ns_user;

	//auto loop = instance<cultlang::uv::Loop>::make();
	instance<Module> live_module = global_env->ns_user->requireModule("repl:console");
	

	auto semantics = live_module->require<CultSemantics>();

	cultlang::uv::make_uv_bindings(live_module);
	
	live_module->initialize();

	instance<Module> statement;
	try
	{
		auto statement_loader = instance<AnonLoader>::make();
		statement_loader->setContent(instance<std::string>::make(test_tcp));
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