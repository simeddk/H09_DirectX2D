//-------------------------------------------------------------------
//Parameters
//-------------------------------------------------------------------
Texture2D DiffuseMap;

matrix World;

cbuffer CB_PerFrame
{
	matrix View;
	matrix Projection;
};

//-------------------------------------------------------------------
//States
//-------------------------------------------------------------------
SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
};

SamplerState PointSampler
{
	Filter = MIN_MAG_MIP_POINT;
};

BlendState Translucent
{
	AlphaToCoverageEnable = false;
	BlendEnable[0] = true;
	DestBlend[0] = INV_SRC_ALPHA;
	SrcBlend[0] = SRC_ALPHA;
	BlendOp[0] = Add;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = One;
	RenderTargetWriteMask[0] = 0x0F;
};

//-------------------------------------------------------------------
//Rendering
//-------------------------------------------------------------------
struct VertexInput
{
	float4 Position : Position;
	float2 Uv : Uv;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Uv = input.Uv;

	return output;
}

float4 PS(VertexOutput input) : SV_Target
{
	return DiffuseMap.Sample(PointSampler, input.Uv);
}

float4 PS2(VertexOutput input) : SV_Target
{
	return DiffuseMap.Sample(LinearSampler, input.Uv);
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));

		SetBlendState(Translucent, float4(0, 0, 0, 0), 0xFF);
	}

	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS2()));

		SetBlendState(Translucent, float4(0, 0, 0, 0), 0xFF);
	}
}