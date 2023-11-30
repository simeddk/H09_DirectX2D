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
	float4 diffuse = float4(1, 1, 1, 1);
	
	if (input.Uv.x < 0.05f)
		diffuse = Color;
	else if (input.Uv.x > 0.95f)
		diffuse = Color;
	else if (input.Uv.y < 0.05f)
		diffuse = Color;
	else if (input.Uv.y > 0.95f)
		diffuse = Color;
		
	return diffuse;
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}