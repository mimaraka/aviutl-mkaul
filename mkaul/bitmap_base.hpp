#pragma once



namespace mkaul::graphics {
	// ビットマップ(抽象クラス)
	struct Bitmap {
	protected:
		void* data;

	public:
		Bitmap() :
			data(nullptr)
		{}
		virtual ~Bitmap() {}

		virtual void release() noexcept = 0;

		// データを取得
		template <typename Ptr>
		Ptr get_data() const noexcept {
			return reinterpret_cast<Ptr>(data);
		}
		// データを設定
		void set_data(void* ptr) noexcept { data = ptr; };

		// 幅・高さを取得
		virtual size_t width() const = 0;
		virtual size_t height() const = 0;
	};
} // namespace mkaul::graphics