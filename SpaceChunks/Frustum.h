#pragma once
#include "XyEngine.h"

class Frustum
{
public:
	float m_Frustum[6][4];

	enum sides
	{
		RIGHT = 0,
		LEFT,
		BOTTOM,
		TOP,
		BACK,
		FRONT
	};

	enum corners
	{
		A = 0,
		B,
		C,
		D,
	};

	float proj[16];
	float modl[16];
	float clip[16];

	static Frustum GetFrustum(XyEngine* engine)
	{
		static Frustum frustum;
		frustum.CalculateFrustum(engine);
		return frustum;
	}

	

	void NormalizePlane(float frustum[6][4], int side)
	{
		float magnitude = (float)sqrt(frustum[side][0] * frustum[side][0] + frustum[side][1] * frustum[side][1] + frustum[side][2] * frustum[side][2]);

		frustum[side][0] /= magnitude;
		frustum[side][1] /= magnitude;
		frustum[side][2] /= magnitude;
		frustum[side][3] /= magnitude;
	}
	void CalculateFrustum(XyEngine* engine)
	{	
		glGetFloatv(2983, proj);
		glGetFloatv(2982, modl);

		clip[0] = (modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12]);
		clip[1] = (modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13]);
		clip[2] = (modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14]);
		clip[3] = (modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15]);

		clip[4] = (modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12]);
		clip[5] = (modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13]);
		clip[6] = (modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14]);
		clip[7] = (modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15]);

		clip[8] = (modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12]);
		clip[9] = (modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13]);
		clip[10] = (modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14]);
		clip[11] = (modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15]);

		clip[12] = (modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12]);
		clip[13] = (modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13]);
		clip[14] = (modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14]);
		clip[15] = (modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15]);

		m_Frustum[0][0] = (clip[3] - clip[0]);
		m_Frustum[0][1] = (clip[7] - clip[4]);
		m_Frustum[0][2] = (clip[11] - clip[8]);
		m_Frustum[0][3] = (clip[15] - clip[12]);

		NormalizePlane(m_Frustum, 0);

		m_Frustum[1][0] = (clip[3] + clip[0]);
		m_Frustum[1][1] = (clip[7] + clip[4]);
		m_Frustum[1][2] = (clip[11] + clip[8]);
		m_Frustum[1][3] = (clip[15] + clip[12]);

		NormalizePlane(m_Frustum, 1);

		m_Frustum[2][0] = (clip[3] + clip[1]);
		m_Frustum[2][1] = (clip[7] + clip[5]);
		m_Frustum[2][2] = (clip[11] + clip[9]);
		m_Frustum[2][3] = (clip[15] + clip[13]);

		NormalizePlane(m_Frustum, 2);

		m_Frustum[3][0] = (clip[3] - clip[1]);
		m_Frustum[3][1] = (clip[7] - clip[5]);
		m_Frustum[3][2] = (clip[11] - clip[9]);
		m_Frustum[3][3] = (clip[15] - clip[13]);

		NormalizePlane(m_Frustum, 3);

		m_Frustum[4][0] = (clip[3] - clip[2]);
		m_Frustum[4][1] = (clip[7] - clip[6]);
		m_Frustum[4][2] = (clip[11] - clip[10]);
		m_Frustum[4][3] = (clip[15] - clip[14]);

		NormalizePlane(m_Frustum, 4);

		m_Frustum[5][0] = (clip[3] + clip[2]);
		m_Frustum[5][1] = (clip[7] + clip[6]);
		m_Frustum[5][2] = (clip[11] + clip[10]);
		m_Frustum[5][3] = (clip[15] + clip[14]);

		NormalizePlane(m_Frustum, 5);
		
	}

	 bool pointInFrustum(float x, float y, float z) {
		for (int i = 0; i < 6; i++) {
			if (m_Frustum[i][0] * x + m_Frustum[i][1] * y + m_Frustum[i][2] * z + m_Frustum[i][3] <= 0.0F) {
				return false;
			}
		}

		return true;
	}

	bool sphereInFrustum(float x, float y, float z, float radius) {
		for (int i = 0; i < 6; i++) {
			if (m_Frustum[i][0] * x + m_Frustum[i][1] * y + m_Frustum[i][2] * z + m_Frustum[i][3] <= -radius) {
				return false;
			}
		}

		return true;
	}

	bool cubeFullyInFrustum(float x1, float y1, float z1, float x2, float y2, float z2) {
		for (int i = 0; i < 6; i++) {
			if ( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F)
				return false;
		}

		return true;
	}

	bool cubeInFrustum(float x1, float y1, float z1, float x2, float y2, float z2) 
	{
		for (int i = 0; i < 6; i++) 
		{
			if (( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F)) 
			{
				return false;
			}
		}
		return true;
	}
};