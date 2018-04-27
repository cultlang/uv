#include "uv/common.h"
#include "fs.h"

#include "uv/loop.h"

#include "lisp/library/system/prelude.h"
#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::lisp;
using namespace craft::types;

using namespace cultlang::uv;

#pragma warning(disable:4244)
#pragma warning(disable:4267)

CRAFT_DEFINE(Fs)
{
	_.defaults();
}

CRAFT_DEFINE(ReadFile)
{
	_.defaults();
}

CRAFT_DEFINE(WriteFile)
{
	_.defaults();
}

CRAFT_DEFINE(FileOp)
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
	this->_buffer = uv_buf_init(new char[uint32_t(*buf)], uint32_t(*buf));

	uv_fs_open(_loop->_loop.get(), _open->_fs.get(), _file->c_str(), O_RDONLY, 0, [](uv_fs_t* open_req) {
		auto self = (ReadFile*)open_req->data;

		if (open_req->result >= 0)
		{
			static void(*on_read)(uv_fs_t*) = [](uv_fs_t* read_req)
			{
				auto self = (ReadFile*)read_req->data;
				if (read_req->result < 0)
				{
					// Exit Case
					delete[] self->_buffer.base;
					self->_buffer.len = 0;
					uv_fs_close(self->_loop->_loop.get(), self->_close->_fs.get(), self->_open->_fs.get()->result, [](uv_fs_t* close_req) {
						auto self = (ReadFile*)close_req->data;
						self->_fdone.getFeature<PSubroutine>()->execute(self->_ferr, {});
					});
				}
				else if (read_req->result == 0)
				{
					// Exit Case
					delete[] self->_buffer.base;
					self->_buffer.len = 0;
					uv_fs_close(self->_loop->_loop.get(), self->_close->_fs.get(), self->_open->_fs.get()->result, [](uv_fs_t* close_req) {
						auto self = (ReadFile*)close_req->data;
						self->_fdone.getFeature<PSubroutine>()->execute(self->_fdone, {});
					});
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
			self->_ferr.getFeature<PSubroutine>()->execute(self->_ferr, {});
			// Memory Was Not Allocated Here
		}
	});
}

ReadFile::~ReadFile()
{
	if (_buffer.len) delete[] _buffer.base;
	uv_fs_req_cleanup(_open->_fs.get());
	uv_fs_req_cleanup(_read->_fs.get());
	uv_fs_req_cleanup(_close->_fs.get());
}

WriteFile::WriteFile(instance<cultlang::uv::Loop> l, instance<std::string> f, instance<int64_t> buf, instance<> err, instance<> write, instance<> done)
	: _open(instance<Fs>::make())
	, _write(instance<Fs>::make())
	, _close(instance<Fs>::make())
	, _file(f)
	, _fwrite(write)
	, _ferr(err)
	, _fdone(done)
{
	_open->_fs.get()->data = this;
	_write->_fs.get()->data = this;
	_close->_fs.get()->data = this;
	_loop = l;

	uv_fs_open(_loop->_loop.get(), _open->_fs.get(), _file->c_str(), O_WRONLY, 0, [](uv_fs_t* open_req) {
		auto self = (WriteFile*)open_req->data;

		if (open_req->result >= 0)
		{
			static void(*on_write)(uv_fs_t*) = [](uv_fs_t* write_req)
			{
				auto self = (WriteFile*)write_req->data;

				if (write_req->result < 0)
				{
					uv_fs_close(self->_loop->_loop.get(), self->_close->_fs.get(), self->_open->_fs.get()->result, [](uv_fs_t* close_req) {
						auto self = (WriteFile*)close_req->data;
						self->_ferr.getFeature<PSubroutine>()->execute(self->_ferr, {});
					});
				}
				else
				{
					auto data = self->_fwrite.getFeature<PSubroutine>()->execute(self->_fwrite, {});
					if (!data)
					{
						uv_fs_close(self->_loop->_loop.get(), self->_close->_fs.get(), self->_open->_fs.get()->result, [](uv_fs_t* close_req) {
							auto self = (WriteFile*)close_req->data;
							self->_ferr.getFeature<PSubroutine>()->execute(self->_fdone, {});
						});
					}
					else
					{
						if (data.isType<craft::lisp::library::Buffer>())
						{
							self->_b_last_buf = data.asType<craft::lisp::library::Buffer>();
							self->_last_buf.base = (char*)self->_b_last_buf->data().data();
							self->_last_buf.len = self->_b_last_buf->data().size();
						}
						else if (data.isType<std::string>())
						{
							self->_s_last_buf = data.asType<std::string>();
							self->_last_buf.base = (char*)self->_s_last_buf->data();
							self->_last_buf.len = self->_s_last_buf->size();
						}
						else if (data.hasFeature<PStringer>())
						{
							self->_s_last_buf = instance<std::string>::make(fmt::format("<{0}>({1})", data.typeId().toString(), (void*)data.get()));
							self->_last_buf.base = (char*)self->_s_last_buf->data();
							self->_last_buf.len = self->_s_last_buf->size();
						}
						else
						{
							self->_s_last_buf = instance<std::string>::make("");
							self->_last_buf.base = (char*)self->_s_last_buf->data();
							self->_last_buf.len = self->_s_last_buf->size();
						}
						uv_fs_write(self->_loop->_loop.get(), self->_write->_fs.get(), self->_open->_fs.get()->result, &self->_last_buf, 1, -1, on_write);
					}
				}
			};
			auto data = self->_fwrite.getFeature<PSubroutine>()->execute(self->_fwrite, {});

			if (!data)
			{
				uv_fs_close(self->_loop->_loop.get(), self->_close->_fs.get(), self->_open->_fs.get()->result, [](uv_fs_t* close_req) {
					auto self = (WriteFile*)close_req->data;
					self->_ferr.getFeature<PSubroutine>()->execute(self->_fdone, {});
				});
			}
			else
			{
				if (data.isType<craft::lisp::library::Buffer>())
				{
					self->_b_last_buf = data.asType<craft::lisp::library::Buffer>();
					self->_last_buf.base = (char*)self->_b_last_buf->data().data();
					self->_last_buf.len = self->_b_last_buf->data().size();
				}
				else if (data.isType<std::string>())
				{
					self->_s_last_buf = data.asType<std::string>();
					self->_last_buf.base = (char*)self->_s_last_buf->data();
					self->_last_buf.len = self->_s_last_buf->size();
				}
				else if (data.hasFeature<PStringer>())
				{
					self->_s_last_buf = instance<std::string>::make(data.getFeature<PStringer>()->toString(data));
					self->_last_buf.base = (char*)self->_s_last_buf->data();
					self->_last_buf.len = self->_s_last_buf->size();
				}
				else
				{
					self->_s_last_buf = instance<std::string>::make(fmt::format("<{0}>({1})", data.typeId().toString(), (void*)data.get()));
					self->_last_buf.base = (char*)self->_s_last_buf->data();
					self->_last_buf.len = self->_s_last_buf->size();
				}
				uv_fs_write(self->_loop->_loop.get(), self->_write->_fs.get(), self->_open->_fs.get()->result, &self->_last_buf, 1, -1, on_write);
			}
			
			
		}
		else
		{
			self->_ferr.getFeature<PSubroutine>()->execute(self->_ferr, {});
			// Memory Was Not Allocated Here
		}
	});
}

WriteFile::~WriteFile()
{
	uv_fs_req_cleanup(_open->_fs.get());
	uv_fs_req_cleanup(_write->_fs.get());
	uv_fs_req_cleanup(_close->_fs.get());
}


FileOp::FileOp(instance<> f)
	: _f(f)
	, _op(instance<Fs>::make())
{

}

FileOp::~FileOp()
{
	uv_fs_req_cleanup(_op->_fs.get());
}

void cultlang::uv::make_fs_ops(craft::types::instance<craft::lisp::Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	semantics->builtin_implementMultiMethod("uv/read", [](instance<cultlang::uv::Loop> l, instance<std::string> s, instance<lisp::PSubroutine> err, instance<lisp::PSubroutine> read, instance<lisp::PSubroutine> done) {
		return instance<ReadFile>::make(l, s, instance<int64_t>::make(1024), err, read, done);
	});

	semantics->builtin_implementMultiMethod("uv/mkdir", 
		[](instance<Loop> l, instance<std::string> s, instance <craft::lisp::Function> cb) 
	{
		auto res = instance<FileOp>::make(cb);
		res->_op->_fs.get()->data = res.get();
		uv_fs_mkdir(l->_loop.get(), res->_op->_fs.get(), s->c_str(), 0, [](uv_fs_t* f)
		{
			auto self = (FileOp*)f->data;
			auto res = instance<int64_t>::make(f->result);
			self->_f.getFeature<PSubroutine>()->execute(self->_f, { res });
		});

		return res;
	});

	semantics->builtin_implementMultiMethod("uv/rmdir",
		[](instance<Loop> l, instance<std::string> s, instance <craft::lisp::Function> cb)
	{
		auto res = instance<FileOp>::make(cb);
		res->_op->_fs.get()->data = res.get();
		uv_fs_rmdir(l->_loop.get(), res->_op->_fs.get(), s->c_str(), [](uv_fs_t* f)
		{
			auto self = (FileOp*)f->data;
			auto res = instance<int64_t>::make(f->result);
			self->_f.getFeature<PSubroutine>()->execute(self->_f, { res });
		});

		return res;
	});

	semantics->builtin_implementMultiMethod("uv/access",
		[](instance<Loop> l, instance<std::string> s, instance <craft::lisp::Function> cb)
	{
		auto res = instance<FileOp>::make(cb);
		res->_op->_fs.get()->data = res.get();
		uv_fs_access(l->_loop.get(), res->_op->_fs.get(), s->c_str(), 0, [](uv_fs_t* f)
		{
			auto self = (FileOp*)f->data;
			auto res = instance<int64_t>::make(f->result);
			self->_f.getFeature<PSubroutine>()->execute(self->_f, { res });
		});

		return res;
	});
}
