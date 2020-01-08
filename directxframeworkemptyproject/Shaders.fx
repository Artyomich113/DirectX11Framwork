#pragma once

Texture2D txDiffuse : register(t0); // Буфер текстуры
SamplerState samLinear : register(s0); // Буфер образца

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 CameraPos;
    float Costime;
    float CurTime;
}

struct VS_INPUTTEX					// Входящие данные вершинного шейдера
{
    float4 Pos : POSITION; // Позиция по X, Y, Z
    float2 Tex : TEXCOORD0; // Координаты текстуры по tu, tv
    float3 Norm : NORMAL;
};

struct PS_INPUTTEX				// Входящие данные пиксельного шейдера
{
    float4 Pos : SV_POSITION; // Позиция пикселя в проекции (экранная)
    float2 Tex : TEXCOORD0; // Координаты текстуры по tu, tv
    float3 Norm : NORMAL;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

struct VS_INPUTUI
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
};

struct PS_INPUTUI
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
};

PS_INPUT VSUI(VS_INPUTUI input)
{
    PS_INPUTUI output = (PS_INPUTUI) 0;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, Projection);
    output.Color = input.Color;
    
    return output;
}

float4 PSUI(PS_INPUTUI input) : SV_Target
{
    return input.Color;
}

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Color = input.Color;
    
    return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
    return input.Color;

}

PS_INPUTTEX VSTEX(VS_INPUTTEX input)
{
    PS_INPUTTEX output = (PS_INPUTTEX) 0;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Tex = input.Tex;
    output.Norm = input.Norm;
    
    return output;
}

float4 PSTEX(PS_INPUTTEX input) : SV_Target
{
    float4 finalColor = (0.0f, 0.0f, 0.0f, 1.0f);

    finalColor = txDiffuse.Sample(samLinear, input.Tex) * (cos(CurTime) / 2 + 0.5);
    

    return finalColor;
}