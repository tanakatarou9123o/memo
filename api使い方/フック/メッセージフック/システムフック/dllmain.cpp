// dllmain.cpp : DLL �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
#include "stdafx.h"

#pragma comment(linker, "/Section:MY_DATA,RWS")//���L�ϐ� �v���Z�X��
#pragma data_seg("MY_DATA") 
HHOOK hhok = 0;									////���L�ϐ� �v���Z�X�� �t�b�N�v���V�[�W���n���h��
#pragma data_seg()

#include "APIHOOKDLL.h"//�w�b�_
HINSTANCE hInst;//�O���[�o���ϐ�




BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: 
		hInst = hModule;
		//MessageBox(NULL, TEXT("�A�^�b�`���܂���"), TEXT("OK"), MB_OK); 
		break;

	case DLL_PROCESS_DETACH: 
		//MessageBox(NULL, TEXT("�f�^�b�`���܂���"), TEXT("OK"), MB_OK); 
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}


EXPORT HHOOK starthook()//�t�b�N�J�n�֐�
{
	hhok = SetWindowsHookEx
		(
		WH_KEYBOARD | WH_MOUSE, // �C���X�g�[������t�b�N�̃^�C�v �}�E�X�ƃL�[�{�[�h
		(HOOKPROC)MyHookProc, // �t�b�N�v���V�[�W���̃A�h���X
		hInst, // �C���X�^���X�n���h�� 
		0 // �X���b�hID 
		);

	return hhok;
}

EXPORT BOOL closehook()//�t�b�N�I���֐�
{
	if (!UnhookWindowsHookEx(hhok))// ��菜���t�b�N�v���V�[�W���n���h��
	{
		MessageBox(NULL, TEXT(""), TEXT(""), MB_OK);
	}

	return true;
}



LRESULT CALLBACK MyHookProc(int nCode, WPARAM wp, LPARAM lp) //�t�b�N�v���V�[�W��
{

	if (nCode < 0)//�}�C�i�X�Ȃ�
	{
		return CallNextHookEx(hhok, nCode, wp, lp);
	}

	if (wp == WM_LBUTTONUP)//���N���b�N�擾
	{
		MessageBox(NULL, TEXT("���N���b�N"), TEXT("OK"), MB_OK);
		return CallNextHookEx(hhok, nCode, wp, lp);
	}



	return CallNextHookEx(hhok, nCode, wp, lp);//�{���̏����֓n��


	return 0;
}