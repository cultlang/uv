#include "uv/common.h"
#include "fs.h"

#include "uv/loop.h"

#include "lisp/library/system/prelude.h"
#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::lisp;
using namespace craft::types;

using namespace cultlang::uv;

CRAFT_DEFINE(Fs)
{
	_.defaults();
}

CRAFT_DEFINE(ReadFile)
{
	_.defaults();
}

Fs::Fs()
	: _fs(std::make_unique<uv_fs_t>())
{

}

Fs::Fs(Fs&& other) noexcept
{
	_fs = std::move(other._fs);
	other._fs = nullptr;
}

Fs& Fs::operator=(Fs&& other) noexcept
{
	if (this == &other) return *this;
	_fs = std::move(other._fs);
	other._fs = nullptr;
	return *this;
}


Fs::~Fs()
{
	if (_fs.get()) uv_fs_req_cleanup(_fs.get());
}

ReadFile::ReadFile(instance<cultlang::uv::Loop> l, instance<std::string> f, instance<int64_t> buf, instance<> err, instance<> read, instance<> done)
	: _open(instance<Fs>::make())
	, _read(instance<Fs>::make())
	, _close(instance<Fs>::make())
	, _file(f)
	, _fread(read)
	, _ferr(err)
	, _fdone(done)
{
	_open->_fs.get()->data = this;
	_read->_fs.get()->data = this;
	_close->_fs.get()->data = this;
	_loop = l;
	this->_buffer = uv_buf_init(new char[*buf], *buf);
}


void ReadFile::craft_setupInstance()
{
	uv_fs_open(_loop->_loop.get(), _open->_fs.get(), _file->c_str(), O_RDONLY, 0, [](uv_fs_t* open_req) {
		auto self = (ReadFile*)open_req->data;

		if (open_req->result >= 0)
		{
			static void(*on_read)(uv_fs_t*)  = [](uv_fs_t* read_req)
			{
				auto self = (ReadFile*)read_req->data;
				if (read_req->result < 0)
				{
					self->_ferr.getFeature<PSubroutine>()->execute(self->_ferr, {});
					// Exit Case
					delete[] self->_buffer.base;
					self->_buffer.len = 0;
				}
				else if (read_req->result == 0)
				{
					// Exit Case
					delete[] self->_buffer.base;
					self->_buffer.len = 0;
					self->_fdone.getFeature<PSubroutine>()->execute(self->_fdone, {});
				}
				else
				{
					auto p = self->_fread.getFeature<craft::lisp::PSubroutine>();

					auto lbuf = instance<craft::lisp::library::Buffer>::make(instance<int64_t>::make(read_req->result));
					memcpy(lbuf->data().data(), self->_buffer.base, read_req->result);
					self->_fread.getFeature<PSubroutine>()->execute(self->_fread, { lbuf });
					uv_fs_read(self->_loop->_loop.get(), self->_read->_fs.get(), self->_open->_fs.get()->result, &self->_buffer, 1, -1, on_read);
				}
			};
			uv_fs_read(self->_loop->_loop.get(), self->_read->_fs.get(), open_req->result, &self->_buffer, 1, -1, on_read);
		}
		else
		{
			self->_ferr.getFeature<PSubroutine>()->execute(self->_fread, {});
			// Memory Was Not Allocated Here
		}
	});
}

ReadFile::~ReadFile()
{
	if (_buffer.len) delete[] _buffer.base;
}

void cultlang::uv::make_fs_ops(craft::types::instance<craft::lisp::Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("uv/read", [](instance<cultlang::uv::Loop> l, instance<std::string> s, instance<lisp::PSubroutine> err, instance<lisp::PSubroutine> read, instance<lisp::PSubroutine> done) {
		return instance<ReadFile>::make(l, s, instance<int64_t>::make(1024), err, read, done);
	});
}