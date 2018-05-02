#pragma once
#include "uv/common.h"

#include <memory>
#include <utility>
#include "emitter.hpp"
#include "underlying_type.hpp"


namespace uvw {


/**
 * @brief Common class for almost all the resources available in `uvw`.
 *
 * This is the base class for handles and requests.
 */
template<typename T, typename U>
class Resource: public UnderlyingType<T, U>, public Emitter<T> {
protected:
    using ConstructorAccess = typename UnderlyingType<T, U>::ConstructorAccess;

    auto parent() const noexcept {
        return this->loop().loop.get();
    }

    void leak() noexcept {
        //sPtr = this->shared_from_this();
    }

    void reset() noexcept {
        //sPtr.reset();
    }

    bool self() const noexcept {
        return static_cast<bool>(this);
    }

public:
    explicit Resource(ConstructorAccess ca, craft::instance<Loop> ref)
        : UnderlyingType<T, U>{ca, std::move(ref)},
          Emitter<T>{}
    {
        this->get()->data = static_cast<T*>(this);
    }

    /**
     * @brief Gets user-defined data. `uvw` won't use this field in any case.
     * @return User-defined data if any, an invalid pointer otherwise.
     */
    template<typename R = void>
    craft::instance<R> data() const {
        return userData.asType<R>();
    }

    /**
     * @brief Sets arbitrary data. `uvw` won't use this field in any case.
     * @param uData User-defined arbitrary data.
     */
    void data(craft::types::instance<> uData) {
        userData = uData;
    }

private:
    craft::instance<> userData;
};


}
