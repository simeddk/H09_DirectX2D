Texture2D DiffuseMap;

matrix World;

cbuffer CB_PerFrame
{
	matrix View;
	matrix Projection;
};

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

float Time;
SamplerState samplerState;
float4 PS(VertexOutput input) : SV_Target
{
	float4 diffuse = DiffuseMap.Sample(samplerState, input.Uv);
	float4 red = float4(1, 0, 0, diffuse.a);

	float alpha = sin(Time * 2.f) * 0.5f + 0.5f; //-1~1 => 0~1
	
	return lerp(diffuse, red, alpha);
}

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

BlendState OpacityMask
{
	AlphaToCoverageEnable = true;
	BlendEnable[0] = true;
	DestBlend[0] = INV_SRC_ALPHA;
	SrcBlend[0] = SRC_ALPHA;
	BlendOp[0] = Add;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = One;
	RenderTargetWriteMask[0] = 0x0F;
};

technique11 T0
{
	pass P0 //Opacity
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1 //Translucent
	{
		SetBlendState(Translucent, float4(0, 0, 0, 0), 0xFF);
		
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P2 //OpacityMask
	{
		SetBlendState(OpacityMask, float4(0, 0, 0, 0), 0xFF);
		
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}