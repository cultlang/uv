#include "uv/common.h"

namespace cultlang {
namespace uv {

	class PromisePair
		: public virtual craft::types::Object
	{
	private:
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::PromisePair);
	public:
		craft::instance<craft::lisp::PSubroutine> fail, success;

		PromisePair(craft::instance<craft::lisp::PSubroutine> fail, craft::instance<craft::lisp::PSubroutine> success);
	};

	class FSContext
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::FSContext);
	public:
		size_t read;
		craft::instance<craft::lisp::PSubroutine> onerr;
		craft::instance<craft::lisp::PSubroutine> ondata;
		craft::instance<craft::lisp::PSubroutine> ondone;
		craft::instance<> ctx;

		inline FSContext(craft::instance<craft::lisp::PSubroutine> e, craft::instance<craft::lisp::PSubroutine> r, craft::instance<craft::lisp::PSubroutine> d)
		{
			read = 0;
			onerr = e;
			ondata = r;
			ondone = d;
		}

	};

	class StreamContext
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::StreamContext);
	public:
		size_t read;
		craft::instance<> ctx;

		craft::instance<craft::lisp::PSubroutine> onerr;
		craft::instance<craft::lisp::PSubroutine> onclose;
		craft::instance<craft::lisp::PSubroutine> onconnect;
		craft::instance<craft::lisp::PSubroutine> onend;
		craft::instance<craft::lisp::PSubroutine> onlisten;
		craft::instance<craft::lisp::PSubroutine> onshutdown;
		craft::instance<craft::lisp::PSubroutine> onwrite;
		craft::instance<craft::lisp::PSubroutine> ondata;

		CULTLANG_UV_EXPORTED StreamContext();

	};

	CULTLANG_UV_EXPORTED void make_stream_bindings(craft::types::instance<craft::lisp::Module> m);
}}