#include <mutex>

namespace synchronized {

    template <typename T>
    class synchronized {
        T t;
        std::mutex mx;

        class synchronization_gaurd {
            synchronized<T>& synch;
            synchronization_gaurd(const synchronization_gaurd&) = delete;
            synchronization_gaurd(synchronization_gaurd&&) = delete;

        public:
            explicit synchronization_gaurd(synchronized<T>& synch) : synch{synch} {}

            ~synchronization_gaurd() {
                synch.mx.unlock();
            }

            T* operator->() const noexcept {
                return std::addressof(synch.t);
            }

            operator T&() const noexcept {
                return synch.t;
            }

            template <typename U, typename = std::enable_if_t<
                  std::is_same_v<T, std::remove_reference_t<U>>>>
            synchronization_gaurd& operator =(U&& other) {
                synch.t = std::forward<U>(other);
                return *this;
            }
        };

    public:
        explicit synchronized(T t) : t{std::move(t)}, mx{} {}

        synchronized(synchronized<T>&& other) : mx{} {
            std::lock_guard<std::mutex> guard {other.mx};
            t = std::move(other).t;
        }

        synchronized(const synchronized<T>&) = delete;
        ~synchronized() = default;

        synchronization_gaurd operator->() {
            mx.lock();
            return synchronization_gaurd{*this};
        }

        synchronization_gaurd value() {
            mx.lock();
            return synchronization_gaurd{*this};
        }
    };

    template <typename T, typename... Args>
    auto make_synchronized(Args&&... args) {
        return synchronized<T>(T(std::forward<Args>(args)...));
    }
}
