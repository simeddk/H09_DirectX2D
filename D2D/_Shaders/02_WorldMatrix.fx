matrix World;
matrix View;
matrix Projection;

struct VertexInput
{
	float4 Position : Position;
	float4 Color : Color;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float4 Color : Color;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Color = input.Color;

	return output;
}

float4 PS(VertexOutput input) : SV_Target
{
	return float4(0, 1, 1, 1);
}

float4 PS2(VertexOutput input) : SV_Target
{
	return input.Color;
}

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS2()));
	}

	pass P2
	{
		SetRasterizerState(FillMode_WireFrame);

		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS2()));
	}
}