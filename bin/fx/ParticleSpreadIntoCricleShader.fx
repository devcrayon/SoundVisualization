
float4x4 gViewMatrix : View;
float4x4 gViewProjectionMatrix : ViewProjection;
float4 gCenterPos;
float gViewPortHeight = 720.0f;
float gParticleSize;
float gDistance;
float gAlpha;


struct VS_INPUT
{
   float4 mPosition : POSITION;
};

struct VS_OUTPUT
{
   float4 mPosition : POSITION;
   float2 mTexCoord: TEXCOORD0;
   float mSize : PSIZE0;
};

VS_OUTPUT ColorMapping_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{

   float3 pos;
   float4 dir = normalize(Input.mPosition - gCenterPos);
   //float4 dir = 0.0f;
   dir.w = 0.0f;
   float4 nowPos = Input.mPosition + dir * gDistance;
   VS_OUTPUT Output;
   
   pos = mul(nowPos, gViewMatrix);   
   Output.mPosition = mul(nowPos, gViewProjectionMatrix);   

   Output.mTexCoord = 0.5 * nowPos.xy + 0.5;
   float fLen = length(pos);
   Output.mSize = gViewPortHeight * gParticleSize * sqrt(1.0f / (1.0f +  fLen * fLen));
   //Output.mDistance = length(nowPos - gCenterPos);
   return Output;
}




struct PS_INPUT 
{
   float2 mTexCoord: TEXCOORD0;
};


float4 gColor
<
   string UIName = "gColor";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 1.00, 0.00, 0.00, 1.00 );

texture DiffuseMap_Tex
<
   string ResourceName = "..\\Resource\\00. TestScene\\01. StaticMesh\\JabberwockSkeleton\\JabberwockSkeleton_DM.tga";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
};
float4 ColorMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
   float4 color = tex2D(DiffuseSampler, Input.mTexCoord);
   color.rgb = 1.0f;
   
   float4 tmpColor = 1.0f;
   if(color.a - gAlpha < 0)
   {
	color.rgb = gColor.rgb;
   }
   else if(gAlpha > 0.05f)
   {
        float fRate = gAlpha / color.a;
	color.rgb = tmpColor.rgb * (1.0f - fRate) + fRate * gColor.rgb;
   }
   
   clip(color.a < 0.1f ? -1: color.a);
   
   return color;
}

technique ColorMapping
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 ColorMapping_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 ColorMapping_Pass_0_Pixel_Shader_ps_main();
   }

}

