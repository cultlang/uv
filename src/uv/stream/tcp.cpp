#include "../common.h"

#include "../loop.h"
#include "tcp.h"
#include "uv/buffer.h"

#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::lisp;
using namespace craft::types;

using namespace cultlang::uv;

CRAFT_DEFINE(cultlang::uv::Tcp)
{
	_.defaults();
}


Tcp::Tcp(instance<cultlang::uv::Loop> l, instance<StreamHandler> p)
	: _tcp(std::make_unique<uv_tcp_t>())
	, _p(p)
{
	uv_tcp_init(l->_loop.get(), _tcp.get());
	_tcp->data = this;

	_tcp->read_cb = [](uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
		auto self = (Tcp*)stream->data;
		if (self->_p->_read)
		{
			auto p = self->_p->_read.getFeature<craft::lisp::PSubroutine>();
			p->execute(self->_p->_read, { 
				instance<uv_stream_t>::make(*stream), 
				instance<int64_t>::make(nread), 
				instance<uv_buf_t>::make(*buf)
			});
		}
	};
}

Tcp::Tcp(Tcp&& other) noexcept
{
	_tcp = std::move(other._tcp);
	other._tcp = nullptr;
}

Tcp& Tcp::operator=(Tcp&& other) noexcept
{
	if (this == &other) return *this;
	_tcp = std::move(other._tcp);
	other._tcp = nullptr;
	return *this;
}


Tcp::~Tcp()
{
	if (_tcp.get())
	{
		uv_close((uv_handle_t*)_tcp.get(), [](uv_handle_t* stream) {
			auto self = (Tcp*)stream->data;
			if (self->_p->_close)
			{
				auto p = self->_p->_read.getFeature<craft::lisp::PSubroutine>();
				p->execute(self->_p->_read, {});
			}
		});
	}
}

void Tcp::nodelay(bool yes)
{
	uv_tcp_nodelay(_tcp.get(), (yes) ? 1 : 0);
}

void Tcp::keepalive(bool yes, int64_t delay)
{
	uv_tcp_keepalive(_tcp.get(), (yes) ? 1 : 0, uint32_t(delay));
}

void Tcp::simultaneous_accepts(bool yes)
{
	uv_tcp_simultaneous_accepts(_tcp.get(), (yes) ? 1 : 0);
}

void Tcp::bind(uint16_t port)
{
	sockaddr_in  s;
	uv_ip4_addr("0.0.0.0", int(port), &s);

	auto res = uv_tcp_bind(_tcp.get(), (sockaddr*)&s, 0);
	if (res == UV_EADDRINUSE) throw stdext::exception("Address already in Use");
}


instance<uv_connect_t> Tcp::connect(std::string address, int16_t port)
{
	sockaddr_in  s;
	uv_ip4_addr(address.c_str(), int(port), &s);
	auto res = instance<uv_connect_t>::make();
	auto err = uv_tcp_connect(res.get(), _tcp.get(), (sockaddr*)&s, 0);
	if (err == UV_EADDRINUSE) throw stdext::exception("Address already in Use");

	return res;
}

void cultlang::uv::make_tcp_ops(craft::types::instance<craft::lisp::Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("uv/tcp", [](instance<Loop> l, instance<StreamHandler> p) {
		auto res = instance<Tcp>::make(l, p);
		return res;
	});

	semantics->builtin_implementMultiMethod("uv/tcp/bind", [](instance<Tcp> p, instance<int64_t> t) {
		p->bind(uint16_t(*t));
		return;
	});

	semantics->builtin_implementMultiMethod("uv/tcp/connect", [](instance<Tcp> p, instance<std::string> t, instance<int64_t> c) {
		return p->connect(*t, *c);
	});
}