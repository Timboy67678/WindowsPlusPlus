#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__

#include <string>

namespace wpp
{
	class animation : public control {
	public:
		static constexpr UINT LOOP_INFINITE = static_cast<UINT>(-1);

		using control::control;

		bool open(LPCTSTR file_name) {
			return SendMessage(m_handle, ACM_OPEN, 0, (LPARAM)file_name) != 0;
		}

		bool open(const std::tstring& file_name) {
			return open(file_name.c_str());
		}

		bool open_resource(HINSTANCE h_instance, UINT resource_id) {
			return SendMessage(m_handle, ACM_OPEN, (WPARAM)h_instance, (LPARAM)MAKEINTRESOURCE(resource_id)) != 0;
		}

		bool open_resource(HINSTANCE h_instance, LPCTSTR resource_name) {
			return SendMessage(m_handle, ACM_OPEN, (WPARAM)h_instance, (LPARAM)resource_name) != 0;
		}

		bool play(UINT from_frame, UINT to_frame, UINT repeat_count) {
			return SendMessage(m_handle, ACM_PLAY, repeat_count, MAKELPARAM(from_frame, to_frame)) != 0;
		}

		bool play_all(UINT repeat_count = 1) {
			return play(0, LOOP_INFINITE, repeat_count);
		}

		bool play_once(UINT from_frame = 0, UINT to_frame = LOOP_INFINITE) {
			return play(from_frame, to_frame, 1);
		}

		bool play_loop(UINT from_frame = 0, UINT to_frame = LOOP_INFINITE) {
			return play(from_frame, to_frame, LOOP_INFINITE);
		}

		bool stop() {
			return SendMessage(m_handle, ACM_STOP, 0, 0L) != 0;
		}

		bool close() {
			return SendMessage(m_handle, ACM_OPEN, 0, 0L) != 0;
		}

		bool seek(UINT frame) {
			return SendMessage(m_handle, ACM_PLAY, 0, MAKELPARAM(frame, frame)) != 0;
		}

		bool is_playing() const {
			return SendMessage(m_handle, ACM_ISPLAYING, 0, 0L) != 0;
		}

		bool is_open() const {
			return SendMessage(m_handle, ACM_ISPLAYING, 0, 0L) != static_cast<LRESULT>(-1);
		}

		bool restart() {
			return seek(0) && is_open() ? play_all() : false;
		}

		bool toggle_play_pause() {
			if (is_playing()) {
				return stop();
			} else {
				return play_all();
			}
		}

		class scoped_animation {
			animation& anim_;
			bool auto_close_;

		public:
			scoped_animation(animation& anim, bool auto_close = true)
				: anim_(anim), auto_close_(auto_close) {}

			~scoped_animation() {
				if (auto_close_) {
					anim_.close();
				}
			}

			scoped_animation(const scoped_animation&) = delete;
			scoped_animation& operator=(const scoped_animation&) = delete;
		};

		scoped_animation make_scoped(bool auto_close = true) {
			return scoped_animation(*this, auto_close);
		}
	};
}

#endif //__ANIMATION_HPP__