#include "subprocess.h"

#include "uv/loop.h"

#include "lisp/library/system/prelude.h"
#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::lisp;
using namespace craft::types;

using namespace cultlang::uv;

CRAFT_DEFINE(SubprocessHandle)
{
	_.defaults();
}

CRAFT_DEFINE(Subprocess)
{
	_.defaults();
}

SubprocessHandle::SubprocessHandle(SubprocessHandle&& other) noexcept
{
	_handle = std::move(other._handle);
	other._handle = nullptr;
}

SubprocessHandle& SubprocessHandle::operator=(SubprocessHandle&& other) noexcept
{
	if (this == &other) return *this;
	_handle = std::move(other._handle);
	other._handle = nullptr;
	return *this;
}


SubprocessHandle::~SubprocessHandle()
{
	if (_handle.get()) uv_close((uv_handle_t*)_handle.get(), 0);
}


Subprocess::Subprocess(instance<std::string> command, std::vector<instance<std::string>> arglist, instance<> function)
	: _command(command)
	, _args(arglist)
	, _handle(instance<SubprocessHandle>::make())
	, _function(function)
{
	file = command->c_str();
	args = new char*[arglist.size()];
	for (auto i = 0; i < arglist.size(); i++)
	{
		args[i] = const_cast<char*>(_args[i]->c_str());
	}
	_handle->_handle->data = this;
	exit_cb = [](uv_process_t* i, int64_t exit, int32_t term) 
	{
		auto self = (Subprocess*)i->data;
	};
}

Subprocess::~Subprocess()
{
	delete[] args;
}


void cultlang::uv::make_subprocess_ops(craft::types::instance<craft::lisp::Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("uv/subprocess",
		[](instance<Loop> l, instance<std::string> s, instance <craft::lisp::Function> cb)
	{
		std::vector<instance<std::string>> args;
		
		auto res = instance<Subprocess>::make(s, args, cb);
		uv_spawn(l->_loop.get(), res->_handle->_handle.get(), (uv_process_options_s*)res.get());
	});
	
	semantics->builtin_implementMultiMethod("uv/subprocess",
		[](instance<Loop> l, instance<std::string> s, instance<craft::lisp::library::List> args, instance <craft::lisp::Function> cb)
	{
		auto& data = args->data();

		std::vector<instance<std::string>> _args;
		for (auto d : data)
		{
			if (!d.isType<std::string>())
			{
				throw stdext::exception("Arglist must be homogonous list of strings");
			}
			_args.push_back(d.asType<std::string>());
		}

		auto res = instance<Subprocess>::make(s, _args, cb);
		uv_spawn(l->_loop.get(), res->_handle->_handle.get(), (uv_process_options_s*)res.get());
	});
}