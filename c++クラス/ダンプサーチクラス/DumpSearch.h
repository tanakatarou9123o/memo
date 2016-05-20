#ifndef DumpSearchaH
#define DumpSearchH
#include <vector>
#include <map>//�}�b�v�N���X
using namespace std;

class DumpSearch
{
public:
	DumpSearch(map<char*, HWND>*,unsigned char*);//�R���e�i�ŃE�B���h�E�n���h���擾 �R���X�g���N�^ byteflag�̃A�h���X�󂯎��
	~DumpSearch();
	void SearchStart();
	void SearchRefine();
	void ReadFile(WCHAR* fn);
	void ReadNew(WCHAR* fn);

private:
	map<char*, HWND>* hwn;//�E�B���h�E�n���h�� �R���e�i

	char* pbuf = NULL;//1byte
	WORD* pwbuf = NULL;//2byte
	int* pibuf = NULL;//4byte
	int fileSize;//char�z��
	int fsw;//WORD�z��
	int fsi;//int�z��
	int amari;//�]�肪�o�邩
	vector<int> adores;//�����Ƀq�b�g�����A�h���X���i�[

	unsigned char* flag;//�o�C�g�� ���ʃt���O
};

#endif