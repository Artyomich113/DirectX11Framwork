#pragma once

Texture2D txDiffuse : register(t0); // Буфер текстуры
SamplerState samLinear : register(s0); // Буфер образца



cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 CameraPos;
    float4 LightColor;
    float4 LightDir;
    float4 pointLightPos[2];
    float4 pointLightColors[2];
    float pointLightPower[2];
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
    float4 Pos0 : POSITION1;
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
    output.Pos0 = mul(input.Pos, World);
    //output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos0, View);
    output.Pos = mul(output.Pos, Projection);
    output.Tex = input.Tex;
    output.Norm = input.Norm;
    
    return output;
}

float Unity_InverseLerp_float4(float A, float B, float T) // 1,0,1 (1-1)/(0-1)// 
{
    return (T - A) / (B - A);
}

float4 PSTEX(PS_INPUTTEX input) : SV_Target
{
    
    
    float4 finalColor = (0.0f, 0.0f, 0.0f, 1.0f);

    float3 LightDirection = LightDir;
    float3 normal = normalize(input.Norm);
    float3 lightcolor = normalize(LightColor);
    
    
    float3 ambientlight = float3(0.2f, 0.2f, 0.2f);
    //return input.Pos0;
    
    //return pointLightPos[1];
    
    for (int i = 0; i < 2; i++)
    {
        float4 r = (pointLightPos[i] - input.Pos0);
        float p = length(r);
        
        ambientlight += pointLightColors[i] * saturate(Unity_InverseLerp_float4(pointLightPower[i], 0, p));
    }
    
    
    float lightFallOff = saturate(dot(-LightDirection, normal));
    
    //return lightFallOff;
    float3 directDiffuseLight = lightcolor * lightFallOff;
    
    //return txDiffuse.Sample(samLinear, input.Tex);
    //return saturate(pointLightColors[0] + pointLightColors[1]);
    finalColor = txDiffuse.Sample(samLinear, input.Tex) * float4(directDiffuseLight, 1.0f) + float4(ambientlight, 1.0f);
    return finalColor;
}