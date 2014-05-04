//--------------------------------------------------------------------------------------
// File: Sample.fx
//
// The effect file for the Sample sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4 g_MaterialAmbientColor;      // Material's ambient color
float4 g_MaterialDiffuseColor;      // Material's diffuse color
int g_nNumLights;

float3 g_LightDir[3];               // Light's direction in world space
float4 g_LightDiffuse[3];           // Light's diffuse color
float4 g_LightAmbient;              // Light's ambient color

texture g_MeshTexture;              // Color texture for mesh

float    g_fTime;                   // App's time in seconds
float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mVP;    // World * View * Projection matrix

float4x4 MatrixPalette[35]; 
int numBoneInfluences = 2;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords
	float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1) 
};

//--------------------------------------------------------------------------------------
// This section computes standard transform and lighting
//--------------------------------------------------------------------------------------
//Morph Weights
float4 weights;

//Vertex Input
struct VS_INPUT
{ 
     float4 basePos     : POSITION0;
     float3 baseNorm    : NORMAL0;
     float2 baseUV      : TEXCOORD0;

     float4 targetPos1  : POSITION1;
     float3 targetNorm1 : NORMAL1;

     float4 targetPos2  : POSITION2;
     float3 targetNorm2 : NORMAL2;

     float4 targetPos3  : POSITION3;
     float3 targetNorm3 : NORMAL3;

     float4 targetPos4  : POSITION4;
     float3 targetNorm4 : NORMAL4;
};

VS_OUTPUT RenderMultiMorphVS(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	float4 pos = IN.basePos;
	float3 norm = IN.baseNorm;

	// Blend Position
	pos += (IN.targetPos1 - IN.basePos) * weights.r;
	pos += (IN.targetPos2 - IN.basePos) * weights.g;
	pos += (IN.targetPos3 - IN.basePos) * weights.b;
	pos += (IN.targetPos4 - IN.basePos) * weights.a;

	//Blend Normal	
    norm += (IN.targetNorm1 - IN.baseNorm) * weights.r;
    norm += (IN.targetNorm2 - IN.baseNorm) * weights.g;
    norm += (IN.targetNorm3 - IN.baseNorm) * weights.b;
    norm += (IN.targetNorm4 - IN.baseNorm) * weights.a;

	//getting the position of the vertex in the world
    float4 posWorld = mul(pos, g_mWorld);
    float4 normal = normalize(mul(norm, g_mWorld)); 
	 
    //getting to position to object space
    OUT.Position = mul(posWorld, g_mVP);
	 
    OUT.Diffuse = max(dot(normal, normalize(g_LightDir[0] - posWorld)), 0.2f);
    
    OUT.TextureUV = IN.baseUV;
    
    return OUT;
}

//--------------------------------------------------------------------------------------
// This section computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0,
                         uniform int nNumLights,
                         uniform bool bTexture,
                         uniform bool bAnimate )
{
    VS_OUTPUT Output = (VS_OUTPUT)0;

    float3 vNormalWorldSpace;
    float4 vAnimatedPos = vPos;
    
    // Animation the vertex based on time and the vertex's object space position
    if( bAnimate )
		vAnimatedPos += float4(vNormal, 0) * (sin(g_fTime+5.5)+0.5)*5;
    
    // Transform the position from object space to homogeneous projection space
    float4 worldPos = mul(vAnimatedPos, g_mWorld);
	Output.Position = mul( worldPos, g_mVP);
    
    // Transform the normal from object space to world space    
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); // normal (world space)
    
    // Compute simple directional lighting equation
    float3 vTotalLightDiffuse = float3(0,0,0);
    for(int i=0; i<nNumLights; i++ )
	{
		vTotalLightDiffuse += g_LightDiffuse[i] * max(0.1f,dot(vNormalWorldSpace, (g_LightDir[i] - worldPos)));
	}
        
    Output.Diffuse.rgb = g_MaterialDiffuseColor * vTotalLightDiffuse + g_MaterialAmbientColor * g_LightAmbient;   
    Output.Diffuse.a = 1.0f; 
    
    // Just copy the texture coordinate through
    if( bTexture ) 
        Output.TextureUV = vTexCoord0; 
    else
        Output.TextureUV = 0; 
    
    return Output;    
}

VS_OUTPUT RenderSkinHALVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0,
						 float4 weights : BLENDWEIGHT0,
						 int4	boneIndices : BLENDINDICES0,
                         uniform int nNumLights,
                         uniform bool bTexture,
                         uniform bool bAnimate )
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
    
    // Transform the position from object space to homogeneous projection space
    float4 localPos = float4( 0.0f, 0.0f, 0.0f, 0.0f);
	float3 normal = float3( 0.0f, 0.0f, 0.0f);
	float lastWeight = 0.0f;

	vNormal  = normalize(vNormal);
	int n = numBoneInfluences - 1;
	//Blend vertex position & normal
	for(int i = 0; i < n; ++i)
	{
		lastWeight += weights[i];
		localPos += weights[i] * mul( vPos, MatrixPalette[boneIndices[i]]);
		normal += weights[i] * mul( vNormal, MatrixPalette[boneIndices[i]]);
	}
	lastWeight = 1.0f - lastWeight;

	localPos += lastWeight * mul( vPos, MatrixPalette[boneIndices[n]]);
	normal += lastWeight * mul( vNormal, MatrixPalette[boneIndices[n]]);
	localPos.w = 1.0f;
	
	float4 worldPos = mul( localPos, g_mWorld);
	Output.Position = mul( worldPos, g_mVP);
    
    // Transform the normal from object space to world space    
	normal = normalize(normal);
    float3 worldNormal = normalize(mul(normal, (float3x3)g_mWorld)); // normal (world space)

	// Compute simple directional lighting equation
    float3 vTotalLightDiffuse = float3(0,0,0);
    for(int i=0; i<nNumLights; i++ )
		vTotalLightDiffuse += g_LightDiffuse[i] * max(0.1f,dot(worldNormal, (g_LightDir[i] - worldPos)));
        
    Output.Diffuse.rgb = g_MaterialDiffuseColor * vTotalLightDiffuse + g_MaterialAmbientColor * g_LightAmbient;   
    Output.Diffuse.a = 1.0f; 

	// Just copy the texture coordinate through
    if( bTexture ) 
        Output.TextureUV = vTexCoord0; 
    else
        Output.TextureUV = 0; 
    
    return Output;   		
}

//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In,
                         uniform bool bTexture ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    if( bTexture )
        Output.RGBColor = tex2D(MeshTextureSampler, In.TextureUV) + In.Diffuse;
    else
        Output.RGBColor = float4(0.0f,0.0f,0.0f,0.0f);

    return Output;
}

//Pixel Shader
float4 RenderShadowPS(VS_OUTPUT IN,
					  uniform bool bTexture ) : COLOR0
{
	return float4(0.1f, 0.1f, 0.1f, 1.0f);
}

//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTexture1Light
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 1, true, false );
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture2Light
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 2, true, false );
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture3Light
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 3, true, false );
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneNoTexture
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( 1, false, false );
        PixelShader  = compile ps_2_0 RenderScenePS( false ); // trivial pixel shader (could use FF instead if desired)
    }
}

//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTexture1LightHAL
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSkinHALVS( 1, true, false );
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture2LightHAL
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSkinHALVS( 2, true, false );
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture3LightHAL
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSkinHALVS( 3, true, false );
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneNoTextureHAL
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSkinHALVS( 1, false, false );
        PixelShader  = compile ps_2_0 RenderScenePS( false ); // trivial pixel shader (could use FF instead if desired)
    }
}

//Shadow Technique
technique ShadowSoft
{
	pass P0
	{        
		Lighting = false;

        VertexShader = compile vs_2_0 RenderSceneVS( 1, false, false );
		PixelShader  = compile ps_2_0 RenderShadowPS(false);
	}
}

// Soft Skinning
technique SkinSoft
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSceneVS( 1, true, false );
		PixelShader  = compile ps_2_0 RenderScenePS(true);
	}
}


//Shadow Technique
technique ShadowHAL
{
	pass P0
	{        
		Lighting = false;

        VertexShader = compile vs_2_0 RenderSkinHALVS( 1, false, false );
		PixelShader  = compile ps_2_0 RenderShadowPS(false);
	}
}

// HAL Skinning
technique SkinHAL
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSkinHALVS( 1, true, false );
		PixelShader  = compile ps_2_0 RenderScenePS(true);
	}
}

// MultiMorph
technique MultiMorph
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderMultiMorphVS();
		PixelShader  = compile ps_2_0 RenderScenePS(true);
	}
}