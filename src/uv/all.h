#pragma once
#include "uv/common.h"

namespace cultlang {
namespace uv {
	CULTLANG_UV_EXPORTED void make_fs_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_process_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_tcp_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_util_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_pipe_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_tty_bindings(craft::types::instance<craft::lisp::Module> m);
	CULTLANG_UV_EXPORTED void make_uv_bindings(craft::types::instance<craft::lisp::Module> m);
}}

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::AsyncEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::CheckEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::AddrInfoEvent)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::NameInfoEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::ErrorEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::OPEN>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::READ>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::WRITE>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::SENDFILE>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::STAT>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::FSTAT>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::LSTAT>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::SCANDIR>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::READLINK>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::MKDIR>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::MKDTEMP>)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEvent<uvw::details::UVFsType::RMDIR>)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsEventEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::FsPollEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::CloseEvent)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::IdleEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::PollEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::PrepareEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::ExitEvent)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::SignalEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::ConnectEvent)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::EndEvent)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::ListenEvent)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::ShutdownEvent)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::WriteEvent)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::DataEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::TimerEvent)

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::SendEvent)
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uvw::UDPDataEvent)

#include "uv/cult/context.h"