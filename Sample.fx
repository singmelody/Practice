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

texture g_DecalTexture;              // Color texture for mesh

float    g_fTime;                   // App's time in seconds
float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mWorldInverse;           // World Inverse matrix for object
float4x4 g_mVP;    // World * View * Projection matrix
float4	 g_mCameraPos;

float4	 g_wrinkleWeight;

float4x4 MatrixPalette[35]; 
int numBoneInfluences = 2;

//Control hair table
extern float3 ControlHairTable[20];
int numPointsPerHair = 4;

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

sampler DecalTextureSampler = sampler_state
{
    Texture = <g_DecalTexture>;
   MinFilter = Linear;   MagFilter = Linear;   MipFilter = Linear;
   AddressU  = Clamp;    AddressV  = Clamp;    AddressW  = Clamp;
   MaxAnisotropy = 16;
};

//Morph Weight
float shapeShift;

//Texture
texture texHuman;
texture texWolf;

//Samplers
sampler HumanSampler = sampler_state
{
   Texture = (texHuman);
   MinFilter = Linear;   MagFilter = Linear;   MipFilter = Linear;
   AddressU  = Wrap;     AddressV  = Wrap;     AddressW  = Wrap;
   MaxAnisotropy = 16;
};

sampler WolfSampler = sampler_state
{
   Texture = (texWolf);
   MinFilter = Linear;   MagFilter = Linear;   MipFilter = Linear;
   AddressU  = Wrap;     AddressV  = Wrap;     AddressW  = Wrap;
   MaxAnisotropy = 16;
};

//Textures
texture texDiffuse;
texture texNormalMap;
texture texSpecular;
texture texBlend;

//Samplers
sampler DiffuseSampler = sampler_state
{
   Texture = (texDiffuse);
   MinFilter = Linear;   MagFilter = Linear;   MipFilter = Linear;
   AddressU  = Wrap;     AddressV  = Wrap;     AddressW  = Wrap;
   MaxAnisotropy = 16;
};

sampler NormalSampler = sampler_state
{
   Texture = (texNormalMap);
   MinFilter = Linear;   MagFilter = Linear;   MipFilter = Linear;
   AddressU  = Wrap;     AddressV  = Wrap;     AddressW  = Wrap;
   MaxAnisotropy = 16;
};

sampler SpecularSampler = sampler_state
{
   Texture = (texSpecular);
   MinFilter = Linear;   MagFilter = Linear;   MipFilter = Linear;
   AddressU  = Wrap;     AddressV  = Wrap;     AddressW  = Wrap;
   MaxAnisotropy = 16;
};

sampler BlendSampler = sampler_state
{
   Texture = (texBlend);
   MinFilter = Linear;   MagFilter = Linear;   MipFilter = Linear;
   AddressU  = Wrap;     AddressV  = Wrap;     AddressW  = Wrap;
   MaxAnisotropy = 16;
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
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

//--------------------------------------------------------------------------------------
// This section computes standard transform and lighting and morph skeleton
//--------------------------------------------------------------------------------------
//Vertex Input
struct VS_INPUT_MORPH
{
     float4 position    : POSITION0;
     float3 normal      : NORMAL0;
     float2 tex0        : TEXCOORD0;
	 float4 weights     : BLENDWEIGHT0;
     int4   boneIndices : BLENDINDICES0;
     
     float4 position2 : POSITION1;
     float3 normal2   : NORMAL1;
};

VS_OUTPUT RenderMorphSkeletonVS(VS_INPUT_MORPH IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	float4 position = IN.position + ( IN.position2 - IN.position ) * shapeShift;	// shapeShift-->[0,1]
	float4 p = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 norm = float3(0.0f, 0.0f, 0.0f);
    float lastWeight = 0.0f;
    int n = numBoneInfluences-1;
    
    IN.normal = normalize(IN.normal);
    for(int i = 0; i < n; ++i)
    {
        lastWeight += IN.weights[i];
	    p += IN.weights[i] * mul(position, MatrixPalette[IN.boneIndices[i]]);
	    norm += IN.weights[i] * mul(IN.normal, MatrixPalette[IN.boneIndices[i]]);
    }
    lastWeight = 1.0f - lastWeight;
    
    p += lastWeight * mul(position, MatrixPalette[IN.boneIndices[n]]);
    norm += lastWeight * mul(IN.normal, MatrixPalette[IN.boneIndices[n]]);

	p.w = 1.0f;    	
	float4 posWorld = mul(p, g_mWorld);
    OUT.Position = mul(posWorld, g_mVP);
    OUT.TextureUV = IN.tex0;
    
	//Calculate Lighting
    norm = normalize(norm);
    norm = mul(norm, g_mWorld);

	// Compute simple directional lighting equation
    float3 vTotalLightDiffuse = float3(0,0,0);
    for(int i=0; i<1; i++ )
		vTotalLightDiffuse += g_LightDiffuse[i] * max(0.1f,dot(norm, (g_LightDir[i] - posWorld)));
        
    OUT.Diffuse.rgb = g_MaterialDiffuseColor * vTotalLightDiffuse + g_MaterialAmbientColor * g_LightAmbient;   
    OUT.Diffuse.a = 1.0f; 

	return OUT;
}

PS_OUTPUT RenderSceneMorphPS( VS_OUTPUT IN,
                         uniform bool bTexture ) 
{ 
    PS_OUTPUT Output;

    //Sample human texture
	float4 colorHuman = tex2D(HumanSampler, IN.TextureUV);
	
	//sample wolf texture
	float4 colorWolf = tex2D(WolfSampler, IN.TextureUV);
	
	//Blend the result based on the shapeShift variable
	Output.RGBColor = (colorHuman * (1.0f - shapeShift) + colorWolf * shapeShift) * IN.Diffuse;

	return Output;
}


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

struct VS_BONE_MORPH_INPUT
{ 
     float4 pos0  : POSITION0;
     float3 norm0 : NORMAL0;
     float2 tex0  : TEXCOORD0;

     float4 pos1  : POSITION01;
     float3 norm1 : NORMAL1;

     float4 pos2  : POSITION2;
     float3 norm2 : NORMAL2;

     float4 pos3  : POSITION3;
     float3 norm3 : NORMAL3;

     float4 pos4  : POSITION4;
     float3 norm4 : NORMAL4;
     
	 float4 weights  : BLENDWEIGHT5;
     int4   boneIndices : BLENDINDICES5;     
};

//--------------------------------------------------------------------------------------
// This section computes standard transform and lighting and simple multi morph
//--------------------------------------------------------------------------------------
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
	 
    OUT.Diffuse = max(dot(normal, normalize(g_LightDir[0] - posWorld)), 1.0f);
    
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
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In,
                         uniform bool bTexture ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    if( bTexture )
        Output.RGBColor = tex2D(MeshTextureSampler, In.TextureUV) * In.Diffuse;
    else
        Output.RGBColor = float4(0.0f, 1.0f, 0.0f, 1.0f) * In.Diffuse ;

    return Output;
}

PS_OUTPUT RenderDecalPS( VS_OUTPUT In,
                         uniform bool bTexture ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    if( bTexture )
        Output.RGBColor = tex2D(DecalTextureSampler, In.TextureUV) * In.Diffuse;
    else
        Output.RGBColor = float4(0.0f, 1.0f, 0.0f, 1.0f) * In.Diffuse ;

    return Output;
}

//Pixel Shader
float4 RenderShadowPS(VS_OUTPUT IN,
					  uniform bool bTexture ) : COLOR0
{
	return float4(0.1f, 0.1f, 0.1f, 1.0f);
}

//--------------------------------------------------------------------------------------
// Morph face & normal technique
//--------------------------------------------------------------------------------------
//Vertex Input
struct VS_INPUT_NORMAL
{ 
     float4 basePos     : POSITION0;
     float3 baseNorm    : NORMAL0;
     float2 baseUV      : TEXCOORD0;
	 float3 tangent		: TANGENT0;

     float4 targetPos1  : POSITION1;
     float3 targetNorm1 : NORMAL1;

     float4 targetPos2  : POSITION2;
     float3 targetNorm2 : NORMAL2;

     float4 targetPos3  : POSITION3;
     float3 targetNorm3 : NORMAL3;

     float4 targetPos4  : POSITION4;
     float3 targetNorm4 : NORMAL4;
};

struct VS_OUTPUT_NORMAL
{
	 float4 position	 : POSITION0;
     float2 tex0		 : TEXCOORD0;
     float3 lightVec	 : TEXCOORD1;
	 float3 specularVec  : TEXCOORD2;
};

VS_OUTPUT_NORMAL RenderMultiMorphNormalVS(VS_INPUT_NORMAL IN)
{
	VS_OUTPUT_NORMAL OUT = (VS_OUTPUT_NORMAL)0;

	float4 position  = IN.basePos;
	float3 normal = IN.baseNorm;

	//Blend Position	
	position += (IN.targetPos1 - IN.basePos) * weights.r;
	position += (IN.targetPos2 - IN.basePos) * weights.g;
	position += (IN.targetPos3 - IN.basePos) * weights.b;
	position += (IN.targetPos4 - IN.basePos) * weights.a;

	//Blend Normal	
	normal += (IN.targetNorm1 - IN.baseNorm) * weights.r;
	normal += (IN.targetNorm2 - IN.baseNorm) * weights.g;
	normal += (IN.targetNorm3 - IN.baseNorm) * weights.b;
	normal += (IN.targetNorm4 - IN.baseNorm) * weights.a;

    //getting the position of the vertex in the world
    float4 posWorld = mul(position, g_mWorld);
    OUT.position = mul(posWorld, g_mVP);
    normal = normalize(mul(normal, g_mWorld));
    
    //getting vertex -> light vector
    float3 light = normalize(g_LightDir[0] - posWorld);

    //calculating the binormal and setting the Tangent Binormal and Normal matrix
	float3 binormal = normalize(cross( normal, IN.tangent));
    float3x3 TBNMatrix = float3x3( IN.tangent, binormal, normal);     
    
	// matrix transform from object space to tagent space
	float3x3 toTangentSpace = transpose(TBNMatrix);

	// a world inv need here
	float3 localLight = (float3)mul( float4(light, 1.0f), g_mWorldInverse);
	
    //setting the lightVector
    OUT.lightVec = mul( localLight, toTangentSpace);

	// Calcuate half vector
	float3 viewDir = g_mCameraPos - posWorld;
	float3 halfVec = normalize( g_LightDir[0] + viewDir );
	OUT.specularVec = mul( halfVec, toTangentSpace);
	
    OUT.tex0 = IN.baseUV;
    
    return OUT;
}

//Hair Vertex
struct VS_INPUT_HAIR
{
     float4 position    : POSITION0;
     float3 normal      : NORMAL;
     float2 tex0        : TEXCOORD0;
     int4   hairIndices : BLENDINDICES0;
};

float3 GetHairPos(int hair, int index1, int index2, float prc)
{
	//Calculate index 0 & 3
	int index0 = max(index1 - 1, 0);
	int index3 = min(index2 + 1, numPointsPerHair - 1);
	
	//Offset index to correct hair in ControlHairTable
	index0 += hair * numPointsPerHair;
	index1 += hair * numPointsPerHair;
	index2 += hair * numPointsPerHair;
	index3 += hair * numPointsPerHair;
	
	//Perform cubic interpolation
	float3 P = (ControlHairTable[index3] - ControlHairTable[index2]) - (ControlHairTable[index0] - ControlHairTable[index1]);
	float3 Q = (ControlHairTable[index0] - ControlHairTable[index1]) - P;
	float3 R =  ControlHairTable[index2] - ControlHairTable[index0];
	float3 S =  ControlHairTable[index1];

	return (P * prc * prc * prc) + 
		   (Q * prc * prc) + 
		   (R * prc) + 
		    S;	
}

VS_OUTPUT RenderFaceBoneMorph(VS_BONE_MORPH_INPUT IN,  uniform int nNumLights)
{
	//First Morph the mesh, then apply skinning!

    VS_OUTPUT OUT = (VS_OUTPUT)0;

	float4 position  = IN.pos0;
	float3 normal = IN.norm0;

	//Blend Position	
	position += (IN.pos1 - IN.pos0) * weights.r;
	position += (IN.pos2 - IN.pos0) * weights.g;
	position += (IN.pos3 - IN.pos0) * weights.b;
	position += (IN.pos4 - IN.pos0) * weights.a;

	//Blend Normal	
	normal += (IN.norm1 - IN.norm0) * weights.r;
	normal += (IN.norm2 - IN.norm0) * weights.g;
	normal += (IN.norm3 - IN.norm0) * weights.b;
	normal += (IN.norm4 - IN.norm0) * weights.a;
	
    //getting the position of the vertex in the world
    float4 posWorld = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 normWorld = float3(0.0f, 0.0f, 0.0f);
    float lastWeight = 0.0f;
    int n = numBoneInfluences-1;   
    normal = normalize(normal);
    
    for(int i = 0; i < n; ++i)
    {
        lastWeight += IN.weights[i];
	    posWorld += IN.weights[i] * mul(position, MatrixPalette[IN.boneIndices[i]]);
	    normWorld += IN.weights[i] * mul(normal, MatrixPalette[IN.boneIndices[i]]);
    }
    lastWeight = 1.0f - lastWeight;
    
    posWorld += lastWeight * mul(position, MatrixPalette[IN.boneIndices[n]]);
    normWorld += lastWeight * mul(normal, MatrixPalette[IN.boneIndices[n]]);   
    posWorld.w = 1.0f;    	
    
    OUT.Position = mul(posWorld, g_mWorld);
	OUT.Position = mul(OUT.Position, g_mVP);
	
	// Compute simple directional lighting equation
    float3 vTotalLightDiffuse = float3(0,0,0);
    for(int i=0; i< nNumLights; i++ )
		vTotalLightDiffuse += g_LightDiffuse[i] * max(0.1f,dot(normWorld, (g_LightDir[i] - posWorld)));
        
    OUT.Diffuse.rgb = g_MaterialDiffuseColor * vTotalLightDiffuse + g_MaterialAmbientColor * g_LightAmbient;   
    OUT.Diffuse.a = 1.0f; 
    
    OUT.TextureUV = IN.tex0;

    return OUT;
}


VS_OUTPUT RenderHairVS(VS_INPUT_HAIR IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	//Get position from the four control hairs
	float3 ch1 = GetHairPos(0, IN.hairIndices[0], IN.hairIndices[1], IN.position.z);
	float3 ch2 = GetHairPos(1, IN.hairIndices[0], IN.hairIndices[1], IN.position.z);
	float3 ch3 = GetHairPos(2, IN.hairIndices[0], IN.hairIndices[1], IN.position.z);
	float3 ch4 = GetHairPos(3, IN.hairIndices[0], IN.hairIndices[1], IN.position.z);
	
	//Blend linearly in 2D
	float3 px1 = ch2 * IN.position.x + ch1 * (1.0f - IN.position.x);
	float3 px2 = ch3 * IN.position.x + ch4 * (1.0f - IN.position.x);
	
	float3 pos = px2 * IN.position.y + px1 * (1.0f - IN.position.y);

	//Transform to world coordinates
    float4 posWorld = mul(float4(pos.xyz, 1), g_mWorld);
	OUT.Position = mul(posWorld, g_mVP);
		
	//Copy texture coordinates
    OUT.TextureUV = IN.tex0;  

	OUT.Diffuse = float4(1.0f,1.0f,1.0f,1.0f);
        
    return OUT;
}



//Pixel Shader
float4 RenderSceneNormalPS(VS_OUTPUT_NORMAL IN) : COLOR0
{
    //calculate the color and the normal
    float4 diffuseColor = tex2D(DiffuseSampler, IN.tex0);

    //this is how you uncompress a normal map
    float3 normal = 2.0f * tex2D(NormalSampler, IN.tex0).rgb - 1.0f;

	// sample wrinkle mask texture
	float4 blend = tex2D( BlendSampler, IN.tex0);

	// calculate final normal weight
	float w = blend.r + g_wrinkleWeight.x * blend.g + g_wrinkleWeight.y * blend.b;

	normal.x *= w;
	normal.y *= w;

	normal = normalize(normal);

    //normalize the light
    float3 light = normalize(IN.lightVec);

	//set the output color
    float diffuse = max(saturate(dot(normal, light)), 1.0f);

	// get specular color
	float4 specularColor = tex2D(SpecularSampler, IN.tex0);

	// normalize half 
	float3 halfVec = normalize(IN.specularVec);
    //set the output color
    float specular = max(saturate(dot(normal, halfVec)), 0.0f);
	specular = pow( specular, 85.0f) * 0.4f;




    
	return diffuseColor * diffuse + specularColor * specular;
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

// Face Morph Normal Technique
technique FaceMorphNormal
{
	pass P0
	{        
		Lighting = false;
		
		VertexShader = compile vs_2_0 RenderMultiMorphNormalVS();
		PixelShader  = compile ps_2_0 RenderSceneNormalPS();
	}
}

// Morph Anim & skeleton
technique MorphSkeleton
{
    pass P0
    {
		Lighting = false;
		
        VertexShader = compile vs_2_0 RenderMorphSkeletonVS();
        PixelShader  = compile ps_2_0 RenderSceneMorphPS(true);        
    }
}

technique SimpleDecal
{
    pass P0
    {      
		Lighting = true;
		CullMode = None;
		    
        VertexShader = compile vs_2_0 RenderSceneVS( 1, true, false );
        PixelShader  = compile ps_2_0 RenderScenePS( true );
    }
}

technique Decal
{
    pass P0
    {
		Lighting = false;
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
        VertexShader = compile vs_2_0 RenderSkinHALVS( 1, true, false );
        PixelShader  = compile ps_2_0 RenderDecalPS( true ); 
    }
}

technique Hair
{
	pass P0
	{        
		//FillMode = Wireframe;
		AlphaBlendEnable = true;	
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;	
		CullMode = None;	
		AlphaOp[0] = ADD;
		ColorOp[0] = ADD;
		ZWriteEnable = false;	
		VertexShader = compile vs_2_0 RenderHairVS();
		PixelShader  = compile ps_2_0 RenderScenePS(true);
	}
}

technique FaceBoneMorph
{
    pass P0
    {
		VertexShader = compile vs_2_0 RenderFaceBoneMorph(1);
		PixelShader  = compile ps_2_0 RenderScenePS(true);
        Lighting = false;
    }
}