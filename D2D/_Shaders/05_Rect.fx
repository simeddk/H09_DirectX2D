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

float4 Color;
float4 PS(VertexOutput input) : SV_Target
{
	return lerp(Color, float4(0, 0, 0, 1), input.Uv.y);
	
	//return Color;
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}