// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "Common/GraphicsPipelineStage.h"
#include "Common/FullscreenPass.h"
#include "Common/ComputeRenderPass.h"

class CVolumetricCloudsStage : public CGraphicsPipelineStage
{
public:
	static bool IsRenderable();
	static Vec4 GetVolumetricCloudShadowParams(const CRenderCamera&, const Vec2& windOffset, const Vec2& vTiling);

public:
	CVolumetricCloudsStage();
	virtual ~CVolumetricCloudsStage();

	void Init() override;
	void Prepare(CRenderView* pRenderView) override;

	void ExecuteShadowGen();

	void Execute();

private:
	void  ExecuteVolumetricCloudShadowGen();
	void  UpdateCloudShadowGenShaderParam(const Vec3& texSize);

	void  ExecuteComputeDensityAndShadow(const struct VCCloudRenderContext& context);
	void  ExecuteRenderClouds(const struct VCCloudRenderContext& context);
	void  UpdateCloudShaderParam(struct VCCloudRenderContext& context);

	int32 GetBufferIndex(const int32 gpuCount, bool bStereoMultiGPURendering) const;
	int32 GetCurrentFrameIndex() const;
	int32 GetPreviousFrameIndex(const int32 gpuCount, bool bStereoMultiGPURendering) const;

	bool  AreTexturesValid() const;

	void  BuildCloudBlockerList();
	void  BuildCloudBlockerSSList();

private:
	static const int32  MaxFrameNum = 4;
	static const uint32 MaxEyeNum = 2;

	CComputeRenderPass  m_passGenerateCloudShadow;
	CComputeRenderPass  m_passComputeDensityAndShadow[MaxEyeNum];
	CComputeRenderPass  m_passRenderClouds[MaxEyeNum];
	CFullscreenPass     m_passTemporalReprojectionDepthMax[MaxEyeNum][2];
	CFullscreenPass     m_passTemporalReprojectionDepthMin[MaxEyeNum][2];
	CFullscreenPass     m_passUpscale[MaxEyeNum][2];

	CTexture*           m_pDownscaledMaxTex[MaxEyeNum][2];
	CTexture*           m_pDownscaledMinTex[MaxEyeNum][2];
	CTexture*           m_pScaledPrevDepthTex[MaxEyeNum];
	CTexture*           m_pCloudDepthTex;
	CTexture*           m_pDownscaledMaxTempTex;
	CTexture*           m_pDownscaledMinTempTex;
	CTexture*           m_pDownscaledLeftEyeTex;
	CTexture*           m_pCloudDensityTex;
	CTexture*           m_pCloudShadowTex;

	CTexture*           m_pCloudMiePhaseFuncTex;
	CTexture*           m_pNoiseTex;

	CConstantBufferPtr  m_pCloudShadowConstantBuffer;
	CConstantBufferPtr  m_pRenderCloudConstantBuffer;
	CConstantBufferPtr  m_pReprojectionConstantBuffer;

	Matrix44            m_viewMatrix[MaxEyeNum][MaxFrameNum];
	Matrix44            m_projMatrix[MaxEyeNum][MaxFrameNum];
	int32               m_nUpdateFrameID[MaxEyeNum];
	int32               m_cleared;
	int32               m_tick;

	int32               m_samplerTrilinearClamp;
	int32               m_samplerTrilinearWrap;

	TArray<Vec4>        m_blockerPosArray;
	TArray<Vec4>        m_blockerParamArray;
	TArray<Vec4>        m_blockerSSPosArray;
	TArray<Vec4>        m_blockerSSParamArray;
};
