#include "uv/common.h"

#include "uv/loop.h"
#include "uv/buffer.h"

#include "lisp/library/system/prelude.h"
#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::lisp;
using namespace craft::types;

using namespace cultlang::uv;

CRAFT_TYPE_DEFINE(uv_buf_t)
{
	_.add<GraphPropertyName>("uv.buffer");
	//_.add<GraphPropertyCppName>("uv_buf_t");
	_.defaults();
};

void cultlang::uv::make_buffer_ops(craft::types::instance<craft::lisp::Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("uv/buffer", [](instance<std::string> s) {
		auto res = instance<uv_buf_t>::make();
		res->base = (char*)s->data();
		res->len = s->size();

		return res;
	});
	semantics->builtin_implementMultiMethod("uv/buffer", [](instance<craft::lisp::library::Buffer> s) {
		auto res = instance<uv_buf_t>::make();
		res->base = (char*)s->data().data();
		res->len = s->data().size();
		return res;
	});

	semantics->builtin_implementMultiMethod("uv/buffer/str", [](instance<uv_buf_t> s) {
		auto res = instance<std::string>::make();
		res->resize(s->len + 1);
		memcpy(const_cast<char*>(res->data()), s->base, s->len);
		res->operator[](s->len) = '\0';

		return res;
	});
}