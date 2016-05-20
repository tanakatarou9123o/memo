#include "stdafx.h"
#include "GetModule.h"


GetModule::GetModule()
{
	HANDLE hand;
	MODULEENTRY32 mo;
	mo.dwSize = sizeof(MODULEENTRY32);
	try
	{
		hand = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());//�v���Z�XID
		if ((int)hand == -1)
		{
			throw(hand);
		}
		Module32First(hand, &mo);//�ŏ��̃��W���[��
		if ((int)hand == -1)
		{
			throw(hand);
		}
		do
		{
			vme.push_back(mo);
		} while (Module32Next(hand, &mo));//���s������false��Ԃ�

		vector<MODULEENTRY32>::iterator pmo;
		pmo = vme.begin();
		while (pmo != vme.end())
		{
			wchar_t h[500];
			wsprintf(h, L"�t�@�C���p�X %s \n�t�@�C���� %s \n ���[�h�� %08X", (*pmo).szExePath, (*pmo).szModule, (*pmo).modBaseAddr);
			MessageBox(nullptr, h, h, MB_OK);


			pmo++;
		}


	}
	catch (HANDLE)
	{

	}
}
GetModule::GetModule(DWORD pid)
{
	HANDLE hand;
	MODULEENTRY32 mo;
	mo.dwSize = sizeof(MODULEENTRY32);
	try
	{
		hand = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);//�v���Z�XID
		if ((int)hand == -1)
		{
			throw(hand);
		}
		Module32First(hand, &mo);//�ŏ��̃��W���[��
		if ((int)hand == -1)
		{
			throw(hand);
		}
		do
		{
			vme.push_back(mo);
		} while (Module32Next(hand, &mo));//���s������false��Ԃ�

		vector<MODULEENTRY32>::iterator pmo;
		pmo = vme.begin();//����
		while (pmo != vme.end())
		{
			wchar_t h[500];
			wsprintf(h, L"�t�@�C���p�X %s \n�t�@�C���� %s \n ���[�h�� %08X", (*pmo).szExePath, (*pmo).szModule, (*pmo).modBaseAddr);
			MessageBox(nullptr, h, h, MB_OK);


			pmo++;
		}


	}
	catch (HANDLE)
	{

	}
}
GetModule::~GetModule()
{
}