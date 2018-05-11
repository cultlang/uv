#include "uv/common.h"

#include "uv/all.h"
#include "lisp/library/system/prelude.h"
#include "lisp/semantics/cult/calling.h"
#include "context.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace uvw;
using namespace cultlang;
using namespace cultlang::uv;

CRAFT_DEFINE(cultlang::uv::FileRequestContext)
{
	_.defaults();
}

uv::FileRequestContext::FileRequestContext()
{

}

typedef instance<uvw::Loop> t_lop;
typedef instance<std::string> t_str;
typedef instance<int64_t> t_i64;
typedef instance<int64_t> t_i32;
typedef instance<uint8_t> t_u8;
typedef instance<craft::lisp::PSubroutine> t_sub;
typedef instance<uv::FileRequestContext> t_fictx;
typedef instance<uvw::FsReq> t_fsreq;
typedef instance<uvw::FileReq> t_fireq;

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

#define lMM semantics->builtin_implementMultiMethod
#define uVF "uv/fs"

void cultlang::uv::make_fs_bindings(craft::types::instance<craft::lisp::Module> m)
{
	auto semantics = m->require<CultSemantics>();
	
#pragma region ctx
	lMM(uVF"/file/ctx", []() {return t_fictx::make();});

	lMM(uVF"ctx/error", [](t_fictx s) { return s->onerr; });
	lMM(uVF"ctx/error", [](t_fictx s, t_sub p) { s->onerr = p; });

	lMM(uVF"ctx/close", [](t_fictx s) { return s->onclose; });
	lMM(uVF"ctx/close", [](t_fictx s, t_sub p) { s->onclose = p; });

	lMM(uVF"ctx/read", [](t_fictx s) { return s->onread; });
	lMM(uVF"ctx/read", [](t_fictx s, t_sub p) { s->onread = p; });

	lMM(uVF"ctx/write", [](t_fictx s) { return s->onwrite; });
	lMM(uVF"ctx/write", [](t_fictx s, t_sub p) { s->onwrite = p; });

	lMM(uVF"ctx/send", [](t_fictx s) { return s->onsend; });
	lMM(uVF"ctx/send", [](t_fictx s, t_sub p) { s->onsend = p; });

	lMM(uVF"ctx/stat", [](t_fictx s) { return s->onstat; });
	lMM(uVF"ctx/stat", [](t_fictx s, t_sub p) { s->onstat = p; });

	lMM(uVF"ctx/fstat", [](t_fictx s) { return s->onfstat; });
	lMM(uVF"ctx/fstat", [](t_fictx s, t_sub p) { s->onfstat = p; });

	lMM(uVF"ctx/lstat", [](t_fictx s) { return s->onlstat; });
	lMM(uVF"ctx/lstat", [](t_fictx s, t_sub p) { s->onlstat = p; });

	lMM(uVF"ctx/sync", [](t_fictx s) { return s->onsync; });
	lMM(uVF"ctx/sync", [](t_fictx s, t_sub p) { s->onsync = p; });

	lMM(uVF, [](t_lop l, t_fictx ctx) {
		auto request = l->resource<uvw::FileReq>();

		request->on<uerr>([](uerr &ev, auto &hndl) {
			auto sub = hndl.data<uv::FileRequestContext>();
			if(sub->onerr)
			{
				sub->onerr->execute(sub->onerr, { instance<uerr>::make(ev.code()), hndl.craft_instance() });
			}
		});
		request->on<uclos>([](uclos &ev, auto &hndl) {
			auto sub = hndl.data<uv::FileRequestContext>();
			if (sub->onclose)
			{
				sub->onclose->execute(sub->onclose, { instance<uclos>::make(), hndl.craft_instance() });
			}
		});
		request->on<uft_open>([](uft_open &ev, auto &hndl) {
			auto sub = hndl.data<uv::FileRequestContext>();
			if (sub->onopen)
			{
				sub->onopen->execute(sub->onopen, { instance<uft_open>::make(ev.path), hndl.craft_instance() });
			}
		});
		request->on<uft_write>([](uft_write &ev, auto &hndl) {
			auto sub = hndl.data<uv::FileRequestContext>();
			if (sub->onwrite)
			{
				sub->onwrite->execute(sub->onwrite, { instance<uft_write>::make(ev.path, ev.size), hndl.craft_instance() });
			}
		});
		request->on<uft_send>([](uft_send &ev, auto &hndl) {
			auto sub = hndl.data<uv::FileRequestContext>();
			if (sub->onsend)
			{
				sub->onsend->execute(sub->onsend, { instance<uft_send>::make(ev.path, ev.size), hndl.craft_instance() });
			}
		});
		request->on<uft_stat>([](uft_stat &ev, auto &hndl) {
			auto sub = hndl.data<uv::FileRequestContext>();
			if (sub->onstat)
			{
				sub->onstat->execute(sub->onstat, { instance<uft_stat>::make(ev.path, ev.stat), hndl.craft_instance() });
			}
		});
		request->on<uft_fstat>([](uft_fstat &ev, auto &hndl) {
			auto sub = hndl.data<uv::FileRequestContext>();
			if (sub->onfstat)
			{
				sub->onfstat->execute(sub->onfstat, { instance<uft_fstat>::make(ev.path, ev.stat), hndl.craft_instance() });
			}
		});
		request->on<uft_lstat>([](uft_lstat &ev, auto &hndl) {
			auto sub = hndl.data<uv::FileRequestContext>();
			if (sub->onlstat)
			{
				sub->onlstat->execute(sub->onlstat, { instance<uft_lstat>::make(ev.path, ev.stat), hndl.craft_instance() });
			}
		});
		
	});
	// Stat Retrival Ops
	typedef instance<uvw::FsEvent<uvw::FsReq::Type::STAT>> t_fse_st;
	lMM(uVF"/stat/dev", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_dev); });
	lMM(uVF"/stat/mode", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_mode); });
	lMM(uVF"/stat/nlink", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_nlink); });
	lMM(uVF"/stat/uid", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_uid); });
	lMM(uVF"/stat/gid", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_gid); });
	lMM(uVF"/stat/rdev", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_rdev); });
	lMM(uVF"/stat/ino", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_ino); });
	lMM(uVF"/stat/size", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_size); });
	lMM(uVF"/stat/blksize", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_blksize); });
	lMM(uVF"/stat/blocks", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_blocks); });
	lMM(uVF"/stat/flags", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_flags); });
	lMM(uVF"/stat/gen", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_gen); });

	lMM(uVF"/stat/atim/sec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_atim.tv_sec); });
	lMM(uVF"/stat/atim/nsec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_atim.tv_nsec); });

	lMM(uVF"/stat/mtim/sec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_mtim.tv_sec); });
	lMM(uVF"/stat/mtim/nsec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_mtim.tv_nsec); });

	lMM(uVF"/stat/ctim/sec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_ctim.tv_sec); });
	lMM(uVF"/stat/ctim/nsec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_ctim.tv_nsec); });

	lMM(uVF"/stat/birthtim/sec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_birthtim.tv_sec); });
	lMM(uVF"/stat/birthtim/nsec", [](t_fse_st s) {instance<uint64_t>::make(s->stat.st_birthtim.tv_nsec); });

	lMM(uVF"/chmod", [](t_fireq r, t_i32 p) {r->chmod(*p); });
	lMM(uVF"/chmodSync", [](t_fireq r, t_i32 p) {r->chmodSync(*p); });
	lMM(uVF"/chmod", [](t_fireq r, t_u8 p, t_u8 g) {r->chown(*p, *g); });
	lMM(uVF"/chmodSync", [](t_fireq r, t_u8 p, t_u8 g) {r->chownSync(*p, *g); });
	lMM(uVF"/close", [](t_fireq r) {r->close(); });
	lMM(uVF"/closeSync", [](t_fireq r) {r->closeSync(); });
	lMM(uVF"/datasync", [](t_fireq r) {r->datasync(); });
	lMM(uVF"/datasyncSync", [](t_fireq r) {r->datasyncSync(); });

	lMM(uVF"/open", [](t_fireq r, t_str p, t_i32 f, t_i32 m) {r->open(*p, *m, *m); });

	lMM(uVF"/openflags/APPEND", []() { return t_i32::make((int32_t)FileReq::FileOpen::APPEND); });
	lMM(uVF"/openflags/CREAT", []() { return t_i32::make((int32_t)FileReq::FileOpen::CREAT); });
	lMM(uVF"/openflags/DIRECT", []() { return t_i32::make((int32_t)FileReq::FileOpen::DIRECT); });
	lMM(uVF"/openflags/DIRECTORY", []() { return t_i32::make((int32_t)FileReq::FileOpen::DIRECTORY); });
	lMM(uVF"/openflags/DSYNC", []() { return t_i32::make((int32_t)FileReq::FileOpen::DSYNC); });
	lMM(uVF"/openflags/EXCL", []() { return t_i32::make((int32_t)FileReq::FileOpen::EXCL); });
	lMM(uVF"/openflags/EXLOCK", []() { return t_i32::make((int32_t)FileReq::FileOpen::EXLOCK); });
	lMM(uVF"/openflags/NOATIME", []() { return t_i32::make((int32_t)FileReq::FileOpen::NOATIME); });
	lMM(uVF"/openflags/NOCTTY", []() { return t_i32::make((int32_t)FileReq::FileOpen::NOCTTY); });
	lMM(uVF"/openflags/NOFOLLOW", []() { return t_i32::make((int32_t)FileReq::FileOpen::NOFOLLOW); });
	lMM(uVF"/openflags/NONBLOCK", []() { return t_i32::make((int32_t)FileReq::FileOpen::NONBLOCK); });
	lMM(uVF"/openflags/RANDOM", []() { return t_i32::make((int32_t)FileReq::FileOpen::RANDOM); });
	lMM(uVF"/openflags/RDONLY", []() { return t_i32::make((int32_t)FileReq::FileOpen::RDONLY); });
	lMM(uVF"/openflags/RDWR", []() { return t_i32::make((int32_t)FileReq::FileOpen::RDWR); });
	lMM(uVF"/openflags/SEQUENTIAL", []() { return t_i32::make((int32_t)FileReq::FileOpen::SEQUENTIAL); });
	lMM(uVF"/openflags/SHORT_LIVED", []() { return t_i32::make((int32_t)FileReq::FileOpen::SHORT_LIVED); });
	lMM(uVF"/openflags/SYMLINK", []() { return t_i32::make((int32_t)FileReq::FileOpen::SYMLINK); });
	lMM(uVF"/openflags/SYNC", []() { return t_i32::make((int32_t)FileReq::FileOpen::SYNC); });
	lMM(uVF"/openflags/TEMPORARY", []() { return t_i32::make((int32_t)FileReq::FileOpen::TEMPORARY); });
	lMM(uVF"/openflags/TRUNC", []() { return t_i32::make((int32_t)FileReq::FileOpen::WRONLY); });

	lMM(uVF"/openflags", [](types::VarArgs<instance<>> args) { 
		int32_t res;
		for (auto i : args.args)
		{
			if (!i.isType<int32_t>()) throw stdext::exception("Arguments must be int32s");
			res = res | *i.asType<int32_t>();
		}
		return instance<int32_t>::make(res);
	});

	lMM(uVF"/read", [](t_fireq r, t_i64 f, t_i64 m) {r->read(*f, *m); });
	lMM(uVF"/stat", [](t_fireq r) {r->stat(); });
	lMM(uVF"/sync", [](t_fireq r) {r->sync(); });
	lMM(uVF"/syncSync", [](t_fireq r) {r->syncSync(); });
	lMM(uVF"/truncate", [](t_fireq r, t_i64 f) {r->truncate(*f); });
	lMM(uVF"/truncateSync", [](t_fireq r, t_i64 f) {r->truncateSync(*f); });
	lMM(uVF"/write", [](t_fireq r, t_str d, t_i64 o) {r->write(d->data(), d->size(), *o); });
}
