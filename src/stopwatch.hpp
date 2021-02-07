#include <chrono>

/**
 * @brief A clock-like class that can change its speed, be stopped or go in reverse.
 * Stopwatch is a class similar to those found in <chrono>. Like `std::chrono::steady_clock`, it has member typedefs
 * `duration` and `time_point` for measuring time. Unlike `steady_clock`, its `now()` method is not static. Every
 * stopwatch instance measures its own time, according to the speed that was set in constructor or in `set_speed()`.
 * At construction, its `time_since_epoch` is the same as `std::chrono::steady_clock`, but then diverges if the
 * stopwatch speed is changed.
 * 
 * Despite changing speed, you can measure time durations between time points, even if stopwatch speed was changed
 * inbetween measurements.
 * 
 * @note Due to the fact that `stopwatch::now()` is not static, stopwatch does not satisfy the Clock reqirement. It
 * cannot be used in functions like `std::this_thread::sleep_for`.
 */
class stopwatch {
private:
	using steady_clock = std::chrono::steady_clock;
	using steady_time_point = std::chrono::time_point<steady_clock>;

public:
	using duration = steady_clock::duration;
	using rep = duration::rep;
	using period = duration::period;
	using time_point = std::chrono::time_point<stopwatch>;

	constexpr static bool is_steady = false;

private:
	std::pair<steady_time_point, time_point> m_sync;
	// A pair of time_points that shows when this stopwatch was last compared to steady_clock. This, along with speed,
	// can be used to calculate now().

	long double m_speed = 1;

	std::pair<steady_time_point, time_point> impl_now() noexcept {
		auto steady_now = steady_clock::now();

		auto before_sync = m_sync.second.time_since_epoch();
		auto after_sync = std::chrono::duration_cast<duration>((steady_now - m_sync.first) * m_speed);
		return {steady_now, time_point {before_sync + after_sync}};
	}

public:
	/**
	 * @brief Construct a stopwatch.
	 * By default, the stopwatch starts with a speed of 1. Setting the speed to 0 means the stopwatch will be stopped,
	 * and `now()` will always return the time of construction.
	 * @warning Setting negative speed may result in the stopwatch advancing to the time before epoch, which results in
	 * undefined behavior.
	 */
	stopwatch(long double speed = 1) noexcept
		: m_speed(speed) {
		auto steady_now = steady_clock::now();
		auto steady_since_epoch = steady_now.time_since_epoch();
		m_sync = {steady_now, time_point(steady_since_epoch)};
	}

	/**
	 * @brief Get current time point from this stopwatch.
	 * Since stopwatch::is_steady is false, and the clock can be paused as well as go in reverse, it is not guaranteed
	 * that `now()` will always return an time point further into the future.
	 */
	time_point now() noexcept {
		return impl_now().second;
	}

	/**
	 * @brief Set the stopwatch speed, changing the ratio at which time moves compared to `std::chrono::steady_clock`.
	 * @note Speed can be positive, negative, or zero.
	 * @warning Setting negative speed may result in the stopwatch advancing to the time before epoch, which results in
	 * undefined behavior.
	 */
	void set_speed(long double speed) noexcept {
		auto [steady_now, my_now] = impl_now();
		m_sync = {steady_now, my_now};
		m_speed = speed;
	}

	/**
	 * @brief Get this stopwatch's speed, as in the ratio at which time moves compared to `std::chrono::steady_clock`.
	 * @note Speed can be positive, negative, or zero. Default-constructed stopwatch has a speed of 1.
	 */
	long double speed() noexcept {
		return m_speed;
	}
};
