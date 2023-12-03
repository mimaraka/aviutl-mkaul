#pragma once



namespace mkaul {
	namespace graphics {
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
			Ptr get_data() const {
				return reinterpret_cast<Ptr>(data);
			}
			// データを設定
			void set_data(void* ptr) { data = ptr; };

			// 幅・高さを取得
			virtual size_t get_width() const = 0;
			virtual size_t get_height() const = 0;
		};
	}
}