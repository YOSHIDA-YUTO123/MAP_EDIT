//===================================================
//
// ���[�h�}�l�[�W���[ [LoadManager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _LOADMANAGER_H_
#define _LOADMANAGER_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>

//***************************************************
// ���[�h�̃}�l�[�W���[�N���X�̒�`
//***************************************************
class CLoadManager
{
public:
	CLoadManager();
	~CLoadManager();
	std::istringstream SetInputvalue(std::string input); // = �����̐��l�����o������
	std::string GetString(std::string input);	   // std::string���當��������o������
private:
};

#endif
