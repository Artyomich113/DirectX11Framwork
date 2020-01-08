#pragma once

Texture2D txDiffuse : register(t0);		// Буфер текстуры
SamplerState samLinear : register(s0);	// Буфер образца

cbuffer ConstantBuffer : register(b0)
{
	matrix World;			// Матрица мира
	matrix View;			// Матрица вида
	matrix Projection;		// Матрица проекции
}


//--------------------------------------------------------------------------------------
struct VS_INPUT					// Входящие данные вершинного шейдера
{
	float4 Pos : POSITION;		// Позиция по X, Y, Z
	float2 Tex : TEXCOORD0;		// Координаты текстуры по tu, tv
};

struct PS_INPUT					// Входящие данные пиксельного шейдера
{
	float4 Pos : SV_POSITION;	// Позиция пикселя в проекции (экранная)
	float2 Tex : TEXCOORD0;		// Координаты текстуры по tu, tv
};


//--------------------------------------------------------------------------------------
// Вершинный шейдер
//--------------------------------------------------------------------------------------
PS_INPUT VSTEX(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;

	return output;
}


//--------------------------------------------------------------------------------------
// Пиксельный шейдер для куба
//--------------------------------------------------------------------------------------
float4 PSTEX(PS_INPUT input) : SV_Target
{
    float4 finalColor = (1, 1, 1, 1);

	finalColor = txDiffuse.Sample(samLinear, input.Tex);

	return finalColor;
}


//--------------------------------------------------------------------------------------
// Пиксельный шейдер для источников света
//--------------------------------------------------------------------------------------
float4 PSSolid(PS_INPUT input) : SV_Target
{
	return (float4)1;
}
