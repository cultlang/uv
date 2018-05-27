#include "uv/common.h"

#include "context.h"

#include "lisp/semantics/cult/calling.h"


#define lMM semantics->builtin_implementMultiMethod
#define uVS "uv/stream"

using namespace craft;
using namespace craft::types;
using namespace cultlang;
using namespace cultlang::uv;

typedef instance<uvw::Loop> t_lop;
typedef instance<StreamContext> t_sC;
typedef instance<lisp::PSubroutine> t_pS;
typedef instance<std::string> t_str;
typedef instance<uvw::PipeHandle> t_pip;
typedef instance<uvw::TcpHandle> t_tcp;
typedef instance<uvw::TTYHandle> t_tty;
typedef instance<int64_t> t_i64;

typedef instance<craft::lisp::PSubroutine> t_sub;
CRAFT_DEFINE(PromisePair) { _.defaults(); }
CRAFT_DEFINE(StreamContext) { _.defaults(); }

PromisePair::PromisePair(instance<craft::lisp::PSubroutine> f, instance<craft::lisp::PSubroutine> s)
	: fail(f)
	, success(s)
{

}

StreamContext::StreamContext()
{

}
namespace _impl {
	//instance<craft::lisp::Symbol> at = craft::lisp::Symbol::makeSymbol
}

void uv::make_stream_bindings(craft::types::instance<craft::lisp::Module> m)
{
	auto semantics = m->require<lisp::CultSemantics>();

	lMM(uVS, []() { return t_sC::make(); });
	lMM(uVS, [](instance<lisp::library::Map> m) { 
		auto res = t_sC::make();
		instance<> at;

		at = m->at(craft::lisp::Symbol::makeSymbol(":error"));
		if (at) res->onerr = at.asType<lisp::PSubroutine>();

		at = m->at(craft::lisp::Symbol::makeSymbol(":close"));
		if (at) res->onclose = at.asType<lisp::PSubroutine>();

		at = m->at(craft::lisp::Symbol::makeSymbol(":connect"));
		if (at) res->onconnect = at.asType<lisp::PSubroutine>();

		at = m->at(craft::lisp::Symbol::makeSymbol(":end"));
		if (at) res->onend = at.asType<lisp::PSubroutine>();

		at = m->at(craft::lisp::Symbol::makeSymbol(":listen"));
		if (at) res->onlisten = at.asType<lisp::PSubroutine>();

		at = m->at(craft::lisp::Symbol::makeSymbol(":shutdown"));
		if (at) res->onshutdown = at.asType<lisp::PSubroutine>();

		at = m->at(craft::lisp::Symbol::makeSymbol(":write"));
		if (at) res->onwrite = at.asType<lisp::PSubroutine>();

		at = m->at(craft::lisp::Symbol::makeSymbol(":data"));
		if (at) res->ondata = at.asType<lisp::PSubroutine>();

		return res;
	});

	lMM(uVS"/error", [](t_sC s) { return s->onerr; });
	lMM(uVS"/error", [](t_sC s, t_pS p) { s->onerr = p; });

	lMM(uVS"/close", [](t_sC s) { return s->onclose; });
	lMM(uVS"/close", [](t_sC s, t_pS p) { s->onclose = p; });

	lMM(uVS"/connect", [](t_sC s) { return s->onconnect; });
	lMM(uVS"/connect", [](t_sC s, t_pS p) { s->onconnect = p; });

	lMM(uVS"/end", [](t_sC s) { return s->onend; });
	lMM(uVS"/end", [](t_sC s, t_pS p) { s->onend = p; });

	lMM(uVS"/listen", [](t_sC s) { return s->onlisten; });
	lMM(uVS"/listen", [](t_sC s, t_pS p) { s->onlisten = p; });

	lMM(uVS"/shutdown", [](t_sC s) { return s->onshutdown; });
	lMM(uVS"/shutdown", [](t_sC s, t_pS p) { s->onshutdown = p; });

	lMM(uVS"/write", [](t_sC s) { return s->onwrite; });
	lMM(uVS"/write", [](t_sC s, t_pS p) { s->onwrite = p; });

	lMM(uVS"/data", [](t_sC s) { return s->ondata; });
	lMM(uVS"/data", [](t_sC s, t_pS p) { s->ondata = p; });

	lMM(uVS"/context", [](instance<uvw::TcpHandle> h) {return h->data<StreamContext>(); });
	lMM(uVS"/context", [](instance<uvw::TcpHandle> h, t_sC c) {return h->data(c); });

	lMM(uVS"/context", [](instance<uvw::PipeHandle> h) {return h->data<StreamContext>(); });
	lMM(uVS"/context", [](instance<uvw::PipeHandle> h, t_sC c) {return h->data(c); });

	lMM(uVS"/context", [](instance<uvw::TTYHandle> h) {return h->data<StreamContext>(); });
	lMM(uVS"/context", [](instance<uvw::TTYHandle> h, t_sC c) {return h->data(c); });

	lMM(uVS"/accept", [](t_pip server, t_pip client) {server->accept(*client);});
	lMM(uVS"/listen", [](t_pip server) {server->listen();});
	lMM(uVS"/read", [](t_pip hnd) {hnd->read();});
	lMM(uVS"/write", [](t_pip hnd, t_str d) {
		hnd->write(d->data(), d->size());
	});
	lMM("uv/close", [](t_pip hnd) {hnd->close();});

	lMM(uVS"/accept", [](t_tcp server, t_tcp client) {server->accept(*client); });
	lMM(uVS"/listen", [](t_tcp server) {server->listen(); });
	lMM(uVS"/read", [](t_tcp hnd) {hnd->read(); });
	lMM(uVS"/write", [](t_tcp hnd, t_str d) {hnd->write(d->data(), d->size()); });
	lMM("uv/close", [](t_tcp hnd) {hnd->close(); });

	lMM(uVS"/accept", [](t_tty server, t_tty client) {server->accept(*client); });
	lMM(uVS"/listen", [](t_tty server) {server->listen(); });
	lMM(uVS"/read", [](t_tty hnd) {hnd->read(); });
	lMM(uVS"/write", [](t_tty hnd, t_str d) {hnd->write(d->data(), d->size()); });
	lMM("uv/close", [](t_tty hnd) {hnd->close(); });
}