float4x4 g_World;
float4x4 g_View;
float4x4 g_Proj;

texture g_Texture;
sampler MeshTextureSampler = sampler_state
{
    Texture = <g_Texture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};