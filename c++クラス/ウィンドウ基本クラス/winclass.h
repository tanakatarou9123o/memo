#ifndef WINCLASSH
#define WINCLASSH
// �w�b�_�t�@�C���̒��g

#define MAX_LOADSTRING 100

class WinClass
{
public:
	WinClass();
	~WinClass();
	virtual ATOM MyRegisterClass(HINSTANCE hInstance);//�E�B���h�E�N���X�o�^
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);//�E�B���h�E�쐬
	int MessageWhile();//���b�Z�[�W���[�v

	// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
	//static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	//static HINSTANCE hInsts;


private:
protected:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//�R�[���o�b�N�֐� �X�^�e�B�b�N

	HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
	TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
	TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
	MSG msg;
	HACCEL hAccelTable;

};

#endif