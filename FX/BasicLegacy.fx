#include "LightHelper.fx"

/// PBR ������ ���Ž� �׷��Ƚ��� �����ֱ� ���� ����
/// 
/// Ambient(�ܻ�), Diffuse(����Ʈ), Specular(��-��)
/// 
/// 2022.07.11 LeHideHome

cbuffer cbPerFrame
{
    DirectionalLight gDirectionLight;
    PointLight gPointLights[100];
	float3 gEyePosW;
    float4 gLightColor;
    float gLightCount;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
}; 

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
//Texture2D gSpecularColorMap;
//Texture2D gSpecularLevelMap;
Texture2D gNormalMap;


SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	//Filter = MIN_MAG_MIP_LINEAR;

	MaxAnisotropy = 4;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
	float3 TangentL : TANGENT;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
	float3 TangentW	: TANGENT;
	float2 Tex     : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW    = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
    vout.TangentW = mul(vin.TangentL, (float3x3) gWorld);
		
	// ������ǥ���� �븻 �׽�Ʈ��
	//vout.NormalW = vin.NormalL;

	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Output vertex attributes for interpolation across triangle.
	vout.Tex = vin.Tex;

	return vout;
}
 
float4 PS(VertexOut pin) : SV_Target
{	
    // Default to multiplicative identity.
    float4 texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    float4 normal = gNormalMap.Sample(samAnisotropic, pin.Tex);

    float3 N = NormalSampleToWorldSpace(normal.xyz, pin.NormalW, pin.TangentW);

    float4 litColor;
	
    float3 toEye = gEyePosW - pin.PosW;
    float distToEye = length(toEye);
    toEye /= distToEye;
		
    float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
    float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
    float4 specular = float4(0.f, 0.f, 0.f, 0.f);
		
    Material mat;
		
    mat.Ambient = float4(1.f, 1.f, 1.f, 1.f);
    mat.Diffuse = float4(1.f, 1.f, 1.f, 1.f);
    mat.Specular = float4(0.f, 0.f, 0.f, 1.f);
    mat.Reflect = float4(0.f, 0.f, 0.f, 0.f);
		
    float4 A, D, S;
    ComputeDirectionalLight(mat, gDirectionLight, N, toEye, A, D, S);
		
    ambient += A;
    diffuse += D;
    specular += S;

	if( gLightCount > 0 )
	{  		
		[unroll]
        for (int i = 0; i < gLightCount; ++i)
        {    
            float4 A, D, S;
        
            ComputePointLight(mat, gPointLights[i],
			pin.PosW, N, toEye, A, D, S);

            ambient += A;
            diffuse += D;
            specular += S;
        }
    }
	
    litColor = texColor * (ambient + diffuse) + specular;

    return litColor;
}


/// ��� �׽�Ʈ
float4 PS_White(VertexOut pin) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

// ����׿� �븻 ���
float4 PS_ShowNormal(VertexOut pin) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

float4 litColor = float4(pin.NormalW.xyz, 1.0f);
	litColor.r = pin.NormalW.x;
	litColor.g = pin.NormalW.y;
	litColor.b = pin.NormalW.z;
	litColor.a = 1.0f;

	return litColor;
}

//
// ��ũ�е�
// �н����� ���̴����� ��� ����ϱ� ���ϰ� �����.
//
technique11 WhiteTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_White() ) );
    }
}

technique11 LightTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

technique11 LightTextureTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

technique11 DebugNormal
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_ShowNormal()));
	}
}