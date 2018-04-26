#pragma once
#include "common.h"

#include "prelude.h"

CRAFT_TYPE_DECLARE(CULTLANG_UV_EXPORTED, uv_buf_t);

namespace cultlang{
namespace uv
{
	CULTLANG_UV_EXPORTED void make_buffer_ops(craft::types::instance<craft::lisp::Module> ret);
}}