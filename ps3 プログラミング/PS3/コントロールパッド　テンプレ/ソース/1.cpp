#include "padutil.h"

int main()
{
	
	cellPadUtilPadInit();		//�p�b�h������
	
	int cono=-1;
	while(cono==-1)
	{
		cellPadUtilUpdate();		//�p�b�h���X�V
		cono=cellPadUtilGetFirstConnectedPad();		//�p�b�h�i���o�[�擾
	}
	if(cono==-1){return 1;}

	while(1)
	{
		cellPadUtilUpdate();			//�p�b�h���X�V
		if(cellPadUtilButtonReleasedOnce(cono,CELL_UTIL_BUTTON_START))	//�{�^���������ꂽ�u�Ԃ��擾
		{return 0;}
	}

    	cellPadUtilPadEnd();		//�p�b�h�I��

	return 0;
}