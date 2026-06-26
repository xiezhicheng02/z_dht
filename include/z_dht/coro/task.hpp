#pragma once

#include <coroutine>
#include <exception>
#include <utility>

namespace z_dht::coro {

template <typename T>
class Task;

template <typename T>
class TaskPromise {
public:
    Task<T> get_return_object() {
        return Task<T>{std::coroutine_handle<TaskPromise>::from_promise(*this)};
    }

    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }

    void unhandled_exception() { exception_ = std::current_exception(); }

    template <typename Value>
    void return_value(Value&& value) {
        value_ = std::forward<Value>(value);
    }

    T& value() & { return value_; }
    const T& value() const & { return value_; }
    T&& value() && { return std::move(value_); }

    void rethrow_if_failed() {
        if (exception_) {
            std::rethrow_exception(exception_);
        }
    }

private:
    T value_{};
    std::exception_ptr exception_;
};

template <typename T>
class Task {
public:
    using promise_type = TaskPromise<T>;

    explicit Task(std::coroutine_handle<promise_type> handle) : handle_(handle) {}

    Task(Task&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            if (handle_) {
                handle_.destroy();
            }
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    ~Task() {
        if (handle_) {
            handle_.destroy();
        }
    }

    T get() {
        handle_.resume();
        auto& promise = handle_.promise();
        promise.rethrow_if_failed();
        return std::move(promise).value();
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

template <>
class Task<void>;

template <>
class TaskPromise<void> {
public:
    Task<void> get_return_object();

    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }

    void unhandled_exception() { exception_ = std::current_exception(); }
    void return_void() {}

    void rethrow_if_failed() {
        if (exception_) {
            std::rethrow_exception(exception_);
        }
    }

private:
    std::exception_ptr exception_;
};

template <>
class Task<void> {
public:
    using promise_type = TaskPromise<void>;

    explicit Task(std::coroutine_handle<promise_type> handle) : handle_(handle) {}

    Task(Task&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            if (handle_) {
                handle_.destroy();
            }
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    ~Task() {
        if (handle_) {
            handle_.destroy();
        }
    }

    void get() {
        handle_.resume();
        handle_.promise().rethrow_if_failed();
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

inline Task<void> TaskPromise<void>::get_return_object() {
    return Task<void>{std::coroutine_handle<TaskPromise>::from_promise(*this)};
}

} // namespace z_dht::coro
