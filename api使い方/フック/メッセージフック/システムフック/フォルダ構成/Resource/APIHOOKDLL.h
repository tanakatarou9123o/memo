#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)		//C�Ăяo���̏ꍇ
#else
#define EXPORT __declspec(dllexport)				//C++�Ăяo���̏ꍇ
#endif



EXPORT HHOOK starthook(void);
EXPORT BOOL closehook(void);//�t�b�N�I���֐�
LRESULT CALLBACK MyHookProc(int nCode, WPARAM wp, LPARAM lp); //�t�b�N�v���V�[�W�� �v���g�^�C�v