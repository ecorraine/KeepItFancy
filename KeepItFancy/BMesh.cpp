#include "BMesh.h"

void MESH::GenerateIndices(int SideToDivide, int offset)
{
	FACE face = {};
	// lower left triangle | 下半分
	face.idx[0] = offset;								// lower left | 左下
	face.idx[1] = offset + (SideToDivide + 1) + 1;		// upper right | 右上
	face.idx[2] = offset + 1;							// lower right | 右下
	m_Faces.emplace_back(face);

	// upper right triangle | 上半分
	face.idx[0] = offset;								// lower left | 左下
	face.idx[1] = offset + (SideToDivide + 1);			// upper left | 左上
	face.idx[2] = offset + (SideToDivide + 1) + 1;		// upper right | 右上
	m_Faces.emplace_back(face);
}

void MESH::BindIndices()
{
	m_Faces.clear();

	// generate indices
	for (unsigned int y = 0; y < m_iDivY; y++) {
		for (unsigned int x = 0; x < m_iDivX; x++) {
			unsigned int offset = (m_iDivX + 1) * y + x;			// index of lower left vertex

			MESH::GenerateIndices(m_iDivX, offset);
		}
	}
	// check if the number of faces is correct
	assert(m_Faces.size() == ((m_iDivY * m_iDivX) * 2));
}

void MESH::Draw(RasterType cullmode)
{
	BindComputeShaders();

	// set buffers
	unsigned int stride = sizeof(VERTEX);
	unsigned offset = 0;
	DirectX11::GetContext()->IASetVertexBuffers(0, 1, m_cpVertexBuf.GetAddressOf(), &stride, &offset);
	DirectX11::GetContext()->IASetIndexBuffer(m_cpIndexBuf.Get(), DXGI_FORMAT_R32_UINT, 0);
	
	//DirectX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	SetTopology(TopologyType::TRIANGLELIST);

	SetWVPMatrix(m_pVS);
	m_pVS->BindShader();

	ProcessTessellation();

	SetLight(m_pPS);
	m_pPS->BindShader();

	// render
	SetCulling(cullmode);
	DirectX11::GetContext()->DrawIndexed(
		m_Faces.size() * 3,						// 描画するインデックス数（面数×３）
		0,										// 最初のインデックスバッファの位置
		0);										// 頂点バッファの最初から使う
}
