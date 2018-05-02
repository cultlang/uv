#include "uv/common.h"

#include "uv/all.h"
#include "lisp/library/system/prelude.h"
#include "lisp/semantics/cult/calling.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace uvw;


typedef instance<uvw::Loop> t_lop;
typedef instance<std::string> t_str;
typedef instance<int64_t> t_i64;
typedef instance<craft::lisp::PSubroutine> t_sub;

typedef uvw::ErrorEvent uerr;
typedef uvw::CloseEvent uclos;

typedef uvw::FsEvent<uvw::details::UVFsType::OPEN> uft_open;
typedef uvw::FsEvent<uvw::details::UVFsType::CLOSE> uft_close;
typedef uvw::FsEvent<uvw::details::UVFsType::READ> uft_read;
typedef uvw::FsEvent<uvw::details::UVFsType::WRITE> uft_write;
typedef uvw::FsEvent<uvw::details::UVFsType::SENDFILE> uft_send;
typedef uvw::FsEvent<uvw::details::UVFsType::STAT> uft_stat;
typedef uvw::FsEvent<uvw::details::UVFsType::FSTAT> uft_fstat;
typedef uvw::FsEvent<uvw::details::UVFsType::LSTAT> uft_lstat;
typedef uvw::FsEvent<uvw::details::UVFsType::SCANDIR> uft_scandir;
typedef uvw::FsEvent<uvw::details::UVFsType::READLINK> uft_readl;
typedef uvw::FsEvent<uvw::details::UVFsType::MKDIR> uft_mkdir;
typedef uvw::FsEvent<uvw::details::UVFsType::MKDTEMP> uft_mkdtmp;
typedef uvw::FsEvent<uvw::details::UVFsType::RMDIR> uft_rmdir;

namespace _impl {
	class StreamContext
		: public types::Object
	{
		CRAFT_OBJECT_DECLARE(_impl::StreamContext);
	public:
		size_t read;
		instance<PSubroutine> onerr;
		instance<PSubroutine> ondata;
		instance<PSubroutine> ondone;
		instance<> ctx;

		StreamContext(instance<PSubroutine> e, instance<PSubroutine> r, instance<PSubroutine> d)
		{
			read = 0;
			onerr = e;
			ondata = r;
			ondone = d;
		}
	};
}

CRAFT_DEFINE(_impl::StreamContext)
{
	_.defaults();
}


void cultlang::uv::make_fs_bindings(craft::types::instance<craft::lisp::Module> m)
{
	auto semantics = m->require<CultSemantics>();
#pragma region uv/mkdir
	semantics->builtin_implementMultiMethod("uv/mkdir",
		[](t_lop loop, t_str dir, t_sub f, t_sub s)
	{
		auto request = loop->resource<uvw::FsReq>();

		request->data(instance<PromisePair>::make(f, s));

		request->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->fail;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});

		request->on<uft_mkdir>([](uft_mkdir &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->success;
			sub->execute(sub, { instance<uft_mkdir>::make(ev.path), hndl.craft_instance() });
		});

		request->mkdir(*dir, 0755);

		return request;
	});
	semantics->builtin_implementMultiMethod("uv/rmdir",
	[](t_lop loop, t_str dir, t_sub f, t_sub s)
	{
		auto request = loop->resource<uvw::FsReq>();

		request->data(instance<PromisePair>::make(f, s));

		request->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->fail;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});

		request->on<uft_rmdir>([](uft_rmdir &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->success;
			sub->execute(sub, { instance<uft_rmdir>::make(ev.path), hndl.craft_instance() });
		});

		request->rmdir(*dir);

		return request;
	});
	semantics->builtin_implementMultiMethod("uv/mkdirtmp",
		[](t_lop loop, t_str dir, t_sub f, t_sub s)
	{
		auto request = loop->resource<uvw::FsReq>();

		request->data(instance<PromisePair>::make(f, s));

		request->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->fail;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});

		request->on<uft_mkdtmp>([](uft_mkdtmp &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->success;
			sub->execute(sub, { instance<uft_mkdtmp>::make(ev.path), hndl.craft_instance() });
		});

		request->mkdtemp(*dir);

		return request;
	});
#pragma endregion Directory Ops

#pragma region uv/stat
	semantics->builtin_implementMultiMethod("uv/stat",
		[](t_lop loop, t_str filename, t_sub f, t_sub s)
	{
		auto fileReq = loop->resource<uvw::FileReq>();
		auto fsReq = loop->resource<uvw::FsReq>();

		fileReq->data(instance<PromisePair>::make(f, s));
		fsReq->data(instance<PromisePair>::make(f, s));

		fileReq->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->fail;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});
		fsReq->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->fail;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});

		fsReq->on<uft_stat>([](uft_stat &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->success;
			sub->execute(sub, { instance<uft_stat>::make(ev.path, ev.stat), hndl.craft_instance() });
		});

		fileReq->on<uft_close>([fsReq, filename](auto &ev, auto &hndl) mutable {
			fsReq->stat(*filename);
		});

		fileReq->on<uft_open>([](const auto &, auto &request) {
			request.close();
		});

		auto flags = uvw::Flags<uvw::FileReq::FileOpen>::from<uvw::FileReq::FileOpen::RDONLY>();
		fileReq->open(*filename, flags, 0644);

		return fsReq;
	});

	semantics->builtin_implementMultiMethod("uv/fstat",
		[](t_lop loop, t_str filename, t_sub f, t_sub s)
	{
		auto fileReq = loop->resource<uvw::FileReq>();
		auto fsReq = loop->resource<uvw::FsReq>();

		fileReq->data(instance<PromisePair>::make(f, s));
		fsReq->data(instance<PromisePair>::make(f, s));

		fileReq->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->fail;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});
		fsReq->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->fail;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});

		fsReq->on<uft_fstat>([](auto &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->success;
			sub->execute(sub, { instance<uft_fstat>::make(ev.path, ev.stat), hndl.craft_instance() });
		});

		fileReq->on<uft_close>([fsReq, filename](auto &ev, auto &hndl) mutable {
			fsReq->stat(*filename);
		});

		fileReq->on<uft_open>([](const auto &, auto &request) {
			request.close();
		});

		auto flags = uvw::Flags<uvw::FileReq::FileOpen>::from<uvw::FileReq::FileOpen::RDONLY>();
		fileReq->open(*filename, flags, 0644);

		return fsReq;
	});

	semantics->builtin_implementMultiMethod("uv/lstat",
		[](t_lop loop, t_str filename, t_sub f, t_sub s)
	{
		auto fileReq = loop->resource<uvw::FileReq>();
		auto fsReq = loop->resource<uvw::FsReq>();

		fileReq->data(instance<PromisePair>::make(f, s));
		fsReq->data(instance<PromisePair>::make(f, s));

		fileReq->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->fail;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});
		fsReq->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->fail;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});

		fsReq->on<uft_fstat>([](auto &ev, auto &hndl) {
			auto sub = hndl.data<PromisePair>()->success;
			sub->execute(sub, { instance<uft_lstat>::make(ev.path, ev.stat), hndl.craft_instance() });
		});

		fileReq->on<uft_close>([fsReq, filename](auto &ev, auto &hndl) mutable {
			fsReq->stat(*filename);
		});

		fileReq->on<uft_open>([](const auto &, auto &request) {
			request.close();
		});

		auto flags = uvw::Flags<uvw::FileReq::FileOpen>::from<uvw::FileReq::FileOpen::RDONLY>();
		fileReq->open(*filename, flags, 0644);

		return fsReq;
	});


	// Stat Retrival Ops
	typedef instance<uvw::FsEvent<uvw::FsReq::Type::STAT>> t_fse_st;
	semantics->builtin_implementMultiMethod("uv/stat/dev", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_dev); });
	semantics->builtin_implementMultiMethod("uv/stat/mode", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_mode); });
	semantics->builtin_implementMultiMethod("uv/stat/nlink", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_nlink); });
	semantics->builtin_implementMultiMethod("uv/stat/uid", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_uid); });
	semantics->builtin_implementMultiMethod("uv/stat/gid", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_gid); });
	semantics->builtin_implementMultiMethod("uv/stat/rdev", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_rdev); });
	semantics->builtin_implementMultiMethod("uv/stat/ino", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_ino); });
	semantics->builtin_implementMultiMethod("uv/stat/size", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_size); });
	semantics->builtin_implementMultiMethod("uv/stat/blksize", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_blksize); });
	semantics->builtin_implementMultiMethod("uv/stat/blocks", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_blocks); });
	semantics->builtin_implementMultiMethod("uv/stat/flags", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_flags); });
	semantics->builtin_implementMultiMethod("uv/stat/gen", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_gen); });

	semantics->builtin_implementMultiMethod("uv/stat/atim/sec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_atim.tv_sec); });
	semantics->builtin_implementMultiMethod("uv/stat/atim/nsec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_atim.tv_nsec); });

	semantics->builtin_implementMultiMethod("uv/stat/mtim/sec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_mtim.tv_sec); });
	semantics->builtin_implementMultiMethod("uv/stat/mtim/nsec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_mtim.tv_nsec); });

	semantics->builtin_implementMultiMethod("uv/stat/ctim/sec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_ctim.tv_sec); });
	semantics->builtin_implementMultiMethod("uv/stat/ctim/nsec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_ctim.tv_nsec); });

	semantics->builtin_implementMultiMethod("uv/stat/birthtim/sec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_birthtim.tv_sec); });
	semantics->builtin_implementMultiMethod("uv/stat/brithtim/nsec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_birthtim.tv_nsec); });

#pragma endregion Stat Stuff

	semantics->builtin_implementMultiMethod("uv/read",
		[](t_lop loop, t_str filename, t_i64 b, t_sub f, t_sub s, t_sub d)
	{
		auto fileReq = loop->resource<uvw::FileReq>();

		fileReq->data(instance<_impl::StreamContext>::make(f, s, d));

		fileReq->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<_impl::StreamContext>()->onerr;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});
		
		fileReq->on<uclos>([](uclos &ev, auto &hndl) {
			auto sub = hndl.data<_impl::StreamContext>()->ondone;
			sub->execute(sub, { hndl.craft_instance() });
		});
		fileReq->on<uft_read>([b](uft_read &ev, uvw::FileReq &hndl) mutable {
			auto ctx = hndl.data<_impl::StreamContext>();
			auto sub = ctx->ondata;
			if (ev.size == 0)
			{
				hndl.close();
				return;
			}
			auto ret = sub->execute(sub, { instance<uft_read>::make(ev.path, std::move(ev.data), ev.size), hndl.craft_instance() });
			
			ctx->read += ev.size;
			hndl.read(ctx->read, uint32_t(*b));
		});

		fileReq->on<uft_open>([b](const auto &, uvw::FileReq &hndl) {
			hndl.read(0, uint32_t(*b));
		});

		auto flags = uvw::Flags<uvw::FileReq::FileOpen>::from<uvw::FileReq::FileOpen::RDONLY>();
		fileReq->open(*filename, flags, 0644);

		return fileReq;
	});

	semantics->builtin_implementMultiMethod("uv/write",
		[](t_lop loop, t_str filename, t_i64 b, t_sub f, t_sub s, t_sub d)
	{
		auto fileReq = loop->resource<uvw::FileReq>();

		fileReq->data(instance<_impl::StreamContext>::make(f, s, d));

		fileReq->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<_impl::StreamContext>()->onerr;
			sub->execute(sub, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
		});

		fileReq->on<uclos>([](uclos &ev, auto &hndl) {
			auto sub = hndl.data<_impl::StreamContext>()->ondone;
			sub->execute(sub, { hndl.craft_instance() });
		});
		fileReq->on<uft_write>([b](uft_write &ev, uvw::FileReq &hndl) mutable {
			auto ctx = hndl.data<_impl::StreamContext>();
			auto sub = ctx->ondata;
			ctx->ctx = sub->execute(sub, { hndl.craft_instance() });

			if (!ctx->ctx)
			{
				hndl.close();
			}
			else if (ctx->ctx.isType<std::string>())
			{
				auto& str = ctx->ctx.asType<std::string>();
				hndl.write(str->data(), uint32_t(str->size()), ctx->read);
				ctx->read = str->size();
			}
			else if (ctx->ctx.isType<lisp::library::Buffer>())
			{
				auto& buf = ctx->ctx.asType<lisp::library::Buffer>();

				hndl.write((char*)buf->data().data(), uint32_t(buf->data().size()), ctx->read);
				ctx->read = uint32_t(*buf->size());
			}
			else if (ctx->ctx.hasFeature<PStringer>())
			{
				auto buf = instance<std::string>::make(ctx->ctx.getFeature<types::PStringer>()->toString(ctx->ctx));
				hndl.write(buf->data(), uint32_t(buf->size()), ctx->read);
				ctx->read = buf->size();
				ctx->ctx = buf;
			}
		});

		fileReq->on<uft_open>([b](const auto &, uvw::FileReq &hndl) {
			auto ctx = hndl.data<_impl::StreamContext>();
			auto sub = ctx->ondata;
			ctx->ctx = sub->execute(sub, { hndl.craft_instance() });

			if (!ctx->ctx)
			{
				hndl.close();
			}
			else if (ctx->ctx.isType<std::string>())
			{
				auto& str = ctx->ctx.asType<std::string>();
				hndl.write(str->data(), uint32_t(str->size()), 0);
				ctx->read = str->size();
			}
			else if (ctx->ctx.isType<lisp::library::Buffer>())
			{
				auto& buf = ctx->ctx.asType<lisp::library::Buffer>();
				
				hndl.write((char*)buf->data().data(), uint32_t(buf->data().size()), 0);
				ctx->read = uint32_t(*buf->size());
			}
			else if (ctx->ctx.hasFeature<PStringer>())
			{
				auto buf = instance<std::string>::make(ctx->ctx.getFeature<types::PStringer>()->toString(ctx->ctx));
				hndl.write(buf->data(), uint32_t(buf->size()), 0);
				ctx->read = buf->size();
				ctx->ctx = buf;
			}
		});

		auto flags = uvw::Flags<uvw::FileReq::FileOpen>::from<uvw::FileReq::FileOpen::RDONLY>();
		fileReq->open(*filename, flags, 0644);

		return fileReq;
	});
}