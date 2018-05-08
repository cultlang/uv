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
typedef instance<int64_t> t_i64;

typedef instance<craft::lisp::PSubroutine> t_sub;
CRAFT_DEFINE(PromisePair) { _.defaults(); }
CRAFT_DEFINE(FSContext) { _.defaults(); }
CRAFT_DEFINE(StreamContext) { _.defaults(); }

PromisePair::PromisePair(instance<craft::lisp::PSubroutine> f, instance<craft::lisp::PSubroutine> s)
	: fail(f)
	, success(s)
{

}

StreamContext::StreamContext()
{

}

void uv::make_stream_bindings(craft::types::instance<craft::lisp::Module> m)
{
	auto semantics = m->require<lisp::CultSemantics>();

	lMM(uVS, []() { return t_sC::make(); });

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
}