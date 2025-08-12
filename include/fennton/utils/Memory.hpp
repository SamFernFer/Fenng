#ifndef FENNTON_MEMORY_HPP
#define FENNTON_MEMORY_HPP

#include <memory>

namespace Fennton::Memory {
    template<typename T> using Strong = std::shared_ptr<T>;
    template<typename T> using Weak = std::weak_ptr<T>;
    template<typename T> using Unique = std::unique_ptr<T>;

    template<typename T> class EnableStrongFromThis : public std::enable_shared_from_this<T> {
    public:
        Strong<T> StrongFromThis() {
            return this->shared_from_this();
        }
        Strong<T const> StrongFromThis() const {
            return this->shared_from_this();
        }
        Weak<T> WeakFromThis() {
            return this->weak_from_this();
        }
        Weak<T const> WeakFromThis() const {
            return this->weak_from_this();
        }
    };

    template<typename T, typename... A> Strong<T> makeStrong(A... args) {
        return std::make_shared<T>(args...);
    }
    template<typename T, typename... A> Unique<T> makeUnique(A... args) {
        return std::make_unique<T>(args...);
    }
}
#endif