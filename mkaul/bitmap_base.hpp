#pragma once



namespace mkaul {
	namespace graphics {
		// �r�b�g�}�b�v(���ۃN���X)
		struct Bitmap {
		protected:
			void* data;

		public:
			Bitmap() :
				data(nullptr)
			{}

			// �r�b�g�}�b�v��j��
			virtual void release() = 0;

			// �f�[�^���擾
			template <typename Ptr>
			Ptr get_data() const
			{
				return reinterpret_cast<Ptr>(data);
			}
			// �f�[�^��ݒ�
			void set_data(void* ptr) { data = ptr; };

			// ���E�������擾
			virtual size_t get_width() const = 0;
			virtual size_t get_height() const = 0;
		};
	}
}