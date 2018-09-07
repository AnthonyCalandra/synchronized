#include <mutex>

namespace synchronized {

    template <typename T>
    class unique_synchronized;

    template <typename T>
    class synchronization_gaurd {
        friend class unique_synchronized<T>;
        unique_synchronized<T>& synch;

        synchronization_gaurd(const synchronization_gaurd<T>&) = delete;
        synchronization_gaurd(synchronization_gaurd<T>&&) = delete;

    public:
        explicit synchronization_gaurd(unique_synchronized<T>& synch) : synch(synch) {}

        ~synchronization_gaurd() {
            synch.mx.unlock();
        }

        T* operator->() const noexcept {
            return std::addressof(synch.t);
        }

        operator T&() const noexcept {
            return synch.t;
        }
    };

    template <typename T>
    class unique_synchronized {
        friend class synchronization_gaurd<T>;
        T t;
        std::mutex mx;

    public:
        explicit unique_synchronized(T&& t) : t(std::forward<T>(t)) {}
        unique_synchronized(unique_synchronized<T>&) = delete;
        unique_synchronized(unique_synchronized<T>&&) = default;
        ~unique_synchronized() = default;

        synchronization_gaurd<T> operator->() {
            mx.lock();
            return synchronization_gaurd<T>{*this};
        }

        T* operator*() const noexcept {
            return std::addressof(t);
        }

        T* get() const noexcept {
            return std::addressof(t);
        }

        synchronization_gaurd<T> operator()() {
            mx.lock();
            return synchronization_gaurd<T>{*this};
        }

        const T& operator()() const noexcept {
            return t;
        }

        const T& operator()(T newT) {
            std::lock_guard<std::mutex> guard {mx};
            t = std::move(newT);
            return t;
        }
    };

    template <typename T, typename ...Args>
    auto make_synchronized(Args&&... args) {
        return unique_synchronized<T>(T(std::forward<Args>(args)...));
    }
}
