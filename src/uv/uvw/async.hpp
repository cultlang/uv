#pragma once
#include "uv/common.h"

#include <utility>
#include <memory>
#include "uv/uv.h"
#include "handle.hpp"
#include "loop.hpp"


namespace uvw {


/**
 * @brief AsyncEvent event.
 *
 * It will be emitted by AsyncHandle according with its functionalities.
 */
struct AsyncEvent
	: public virtual craft::types::Object
{
	CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(uvw::AsyncEvent);
};


/**
 * @brief The AsyncHandle handle.
 *
 * Async handles allow the user to _wakeup_ the event loop and get an event
 * emitted from another thread.
 *
 * To create an `AsyncHandle` through a `Loop`, no arguments are required.
 */
class AsyncHandle final
	: public Handle<AsyncHandle, uv_async_t>
	, public virtual craft::types::Object
{
	CULTLANG_UV_EXPORTED CRAFT_OBJECT_DECLARE(uvw::AsyncHandle);
public:
    static void sendCallback(uv_async_t *handle) {
        AsyncHandle &async = *(static_cast<AsyncHandle*>(handle->data));
        async.publish(AsyncEvent{});
    }

public:
    using Handle::Handle;

    /**
     * @brief Initializes the handle.
     *
     * Unlike other handle initialization functions, it immediately starts the
     * handle.
     *
     * @return True in case of success, false otherwise.
     */
    bool init() {
        return initialize(&uv_async_init, &sendCallback);
    }

    /**
     * @brief Wakeups the event loop and emits the AsyncEvent event.
     *
     * It’s safe to call this function from any thread.<br/>
     * An AsyncEvent event will be emitted on the loop thread.
     *
     * See the official
     * [documentation](http://docs.libuv.org/en/v1.x/async.html#c.uv_async_send)
     * for further details.
     */
    void send() {
        invoke(&uv_async_send, get());
    }
};


}
