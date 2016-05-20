#include "stdafx.h"
#include "DumpSearch.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void DumpSearch::ReadNew(WCHAR* fn)//���߂ɐV�K�������� �t�@�C���T�C�Y�̐�����new���� byte�t���O�ɉ�����
{
	if (pbuf)//���ڂ̐V�K�����Ȃ烁�������m�ۂ���Ă�̂�
	{
		delete pbuf;//NULL�łȂ��ꍇ�����������
	}
	if (pwbuf)
	{
		delete pwbuf;//NULL�łȂ��ꍇ�����������
	}
	if (pibuf)
	{
		delete pibuf;//NULL�łȂ��ꍇ�����������
	}

	ifstream RPE(fn, ios::in | ios::binary);//�t�@�C���p�X �ǂݍ��݃��[�h �o�C�i���ŊJ��
	if (!RPE)
	{
		MessageBox(NULL, L"open���s", L"ReadNew", MB_OK);
		return;//���s
	}
	RPE.seekg(0, ios::end);//seek���Ō�Ɉړ�
	fileSize = (int)RPE.tellg();//�T�C�Y�擾 ���݈ʒu�𒲂ׂ�֐�
	RPE.seekg(0, ios_base::beg);//seek�ŏ���


	int i = 0;
	int m = 0;//pwbuf�̈�v�ꏊ
	//int m = 0;//pibuf�̈�v�ꏊ
	
	switch (*flag)
	{
	case 1://1byte
		amari = 0;
		pbuf = new char[fileSize];//�t�@�C���T�C�Y�̕������m��

		while (!RPE.eof())
		{
			RPE.read(&pbuf[i], 1);
			i++;
		}

		break;
	case 2://2byte
		fsw = fileSize / 2;//2byte�̔z��̌�
		amari = fileSize % 2;//�]�肪�o�邩���ׂ�
		if (amari != 0)//�]�肪����
		{
			pwbuf = new WORD[fsw + 1];//�]�肪�o��ꍇ�̂���+1 ���ă������m��
			for (int i = 0; i < fsw+1; i++)//char�u�b�t�@����WORD�o�b�t�@�쐬
			{
				char temp[2];
				RPE.read(temp, 2);
				if (RPE.eof())
				{
					pwbuf[m] = (0xff & temp[0]);
					pwbuf[m] = pwbuf[m] << 8;//1byte ���V�t�g
					pwbuf[m] += (0xff & temp[1]);
					break;
				}

				pwbuf[m] = (0xff & temp[0]);
				pwbuf[m] = pwbuf[m] << 8;//1byte ���V�t�g
				pwbuf[m] += (0xff & temp[1]);

				m++;
			}
			break;
		}

		pwbuf = new WORD[fsw];//�]�肪�o�Ȃ� �������m��
		for (int i = 0; i < fsw; i++)//char�u�b�t�@����WORD�o�b�t�@�쐬
		{
			char temp[2];
			RPE.read(temp, 2);

			pwbuf[m] = (0xff & temp[0]);
			pwbuf[m] = pwbuf[m] << 8;//1byte ���V�t�g
			pwbuf[m] += (0xff & temp[1]);

			m++;
		}
		break;

	case 4:
		fsi = fileSize / 4;//4byte�̔z��̌�
		amari = fileSize % 4;//�]�肪�o�邩���ׂ�
		if (amari!=0)//�]�肪����
		{
			pibuf = new int[fsi + 1];//�]�肪�o��ꍇ�̂���+1 ���ă������m��
			for (int i = 0; i < fsi+1; i += 4)//char�u�b�t�@����int�o�b�t�@�쐬
			{
				char temp[4];
				RPE.read(temp, 4);
				if (RPE.eof())//����؂ꂽ��eof�͔������Ȃ�
				{
					pibuf[m] = (0xff & temp[0]);
					pibuf[m] = pibuf[m] << 8;//1byte ���V�t�g
					pibuf[m] += (0xff & temp[1]);
					pibuf[m] = pibuf[m] << 8;//1byte ���V�t�g
					pibuf[m] += (0xff & temp[2]);
					pibuf[m] = pibuf[m] << 8;//1byte ���V�t�g
					pibuf[m] += (0xff & temp[3]);
					break;
				}

				pibuf[m] = (0xff & temp[0]);
				pibuf[m] = pibuf[m] << 8;//1byte ���V�t�g
				pibuf[m] += (0xff & temp[1]);
				pibuf[m] = pibuf[m] << 8;//1byte ���V�t�g
				pibuf[m] += (0xff & temp[2]);
				pibuf[m] = pibuf[m] << 8;//1byte ���V�t�g
				pibuf[m] += (0xff & temp[3]);

				m++;
			}
			break;

		}
		
		pibuf = new int[fsi];//�]�肪�o�Ȃ� �������m��
		for (int i = 0; i < fsi; i++)//char�u�b�t�@����int�o�b�t�@�쐬
		{
			char temp[4];
			RPE.read(temp, 4);

			pibuf[m] = (0xff & temp[0]);
			pibuf[m] = pibuf[m] << 8;//1byte ���V�t�g
			pibuf[m] += (0xff & temp[1]);
			pibuf[m] = pibuf[m] << 8;//1byte ���V�t�g
			pibuf[m] += (0xff & temp[2]);
			pibuf[m] = pibuf[m] << 8;//1byte ���V�t�g
			pibuf[m] += (0xff & temp[3]);

			m++;
		}
		break;

	default:
		break;
	}
	

}

void DumpSearch::ReadFile(WCHAR* fn)//���ڈȍ~
{
	if (!pbuf && !pwbuf && !pibuf)
	{
		return;
	}

	ifstream RPE(fn, ios::in | ios::binary);
	if (!RPE)
	{
		//MessageBox(NULL, L"open���s", L"", MB_OK);
		return;//���s
	}
	RPE.seekg(0, ios::end);//seek���Ō�Ɉړ�
	int fileSize2 = (int)RPE.tellg();//�T�C�Y�擾 ���݈ʒu�𒲂ׂ�֐�
	RPE.seekg(0, ios_base::beg);//seek�ŏ���
	if (fileSize!=fileSize2)
	{
		MessageBox(NULL, L"�t�@�C���T�C�Y���Ⴂ�܂�", L"", MB_OK);
		return;
	}

	int i = 0;
	switch (*flag)
	{
	case 1://1byte
		while (!RPE.eof())
		{
			RPE.read(&pbuf[i], 1);
			i++;
		}

		break;
	case 2://2byte
		if (amari != 0)//�]�肪����
		{
			for (int i = 0; i < fsw + 1; i++)//char�u�b�t�@����WORD�o�b�t�@�쐬
			{
				char temp[2];
				RPE.read(temp, 2);

				pwbuf[i] = (0xff & temp[0]);
				pwbuf[i] = pwbuf[i] << 8;//1byte ���V�t�g
				pwbuf[i] += (0xff & temp[1]);

			}
		}
		else
		{
			for (int i = 0; i < fsw; i++)//char�u�b�t�@����WORD�o�b�t�@�쐬
			{
				char temp[2];
				RPE.read(temp, 2);

				pwbuf[i] = (0xff & temp[0]);
				pwbuf[i] = pwbuf[i] << 8;//1byte ���V�t�g
				pwbuf[i] += (0xff & temp[1]);

			}
		}
		

		break;
	case 4:
		if (amari != 0)//�]�肪����
		{
			for (int i = 0; i < fsi + 1; i++)//char�u�b�t�@����int�o�b�t�@�쐬
			{
				char temp[4];
				RPE.read(temp, 4);

				pibuf[i] = (0xff & temp[0]);
				pibuf[i] = pibuf[i] << 8;//1byte ���V�t�g
				pibuf[i] += (0xff & temp[1]);
				pibuf[i] = pibuf[i] << 8;//1byte ���V�t�g
				pibuf[i] += (0xff & temp[2]);
				pibuf[i] = pibuf[i] << 8;//1byte ���V�t�g
				pibuf[i] += (0xff & temp[3]);

			}
		}
		else
		{
			for (int i = 0; i < fsi; i++)//char�u�b�t�@����int�o�b�t�@�쐬
			{
				char temp[4];
				RPE.read(temp, 4);

				pibuf[i] = (0xff & temp[0]);
				pibuf[i] = pibuf[i] << 8;//1byte ���V�t�g
				pibuf[i] += (0xff & temp[1]);
				pibuf[i] = pibuf[i] << 8;//1byte ���V�t�g
				pibuf[i] += (0xff & temp[2]);
				pibuf[i] = pibuf[i] << 8;//1byte ���V�t�g
				pibuf[i] += (0xff & temp[3]);

			}
		}

		break;

	default:
		break;
	}

	
}

void DumpSearch::SearchStart()//�V�K���� �G���f�B�A�����猟�����l��ϊ�
{
	if (!pbuf && !pwbuf && !pibuf)//null�Ȃ�
	{
		return;
	}

	SendMessage((*hwn)["HLISTBOX1"] , LB_RESETCONTENT, 0, 0);	//listbox �N���A
	adores.clear();//�A�h���X�x�N�^�[�N���A

	WCHAR text[50];
	GetWindowText((*hwn)["HEDIT1"] , text, 50);//�E�B���h�E�n���h���̃e�L�X�g���擾

	int s1;
	s1 = _wtoi(text);
	
	if ((text[0]<0x30)|(text[0]>0x39))//���l�ȊO�Ȃ� ������`�F�b�N
	{
		MessageBox(NULL, L"������ł�", L"", MB_OK);
		return;
	}

	if (*flag==1)//1byte
	{
		for (int i = 0; i < fileSize; i++)
		{
			int b = (0xff & pbuf[i]);//�I�[�o�[�t���[�΍�
			if (s1==b)
			{
				WCHAR s1[50];

				wsprintf(s1, TEXT("0x%08X"), i);

				SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox ���ڒǉ� �E�B���h�E�^�C�g��

				adores.push_back(i);//�A�h���X�̏ꏊ
			}
		}
		SetWindowText((*hwn)["HWND1"], L"1byte");
	}
	else if (*flag == 2)//2byte �r�b�O�G���f�B�A��
	{
		int m = 0;//pwbuf�̈�v�ꏊ
		if (amari!=0)//�]�肪����
		{
			for (int i = 0; i < fsi + 2; i += 2)//char�u�b�t�@����WORD�o�b�t�@�쐬
			{
				if (s1 == pwbuf[m])
				{
					WCHAR s1[50];
					wsprintf(s1, TEXT("0x%08X"), i);

					SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox ���ڒǉ� �E�B���h�E�^�C�g��

					adores.push_back(m);//�A�h���X�̏ꏊ
				}

				m++;
			}
		}
		else
		{
			for (int i = 0; i < fileSize; i += 2)//char�u�b�t�@����WORD�o�b�t�@�쐬
			{
				if (s1 == pwbuf[m])
				{
					WCHAR s1[50];
					wsprintf(s1, TEXT("0x%08X"), i);

					SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox ���ڒǉ� �E�B���h�E�^�C�g��

					adores.push_back(m);//�A�h���X�̏ꏊ
				}

				m++;
			}
		}
		

		SetWindowText((*hwn)["HWND1"], L"2byte");

	}
	else if (*flag == 4)//4byte
	{
		int m = 0;//pibuf�̈�v�ꏊ

		if (amari != 0)//�]�肪����
		{
			for (int i = 0; i < fileSize + 4; i += 4)//char�u�b�t�@����WORD�o�b�t�@�쐬
			{
				if (s1 == pibuf[m])
				{
					WCHAR s1[50];
					wsprintf(s1, TEXT("0x%08X"), i);

					SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox ���ڒǉ� �E�B���h�E�^�C�g��

					adores.push_back(m);//�A�h���X�̏ꏊ
				}

				m++;
			}

		}
		else
		{
			for (int i = 0; i < fileSize; i += 4)//char�u�b�t�@����WORD�o�b�t�@�쐬
			{
				if (s1 == pibuf[m])
				{
					WCHAR s1[50];
					wsprintf(s1, TEXT("0x%08X"), i);

					SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox ���ڒǉ� �E�B���h�E�^�C�g��

					adores.push_back(m);//�A�h���X�̏ꏊ
				}

				m++;
			}

		}
		
		

		SetWindowText((*hwn)["HWND1"], L"4byte");
	}

}

void DumpSearch::SearchRefine()//�J��Ԃ�����
{
	if (flag==0)
	{
		return;
	}

	SendMessage((*hwn)["HLISTBOX1"], LB_RESETCONTENT, 0, 0);	//listbox �N���A

	WCHAR text[50];
	GetWindowText((*hwn)["HEDIT1"], text, 50);//�E�B���h�E�n���h���̃e�L�X�g���擾
	int s1 = _wtoi(text);

	switch (*flag)
	{
	case 1://1byte
	{
		SetWindowText((*hwn)["HWND1"], L"1byte");
		vector<int>::iterator pa;//�R���e�i�̈ʒu���i�[����ϐ�
		pa = adores.begin();//�R���e�i�̍ŏ��̗v�f������

		while (pa != adores.end())//�Ō�łȂ��Ȃ�J��Ԃ�
		{
			int b = (0xff & pbuf[*pa]);//�I�[�o�[�t���[�΍�
			if (s1 == b)
			{
				WCHAR s1[50];

				wsprintf(s1, TEXT("0x%08X"), *pa);

				SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox ���ڒǉ� �E�B���h�E�^�C�g��

				++pa;//���̗v�f��
			}
			else
			{
				pa = adores.erase(pa);//�폜
			}
		}
	}
		break;
	case 2://2byte
	{
		SetWindowText((*hwn)["HWND1"], L"2byte");

		vector<int>::iterator pa;//�R���e�i�̈ʒu���i�[����ϐ�
		pa = adores.begin();//�R���e�i�̍ŏ��̗v�f������

		while (pa != adores.end())//�Ō�łȂ��Ȃ�J��Ԃ�
		{
			int b = (0xffff & pwbuf[*pa]);//�I�[�o�[�t���[�΍�
			if (s1 == b)
			{
				WCHAR s1[50];

				wsprintf(s1, TEXT("0x%08X"), (*pa)*2);//WORD������2�{���Ă�

				SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox ���ڒǉ� �E�B���h�E�^�C�g��

				++pa;//���̗v�f��
			}
			else
			{
				pa = adores.erase(pa);//�폜
			}
		}
	}

		break;
	case 4://4byte
	{
		SetWindowText((*hwn)["HWND1"], L"4byte");

		vector<int>::iterator pa;//�R���e�i�̈ʒu���i�[����ϐ�
		pa = adores.begin();//�R���e�i�̍ŏ��̗v�f������

		while (pa != adores.end())//�Ō�łȂ��Ȃ�J��Ԃ�
		{
			int b = pibuf[*pa];//�I�[�o�[�t���[�΍�
			if (s1 == b)
			{
				WCHAR s1[50];

				wsprintf(s1, TEXT("0x%08X"), (*pa) * 4);//int������4�{���Ă�

				SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox ���ڒǉ� �E�B���h�E�^�C�g��

				++pa;//���̗v�f��
			}
			else
			{
				pa = adores.erase(pa);//�폜
			}
		}
	}
		break;

	default:
		break;
	}

}


DumpSearch::DumpSearch(map<char*, HWND>* h, unsigned char* byteflag)
{
	hwn=h;//�E�B���h�E�n���h���̃R���e�i�̃A�h���X�󂯎�� 
	flag = byteflag;
}


DumpSearch::~DumpSearch()
{
}
