#pragma once
#include "uv/common.h"

#include "uv/prelude.h"

#include "uv/buffer.h"
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uv_stat_t);
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uv_fs_type);
CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uv_dirent_t);

namespace cultlang {
namespace uv
{
	class Fs
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::Fs)
	public:
		std::unique_ptr<uv_fs_t> _fs;

	public:
		CULTLANG_UV_EXPORTED Fs();
		CULTLANG_UV_EXPORTED Fs(const Check& other) = delete;
		CULTLANG_UV_EXPORTED Fs(Fs&& other) noexcept;
		CULTLANG_UV_EXPORTED ~Fs() noexcept;
		CULTLANG_UV_EXPORTED Fs& operator= (const Fs& other) = delete;
		CULTLANG_UV_EXPORTED Fs& operator= (Fs&& other) noexcept;


		CULTLANG_UV_EXPORTED void stop();
	};


	class ReadFile
		: public virtual craft::types::Object
	{
		CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::uv::ReadFile)
	private:
		typedef craft::types::instance<cultlang::uv::Loop> i_loop;
		typedef craft::types::instance<std::string> i_str;
		typedef craft::types::instance<int64_t> i_int;
		typedef craft::types::instance<> i_func;
		
		craft::types::instance<Fs> _open, _read, _close;
		uv_buf_t _buffer;
		i_str _file;

		i_func _fread;
		i_func _ferr;
		i_func _fdone;
		i_loop _loop;
	public:
		CULTLANG_UV_EXPORTED ReadFile(i_loop l, i_str file, i_int bufsize, i_func err, i_func read, i_func done);
		CULTLANG_UV_EXPORTED void craft_setupInstance();
		~ReadFile();
	};
		
	CULTLANG_UV_EXPORTED void make_fs_ops(craft::types::instance<craft::lisp::Module> ret);
}}