#ifndef FENNTON_MEMORY_HPP
#define FENNTON_MEMORY_HPP

/*
    For now, only thin wrappers around standard automatic memory management utilities.
*/

#include <memory>
#include <utility>
#include <list>

/* #define FENNTON_STACK_LOCK(name)\
    _bodyTrans = bodyTrans.lock();\
    if (!_bodyTrans)\
        throw std::runtime_error("bodyTrans is null.");
 */

namespace Fennton {
    template<typename T> using Strong = std::shared_ptr<T>;
    template<typename T> using Weak = std::weak_ptr<T>;
    template<typename T> using Scoped = std::unique_ptr<T>;

    template<typename T> class EnableStrongFromThis : public std::enable_shared_from_this<T> {
        // friend Strong<T>;
    public:
        // Returns a strong pointer sharing this.
        Strong<T> StrongFromThis() {
            return this->shared_from_this();
        }
        // Returns a strong pointer sharing this.
        Strong<T const> StrongFromThis() const {
            return this->shared_from_this();
        }
        // Returns a weak pointer sharing this.
        Weak<T> WeakFromThis() noexcept {
            return this->weak_from_this();
        }
        // Returns a weak pointer sharing this.
        Weak<T const> WeakFromThis() const noexcept {
            return this->weak_from_this();
        }
        virtual ~EnableStrongFromThis() = 0;
    };
    template<typename T> EnableStrongFromThis<T>::~EnableStrongFromThis() {}

    // Constructs a normal shared pointer.
    template<typename T, typename...A> Strong<T> makeStrong(A&&... args) {
        return std::make_shared<T>(std::forward<A>(args)...);
    }
    template<typename T, typename...A> Scoped<T> makeScoped(A&&... args) {
        return std::make_unique<T>(std::forward<A>(args)...);
    }

    template< class T, class U >
    Strong<T> staticStrongCast( const std::shared_ptr<U>& r ) noexcept {
        return static_pointer_cast<T>(r);
    }
    template< class T, class U >
    Strong<T> staticStrongCast( std::shared_ptr<U>&& r ) noexcept {
		return static_pointer_cast<T>(r);
	}
    template< class T, class U >
    Strong<T> dynamicStrongCast( const std::shared_ptr<U>& r ) noexcept {
		return dynamic_pointer_cast<T>(r);
	}
    template< class T, class U >
    Strong<T> dynamicStrongCast( std::shared_ptr<U>&& r ) noexcept {
		return dynamic_pointer_cast<T>(r);
	}
    template< class T, class U >
    Strong<T> constStrongCast( const std::shared_ptr<U>& r ) noexcept {
		return const_pointer_cast<T>(r);
	}
    template< class T, class U >
    Strong<T> constStrongCast( std::shared_ptr<U>&& r ) noexcept {
		return const_pointer_cast<T>(r);
	}
    template< class T, class U >
    Strong<T> reinterpretStrongCast( const std::shared_ptr<U>& r ) noexcept {
		return reinterpret_pointer_cast<T>(r);
	}
    template< class T, class U >
    Strong<T> reinterpretStrongCast( std::shared_ptr<U>&& r ) noexcept {
		return reinterpret_pointer_cast<T>(r);
	}

    // template<typename T> using List = std::list<T>;
}
#endif