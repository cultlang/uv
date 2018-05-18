#include "uv/common.h"

#include "all.h"

#include "cult/context.h"


#include "lisp/semantics/cult/calling.h"

#include "uvw.hpp"

using namespace craft;
using namespace craft::lisp;
using namespace craft::types;
using namespace cultlang::uv;

namespace _impl {
	std::string stat_str(uvw::Stat s)
	{
		std::ostringstream res;
		res << "{";
		res << "dev: " << s.st_dev << ", ";
		res << "mode:" << s.st_mode << ", ";
		res << "nlink: " << s.st_nlink << ", ";
		res << "uid: " << s.st_uid << ", ";
		res << "gid: " << s.st_gid << ", ";
		res << "rdev: " << s.st_rdev << ", ";
		res << "ino: " << s.st_ino << ", ";
		res << "size: " << s.st_size << ", ";
		res << "blksize: " << s.st_blksize << ", ";
		res << "blocks: " << s.st_blocks << ", ";
		res << "flags: " << s.st_flags << ", ";
		res << "gen: " << s.st_gen << ", ";
		res << "atim: " << s.st_atim.tv_sec << "." << s.st_atim.tv_nsec << ", ";
		res << "mtim: " << s.st_mtim.tv_sec << "." << s.st_mtim.tv_nsec << ", ";
		res << "ctim: " << s.st_ctim.tv_sec << "." << s.st_ctim.tv_nsec << ", ";
		res << "birthtim: " << s.st_birthtim.tv_sec << "." << s.st_birthtim.tv_nsec;
		res << "}";

		return res.str();
	}

	std::string stat_repr(uvw::Stat s)
	{
		std::ostringstream res;
		res << "(map ";
		res << "\"dev \"" << s.st_dev << " ";
		res << "\"mode \"" << s.st_mode << " ";
		res << "\"nlink \"" << s.st_nlink << " ";
		res << "\"uid \"" << s.st_uid << " ";
		res << "\"gid \"" << s.st_gid << " ";
		res << "\"rdev \"" << s.st_rdev << " ";
		res << "\"ino \"" << s.st_ino << " ";
		res << "\"size \"" << s.st_size << " ";
		res << "\"blksize \"" << s.st_blksize << " ";
		res << "\"blocks \"" << s.st_blocks << " ";
		res << "\"flags \"" << s.st_flags << " ";
		res << "\"gen \"" << s.st_gen << " ";
		res << "\"atim \"" << s.st_atim.tv_sec << "." << s.st_atim.tv_nsec << " ";
		res << "\"mtim \"" << s.st_mtim.tv_sec << "." << s.st_mtim.tv_nsec << " ";
		res << "\"ctim \"" << s.st_ctim.tv_sec << "." << s.st_ctim.tv_nsec << " ";
		res << "\"birthtim \"" << s.st_birthtim.tv_sec << "." << s.st_birthtim.tv_nsec;
		res << ")";

		return res.str();
	}
}


#pragma region UvwWrapperTypes  
// async.hpp
CRAFT_TYPE_DEFINE(uvw::AsyncEvent)
{
	_.add<GraphPropertyName>("uv/AsyncEvent");
	_.add<GraphPropertyCppName>("uvw::AsyncEvent");
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uvw::AsyncEvent> _this) { return "uv.AsyncEvent"; });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uvw::AsyncEvent> _this) { return "uv.AsyncEvent"; });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
CRAFT_DEFINE(uvw::AsyncHandle)
{
	_.defaults();
}

// check.hpp
CRAFT_TYPE_DEFINE(uvw::CheckEvent)
{
	_.add<GraphPropertyName>("uv/CheckEvent");
	_.add<GraphPropertyCppName>("uvw::CheckEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::CheckEvent> _this) { return "uv.CheckEvent"; });
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::CheckEvent> _this) { return "uv.CheckEvent"; });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
CRAFT_DEFINE(uvw::CheckHandle)
{
	_.defaults();
}

//dns.hpp
CRAFT_TYPE_DEFINE(uvw::AddrInfoEvent)
{
	_.add<GraphPropertyName>("uv/AddrInfoEvent");
	_.add<GraphPropertyCppName>("uvw::AddrInfoEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::AddrInfoEvent> _this) { return "uv.AddrInfoEvent"; });
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::AddrInfoEvent> _this) { return "(uv.AddrInfoEvent )"; });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
CRAFT_TYPE_DEFINE(uvw::NameInfoEvent)
{
	_.add<GraphPropertyName>("uv/NameInfoEvent");
	_.add<GraphPropertyCppName>("uvw::AddrInfoEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::NameInfoEvent> _this) { return fmt::format("{0}:{1}", _this->service, _this->hostname); });
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::NameInfoEvent> _this) { return fmt::format("(uv/AddrInfoEvent {0} {1})", _this->hostname, _this->service); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_DEFINE(uvw::GetAddrInfoReq)
{
	_.defaults();
}
CRAFT_DEFINE(uvw::GetNameInfoReq)
{
	_.defaults();
}

// Emitter.hpp
CRAFT_TYPE_DEFINE(uvw::ErrorEvent)
{
	_.add<GraphPropertyName>("uv/ErrorEvent");
	_.add<GraphPropertyCppName>("uvw::AddrInfoEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::ErrorEvent> _this) { return fmt::format("{0} {1}", _this->name(), _this->what()); });
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::ErrorEvent> _this) { return fmt::format("(uv/ErrorEvent {0})", _this->code()); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

// fs.hpp
CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::OPEN>)
{
	_.add<GraphPropertyName>("uv/FsEvent/Open");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::OPEN>");
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();

}
CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::READ>)
{
	_.add<GraphPropertyName>("uv/FsEvent/Read");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::READ>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::READ>> _this) { 
		std::ostringstream res;
		res.write(_this->data.get(), _this->size);
		return res.str();
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::READ>> _this) {
			std::ostringstream res;
			res.write(_this->data.get(), _this->size);
			return res.str();
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::WRITE>)
{
	_.add<GraphPropertyName>("uv/FsEvent/Write");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::WriteEvent>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::WRITE>> _this) {
		return fmt::format("{0}<{1}> bytes", _this->path, _this->size);
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::WRITE>> _this) {
		return fmt::format("(uv/WriteEvent {0} {1})", _this->path, _this->size);
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::SENDFILE>)
{
	_.add<GraphPropertyName>("uv/FsEvent/SendFile");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::SENDFILE>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::SENDFILE>> _this) {
		return fmt::format("{0}<{1}> bytes", _this->path, _this->size);
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::SENDFILE>> _this) {
		return fmt::format("(uv/WriteEvent {0} {1})", _this->path, _this->size);
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::STAT>)
{
	_.add<GraphPropertyName>("uv/FsEvent/Stat");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::STAT>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::STAT>> _this) {
		std::ostringstream res;
		res << _this->path << ' ' << _impl::stat_str(_this->stat);
		return res.str();
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::STAT>> _this) {
		std::ostringstream res;

		res << "(map \"path\" " << _this->path << " stat " << _impl::stat_str(_this->stat);
		return res.str();
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::FSTAT>)
{
	_.add<GraphPropertyName>("uv/FsEvent/FStat");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::FSTAT>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::FSTAT>> _this) {
		std::ostringstream res;
		res << _this->path << ' ' << _impl::stat_str(_this->stat);
		return res.str();
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::FSTAT>> _this) {
		std::ostringstream res;

		res << "(map \"path\" " << _this->path << " stat " << _impl::stat_str(_this->stat);
		return res.str();
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::LSTAT>)
{
	_.add<GraphPropertyName>("uv/FsEvent/LStat");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::LSTAT>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::LSTAT>> _this) {
		std::ostringstream res;
		res << _this->path << ' ' << _impl::stat_str(_this->stat);
		return res.str();
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::LSTAT>> _this) {
		std::ostringstream res;

		res << "(map \"path\" " << _this->path << " stat " << _impl::stat_str(_this->stat);
		return res.str();
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::SCANDIR>)
{
	_.add<GraphPropertyName>("uv/FsEvent/ScanDir");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::SCANDIR>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::SCANDIR>> _this) {
		return fmt::format("{0}<{1}> bytes", _this->path, _this->size);
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::SCANDIR>> _this) {
		return fmt::format("(uv/ScanDir {0} {1})", _this->path, _this->size);
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::READLINK>)
{
	_.add<GraphPropertyName>("uv/FsEvent/ReadLink");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::READLINK>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::READLINK>> _this) {
		std::ostringstream res;
		res.write(_this->data, _this->size);
		return res.str();
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::READLINK>> _this) {
		std::ostringstream res;
		res.write(_this->data, _this->size);
		return res.str();
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::MKDIR>)
{
	_.add<GraphPropertyName>("uv/FsEvent/MkDir");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::MKDIR>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::MKDIR>> _this) {
		return std::string(_this->path);
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::MKDIR>> _this) {
		return fmt::format("(uv/FsEvent/MkDir {0)", _this->path);
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::MKDTEMP>)
{
	_.add<GraphPropertyName>("uv/FsEvent/MkDirTemp");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::MKDTEMP>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::MKDTEMP>> _this) {
		return std::string(_this->path);
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::MKDTEMP>> _this) {
		return fmt::format("(uv/FsEvent/MkDirTemp {0)", _this->path);
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_TYPE_DEFINE(uvw::FsEvent<uvw::details::UVFsType::RMDIR>)
{
	_.add<GraphPropertyName>("uv/FsEvent/RmDir");
	_.add<GraphPropertyCppName>("uvw::FsEvent<uvw::details::UVFsType::RMDIR>");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::RMDIR>> _this) {
		return std::string(_this->path);
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEvent<uvw::details::UVFsType::RMDIR>> _this) {
		return fmt::format("(uv/FsEvent/RmDir {0)", _this->path);
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
	_.defaults();
}

CRAFT_DEFINE(uvw::FileReq)
{
	_.defaults();
}

CRAFT_DEFINE(uvw::FsReq)
{
	_.defaults();
}

//fs_event.hpp
CRAFT_TYPE_DEFINE(uvw::FsEventEvent)
{
	_.add<GraphPropertyName>("uv/FsEventEvent");
	_.add<GraphPropertyCppName>("uvw::FsEventEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsEventEvent> _this) { 
		return fmt::format("{0} {1}", _this->filename, _this->flags == uvw::Flags<uvw::details::UVFsEvent>(uvw::details::UVFsEvent::CHANGE) ? "Change" : "Rename");
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsEventEvent> _this) { 
		return fmt::format("(uv/FsEventEvent {0} {1})", _this->filename, _this->flags == uvw::Flags<uvw::details::UVFsEvent>(uvw::details::UVFsEvent::CHANGE) ? "Change" : "Rename"); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_DEFINE(uvw::FsEventHandle)
{
	_.defaults();
}

//fs_event.hpp
CRAFT_TYPE_DEFINE(uvw::FsPollEvent)
{
	_.add<GraphPropertyName>("uv/FsPollEvent");
	_.add<GraphPropertyCppName>("uvw::FsPollEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::FsPollEvent> _this) {
		return fmt::format("Old: {0}, New: {1}", _impl::stat_str(_this->prev), _impl::stat_str(_this->curr));
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::FsPollEvent> _this) {
		return fmt::format("(uv/FsPollEvent {0} {1})", _impl::stat_repr(_this->prev), _impl::stat_repr(_this->curr));
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_DEFINE(uvw::FsPollHandle)
{
	_.defaults();
}

// handle.hpp
CRAFT_TYPE_DEFINE(uvw::CloseEvent)
{
	_.add<GraphPropertyName>("uv/CloseEvent");
	_.add<GraphPropertyCppName>("uvw::CloseEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::CloseEvent> _this) {
		return "CloseEvent";
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::CloseEvent> _this) {
		return "(uv/CloseEvent)";
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

// idle.hpp
CRAFT_TYPE_DEFINE(uvw::IdleEvent)
{
	_.add<GraphPropertyName>("uv/IdleEvent");
	_.add<GraphPropertyCppName>("uvw::IdleEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::IdleEvent> _this) {
		return "IdleEvent";
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::IdleEvent> _this) {
		return "(uv/IdleEvent)";
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_DEFINE(uvw::IdleHandle)
{
	_.defaults();
}

// loop.hpp
CRAFT_DEFINE(uvw::Loop)
{
	_.defaults();
}

// pipe.hpp
CRAFT_DEFINE(uvw::PipeHandle)
{
	_.defaults();
}

// poll
CRAFT_TYPE_DEFINE(uvw::PollEvent)
{
	_.add<GraphPropertyName>("uv/PollEvent");
	_.add<GraphPropertyCppName>("uvw::PollEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::PollEvent> _this) {
		std::ostringstream res;
		res << "<";
		bool first = true;
		if (_this->flags | uvw::Flags<uvw::PollHandle::Event>(uvw::PollHandle::Event::DISCONNECT)) {
			res << "DISCONNECT";
			first = false;
		}
		if (_this->flags | uvw::Flags<uvw::PollHandle::Event>(uvw::PollHandle::Event::PRIORITIZED)) {
			res << ((first) ? "" : "|") << "PRIORITIZED";
			first = false;
		}
		if (_this->flags | uvw::Flags<uvw::PollHandle::Event>(uvw::PollHandle::Event::READABLE)) {
			res << ((first) ? "" : "|") << "READABLE";
			first = false;
		}
		if (_this->flags | uvw::Flags<uvw::PollHandle::Event>(uvw::PollHandle::Event::WRITABLE)) {
			res << ((first) ? "" : "|") << "WRITEABLE";
		}
		
		return res.str();
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::PollEvent> _this) {
		std::ostringstream res;
		res << "(uv/PollEvent ";
		bool first = true;
		if (_this->flags | uvw::Flags<uvw::PollHandle::Event>(uvw::PollHandle::Event::DISCONNECT)) {
			res << "DISCONNECT";
			first = false;
		}
		if (_this->flags | uvw::Flags<uvw::PollHandle::Event>(uvw::PollHandle::Event::PRIORITIZED)) {
			res << ((first) ? "" : "|") << "PRIORITIZED";
			first = false;
		}
		if (_this->flags | uvw::Flags<uvw::PollHandle::Event>(uvw::PollHandle::Event::READABLE)) {
			res << ((first) ? "" : "|") << "READABLE";
			first = false;
		}
		if (_this->flags | uvw::Flags<uvw::PollHandle::Event>(uvw::PollHandle::Event::WRITABLE)) {
			res << ((first) ? "" : "|") << "WRITEABLE";
		}
		res << ")";
		return res.str();
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_DEFINE(uvw::PollHandle)
{
	_.defaults();
}

// prepare.hpp
CRAFT_TYPE_DEFINE(uvw::PrepareEvent)
{
	_.add<GraphPropertyName>("uv/PrepareEvent");
	_.add<GraphPropertyCppName>("uvw::PrepareEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::PrepareEvent> _this) {
		return "PrepareEvent";
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::PrepareEvent> _this) {
		return "(uv/PrepareEvent)";
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
CRAFT_DEFINE(uvw::PrepareHandle)
{
	_.defaults();
}

// process.hpp
CRAFT_TYPE_DEFINE(uvw::ExitEvent)
{
	_.add<GraphPropertyName>("uv/ExitEvent");
	_.add<GraphPropertyCppName>("uvw::ExitEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::ExitEvent> _this) {
		return fmt::format("ExitEvent Signal:{0}, Code:{1}", _this->signal, _this->status);
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::ExitEvent> _this) {
		return fmt::format("(uv/ExitEventSignal {0} {1}", _this->signal, _this->status);
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
CRAFT_DEFINE(uvw::ProcessHandle)
{
	_.defaults();
}

// signal.hpp
CRAFT_TYPE_DEFINE(uvw::SignalEvent)
{
	_.add<GraphPropertyName>("uv/SignalEvent");
	_.add<GraphPropertyCppName>("uvw::SignalEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::SignalEvent> _this) {
		return fmt::format("Signal:{0}", _this->signum);
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::SignalEvent> _this) {
		return fmt::format("(uv/SignalEvent {0})", _this->signum);
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
CRAFT_DEFINE(uvw::SignalHandle)
{
	_.defaults();
}

// stream.hpp
CRAFT_TYPE_DEFINE(uvw::ConnectEvent)
{
	_.add<GraphPropertyName>("uv/ConnectEvent");
	_.add<GraphPropertyCppName>("uvw::ConnectEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::ConnectEvent> _this) {
		return fmt::format("ConnectEvent:{0}");
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::ConnectEvent> _this) {
		return fmt::format("(uv/ConnectEvent {0})");
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_TYPE_DEFINE(uvw::EndEvent)
{
	_.add<GraphPropertyName>("uv/EndEvent");
	_.add<GraphPropertyCppName>("uvw::EndEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::EndEvent> _this) {
		return std::string("ExitEvent");
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::EndEvent> _this) {
		return std::string("(ExitEvent)");
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_TYPE_DEFINE(uvw::ListenEvent)
{
	_.add<GraphPropertyName>("uv/ListenEvent");
	_.add<GraphPropertyCppName>("uvw::ListenEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::ListenEvent> _this) {
		return std::string("ListenEvent");
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::ListenEvent> _this) {
		return std::string("(ListenEvent)");
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_TYPE_DEFINE(uvw::ShutdownEvent)
{
	_.add<GraphPropertyName>("uv/ShutdownEvent");
	_.add<GraphPropertyCppName>("uvw::ShutdownEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::ShutdownEvent> _this) {
		return std::string("ShutdownEvent");
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::ShutdownEvent> _this) {
		return std::string("(ShutdownEvent)");
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_TYPE_DEFINE(uvw::WriteEvent)
{
	_.add<GraphPropertyName>("uv/WriteEvent");
	_.add<GraphPropertyCppName>("uvw::WriteEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::WriteEvent> _this) {
		return std::string("WriteEvent");
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::WriteEvent> _this) {
		return std::string("(WriteEvent)");
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_TYPE_DEFINE(uvw::DataEvent)
{
	_.add<GraphPropertyName>("uv/DataEvent");
	_.add<GraphPropertyCppName>("uvw::DataEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::DataEvent> _this) {
		return std::string(_this->data.get(), _this->length);
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::DataEvent> _this) {
		return std::string("(DataEvent)");
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_DEFINE(uvw::details::ConnectReq)
{
	_.defaults();
}

CRAFT_DEFINE(uvw::details::ShutdownReq)
{
	_.defaults();
}

CRAFT_DEFINE(uvw::details::WriteReq)
{
	_.defaults();
}

// tcp.hpp
CRAFT_DEFINE(uvw::TcpHandle)
{
	_.defaults();
}

// timer.hpp
CRAFT_TYPE_DEFINE(uvw::TimerEvent)
{
	_.add<GraphPropertyName>("uv/TimerEvent");
	_.add<GraphPropertyCppName>("uvw::TimerEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::TimerEvent> _this) {
		return std::string("TimerEvent");
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::TimerEvent> _this) {
		return std::string("(TimerEvent)");
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_DEFINE(uvw::TimerHandle)
{
	_.defaults();
}

// tty.hpp
CRAFT_DEFINE(uvw::TTYHandle)
{
	_.defaults();
}

// udp.hpp
CRAFT_TYPE_DEFINE(uvw::SendEvent)
{
	_.add<GraphPropertyName>("uv/SendEvent");
	_.add<GraphPropertyCppName>("uvw::SendEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::SendEvent> _this) {
		return std::string("SendEvent");
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::SendEvent> _this) {
		return std::string("(SendEvent)");
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_TYPE_DEFINE(uvw::UDPDataEvent)
{
	_.add<GraphPropertyName>("uv/UDPDataEvent");
	_.add<GraphPropertyCppName>("uvw::UDPDataEvent");
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<uvw::UDPDataEvent> _this) {
		std::ostringstream res;
		res << "Datagram From " << _this->sender.ip << ":" << _this->sender.port << " ";
		res.write(_this->data.get(), _this->length);
		return res.str();
	});
	_.use<PRepr>().singleton<FunctionalRepr>(
		[](::craft::instance<uvw::UDPDataEvent> _this) {
		std::ostringstream res;
		res << "(uv/UDPDataEvent " << _this->sender.ip << ":" << _this->sender.port << " \"";
		res.write(_this->data.get(), _this->length) << "\"";
		return res.str();
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_DEFINE(uvw::UDPHandle)
{
	_.defaults();
}
#pragma endregion comment




instance<Module> cultlang::uv::make_uv_bindings(craft::instance<craft::lisp::Namespace> ns, craft::instance<> loader)
{
	auto ret = instance<Module>::make(ns, loader);
	auto sem = instance<CultSemantics>::make(ret);
	sem->builtin_implementMultiMethod("uv/loop",
		[]()
	{
		return uvw::Loop::create();
	});

	sem->builtin_implementMultiMethod("uv/run",
		[](instance<uvw::Loop> loop)
	{
		loop->run();
	});

	make_fs_bindings(ret);
	make_process_bindings(ret);

	make_pipe_bindings(ret);
	make_stream_bindings(ret);
	make_tcp_bindings(ret);
	make_tty_bindings(ret);
	make_util_bindings(ret);

	return ret;
}


BuiltinModuleDescription cultlang::uv::BuiltinUv("cult/uv", cultlang::uv::make_uv_bindings);
#include "types/dll_entry.inc"
