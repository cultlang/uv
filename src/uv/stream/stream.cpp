#include "uv/common.h"

#include "uv/loop.h"
#include "uv/stream/stream.h"

#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::lisp;
using namespace craft::types;

using namespace cultlang::uv;

CRAFT_TYPE_DEFINE(uv_stream_t)
{
	_.add<GraphPropertyName>("uv.stream");
	_.defaults();
};

CRAFT_TYPE_DEFINE(uv_connect_t)
{
	_.add<GraphPropertyName>("uv.stream.connect");
	_.defaults();
};

CRAFT_TYPE_DEFINE(uv_shutdown_t)
{
	_.add<GraphPropertyName>("uv.stream.shutdown");
	_.defaults();
};

CRAFT_TYPE_DEFINE(uv_write_t)
{
	_.add<GraphPropertyName>("uv.stream.write");
	_.defaults();
};

CRAFT_DEFINE(cultlang::uv::StreamHandler)
{
	_.defaults();
}

void cultlang::uv::make_stream_ops(craft::types::instance<craft::lisp::Module> ret)
{
	typedef instance<craft::lisp::Function> lFunc;
	typedef instance<StreamHandler> uStream;

	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("uv/stream", []() {return uStream::make();});

	semantics->builtin_implementMultiMethod("uv/stream/connect", [](uStream a) {return a->getConnect(); });
	semantics->builtin_implementMultiMethod("uv/stream/connect", [](uStream a, lFunc b) {a->setConnect(b);});

	semantics->builtin_implementMultiMethod("uv/stream/connection", [](uStream a) {return a->getConnection(); });
	semantics->builtin_implementMultiMethod("uv/stream/connection", [](uStream a, lFunc b) {a->setConnection(b); });

	semantics->builtin_implementMultiMethod("uv/stream/shutdown", [](uStream a) {return a->getShutdown(); });
	semantics->builtin_implementMultiMethod("uv/stream/shutdown", [](uStream a, lFunc b) {a->setShutdown(b); });

	semantics->builtin_implementMultiMethod("uv/stream/read", [](uStream a) {return a->getRead();});
	semantics->builtin_implementMultiMethod("uv/stream/read", [](uStream a, lFunc b) {a->setRead(b);});

	semantics->builtin_implementMultiMethod("uv/stream/write", [](uStream a) {return a->getWrite(); });
	semantics->builtin_implementMultiMethod("uv/stream/write", [](uStream a, lFunc b) {a->setWrite(b); });

	semantics->builtin_implementMultiMethod("uv/stream/close", [](uStream a) {return a->getClose(); });
	semantics->builtin_implementMultiMethod("uv/stream/close", [](uStream a, lFunc b) {a->setClose(b); });
}