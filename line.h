//==============================================
//
// ���̕`�������N���X [line.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// ���d�C���N���[�h�h�~
//**********************************************
#ifndef _LINE_H_
#define _LINE_H_

//**********************************************
// �C���N���[�h�t�@�C��
//**********************************************
#include <vector>
#include "object.h"

//**********************************************
// ���̕`��N���X�̒�`
//**********************************************
class CLineRenderer : public CObject
{
public:

	// ���_�\����
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
	};

	~CLineRenderer();
	static CLineRenderer* GetInstance(void);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override {};
	void Draw(void) override;

	void AddLine(const D3DXVECTOR3 start, const D3DXVECTOR3 end, const D3DXCOLOR col);
private:
	CLineRenderer();

	static constexpr int NUM_VERTEX = 500;	// ���_�̑���

	static CLineRenderer* m_pInstance;		// �����̃C���X�^���X
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	std::vector<Vertex> m_aLine;			// ���̏��
};
#endif
