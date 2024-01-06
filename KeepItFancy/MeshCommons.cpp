#include "MeshCommons.h"

void LINEBASE::BindIndices()
{
	m_Faces.clear();
	for (int currentSide = 0; currentSide < m_iSides; ++currentSide) {
		if (currentSide < 1) {
			for (int i = 0; i < m_iSegments; ++i) {
				FACE face = {};
				face.idx[0] = i;
				face.idx[1] = i + 1;

				m_Faces.emplace_back(face);
			}
		}
		else {
			for (int i = 0; i < m_iSegments; ++i) {
				FACE face = {};
				face.idx[0] = i + (m_iSegments * currentSide + 1);
				face.idx[1] = i + (m_iSegments * currentSide + 1) + 1;

				m_Faces.emplace_back(face);
			}
		}
	}
	assert(m_Faces.size() == (m_iSegments * m_iSides));
}

void LINEBASE::Draw(RasterType cullmode)
{
	BindComputeShaders();

	// set buffers
	unsigned int stride = sizeof(VERTEX);
	unsigned offset = 0;
	DirectX11::GetContext()->IASetVertexBuffers(0, 1, m_cpVertexBuf.GetAddressOf(), &stride, &offset);
	DirectX11::GetContext()->IASetIndexBuffer(m_cpIndexBuf.Get(), DXGI_FORMAT_R32_UINT, 0);

	// set topology to line list for segments
	DirectX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	SetWVPMatrix(m_pVS);
	m_pVS->BindShader();

	ProcessTessellation();

	SetLight(m_pPS);
	m_pPS->BindShader();
	if (m_cpSRV)
		m_pPS->SetSRV(0, m_cpSRV.Get());

	SetCulling(cullmode);
	// render with 2 indices per segment
	DirectX11::GetContext()->DrawIndexed(m_Faces.size() * 2, 0, 0);
}



void TRIANGLEBASE::GenerateIndices(int SideToDivide, int offset)
{
	FACE face = {};
	// lower left triangle | 下半分
	face.idx[0] = offset;									// lower left | 左下
	face.idx[1] = offset + (SideToDivide + 1) + 1;			// upper right | 右上
	face.idx[2] = offset + 1;								// lower right | 右下
	m_Faces.emplace_back(face);

	// upper right triangle | 上半分
	face.idx[0] = offset;									// lower left | 左下
	face.idx[1] = offset + (SideToDivide + 1);				// upper left | 左上
	face.idx[2] = offset + (SideToDivide + 1) + 1;			// upper right | 右上
	m_Faces.emplace_back(face);
}

void TRIANGLEBASE::BindIndices()
{
	m_Faces.clear();

	// generate indices
	for (unsigned int y = 0; y < m_iDivY; y++) {
		for (unsigned int x = 0; x < m_iDivX; x++) {
			unsigned int offset = (m_iDivX + 1) * y + x;	// index of lower left vertex

			GenerateIndices(m_iDivX, offset);
		}
	}
	// check if the number of faces is correct
	assert(m_Faces.size() == ((m_iDivY * m_iDivX) * 2));
}

void TRIANGLEBASE::Draw(RasterType cullmode)
{
	BindComputeShaders();

	// set buffers
	unsigned int stride = sizeof(VERTEX);
	unsigned offset = 0;
	DirectX11::GetContext()->IASetVertexBuffers(0, 1, m_cpVertexBuf.GetAddressOf(), &stride, &offset);
	DirectX11::GetContext()->IASetIndexBuffer(m_cpIndexBuf.Get(), DXGI_FORMAT_R32_UINT, 0);

	// set topology to triangle list for faces
	DirectX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	SetWVPMatrix(m_pVS);
	m_pVS->BindShader();

	ProcessTessellation();

	SetLight(m_pPS);
	m_pPS->BindShader();
	if (m_cpSRV)
		m_pPS->SetSRV(0, m_cpSRV.Get());

	SetCulling(cullmode);
	// render with 3 indices per face
	DirectX11::GetContext()->DrawIndexed( m_Faces.size() * 3, 0, 0);
}
