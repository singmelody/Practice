#include "common.fx"

//Vertex Input
struct VS_INPUT
{ 
     float4 Pos     : POSITION0;
	 float4 Diffuse     : COLOR0;
};

// Vertex Ouput / PS Input
struct VS_OUTPUT
{
    float4 Pos		: POSITION;   // vertex position 
	float4 Diffuse  : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1) 
};

// PS Output
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

VS_OUTPUT RenderSceneVS( VS_INPUT input )
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 worldPos = mul(input.Pos, g_World);
	output.Pos = mul( worldPos, g_VP);
	output.Diffuse = input.Diffuse;

	return output;
}

PS_OUTPUT RenderScenePS( VS_OUTPUT In )
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.RGBColor = In.Diffuse;

	return output;
} 

technique Simple
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSceneVS();
        PixelShader  = compile ps_2_0 RenderScenePS(); // trivial pixel shader (could use FF instead if desired)
	}
}