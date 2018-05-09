#include "uv/common.h"

#include "uv/all.h"
#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::types;
using namespace cultlang;
using namespace cultlang::uv;

typedef instance<uvw::Loop> t_lop;
typedef instance<StreamContext> t_sC;
typedef instance<std::string> t_str;
typedef instance<uint32_t> t_u32;
typedef instance<craft::lisp::PSubroutine> t_sub;
typedef instance<uvw::PipeHandle> t_pH;

#define lMM semantics->builtin_implementMultiMethod
#define uVP "uv/pipe"

void cultlang::uv::make_pipe_bindings(craft::types::instance<craft::lisp::Module> m)
{
	auto semantics = m->require<lisp::CultSemantics>();

	lMM(uVP, [](t_lop l) {
		auto request = l->resource<uvw::PipeHandle>();

		request->on<uvw::ErrorEvent>([](const uvw::ErrorEvent & e, uvw::PipeHandle &srv)
		{
			auto data = srv.data<StreamContext>();
			if (data->onerr)
			{
				data->onerr->execute(data->onerr, { instance<uvw::ErrorEvent>::make(e.code()), srv.craft_instance() });
			}
		});

		request->on<uvw::ConnectEvent>([](uvw::ConnectEvent & e, uvw::PipeHandle &srv)
		{
			auto data = srv.data<StreamContext>();
			if (data->onconnect)
			{
				data->onconnect->execute(data->onconnect, { instance<uvw::ConnectEvent>::make(e), srv.craft_instance() });
			}
		});

		request->on<uvw::CloseEvent>([](const uvw::CloseEvent & e, uvw::PipeHandle &srv)
		{
			auto data = srv.data<StreamContext>();
			if (data->onclose)
			{
				data->onclose->execute(data->onclose, { instance<uvw::CloseEvent>::make(), srv.craft_instance() });
			}
		});

		request->on<uvw::EndEvent>([](const uvw::EndEvent & e, uvw::PipeHandle &srv)
		{
			auto data = srv.data<StreamContext>();
			if (data->onend)
			{
				data->onend->execute(data->onend, { instance<uvw::EndEvent>::make(e), srv.craft_instance() });
			}
		});

		request->on<uvw::ListenEvent>([](const uvw::ListenEvent & e, uvw::PipeHandle &srv)
		{
			auto data = srv.data<StreamContext>();
			if (data->onlisten)
			{
				data->onlisten->execute(data->onlisten, { instance<uvw::ListenEvent>::make(e), srv.craft_instance() });
			}
		});

		request->on<uvw::ShutdownEvent>([](const uvw::ShutdownEvent & e, uvw::PipeHandle &srv)
		{
			auto data = srv.data<StreamContext>();
			if (data->onshutdown)
			{
				data->onshutdown->execute(data->onshutdown, { instance<uvw::ShutdownEvent>::make(e), srv.craft_instance() });
			}
		});

		request->on<uvw::WriteEvent>([](const uvw::WriteEvent & e, uvw::PipeHandle &srv)
		{
			auto data = srv.data<StreamContext>();
			if (data->onwrite)
			{
				data->onwrite->execute(data->onwrite, { instance<uvw::WriteEvent>::make(e), srv.craft_instance() });
			}
		});

		request->on<uvw::DataEvent>([](uvw::DataEvent & e, uvw::PipeHandle &srv) mutable
		{
			auto data = srv.data<StreamContext>();
			if (data->ondata)
			{
				data->ondata->execute(data->ondata, {
					instance<uvw::DataEvent>::make(std::move(e.data), e.length),
					srv.craft_instance()
					});
			}
		});

		return request;
	});
	
	lMM(uVP"/bind", [](t_pH server, t_str pname) {
		server->bind(*pname);
		server->listen();
	});
	lMM(uVP"/connect", [](t_pH client, t_str pname) {
		client->connect(*pname);
	});
}