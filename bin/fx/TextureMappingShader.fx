float4x4 gViewProjectionMatrix : ViewProjection;

struct VS_INPUT
{
   float4 mPosition : POSITION;
   float2 mUV : TEXCOORD0;
};

struct VS_OUTPUT
{
   float4 mPosition : POSITION;
   float2 mUV : TEXCOORD0;
};

VS_OUTPUT TextureMapping_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   Output.mPosition = mul(Input.mPosition, gViewProjectionMatrix);
   
   Output.mUV = Input.mUV;
   
   return Output;
}
texture DiffuseMap_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Earth.jpg";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};

struct PS_INPUT
{
   float2 mUV : TEXCOORD0;
};

float4 TextureMapping_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR
{
   float4 albedo = tex2D(DiffuseSampler, Input.mUV);
   albedo += tex2D(DiffuseSampler, float2(Input.mUV.x + 0.001, Input.mUV.y + 0.001));
   albedo += tex2D(DiffuseSampler, float2(Input.mUV.x, Input.mUV.y + 0.001));
   albedo += tex2D(DiffuseSampler, float2(Input.mUV.x - 0.001, Input.mUV.y + 0.001));
   albedo += tex2D(DiffuseSampler, float2(Input.mUV.x + 0.001, Input.mUV.y));
   albedo += tex2D(DiffuseSampler, float2(Input.mUV.x, Input.mUV.y));
   albedo += tex2D(DiffuseSampler, float2(Input.mUV.x - 0.001, Input.mUV.y));
   albedo += tex2D(DiffuseSampler, float2(Input.mUV.x + 0.001, Input.mUV.y - 0.001));
   albedo += tex2D(DiffuseSampler, float2(Input.mUV.x, Input.mUV.y - 0.001));
   albedo += tex2D(DiffuseSampler, float2(Input.mUV.x - 0.001, Input.mUV.y - 0.001));
   albedo *= 0.1f;
   albedo.a = 0.2f;
   return albedo.rgba;
}

technique TextureMapping
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 TextureMapping_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 TextureMapping_Pass_0_Pixel_Shader_ps_main();
   }

}

