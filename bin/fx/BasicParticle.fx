
float4x4 gViewMatrix : View;
float4x4 gViewProjectionMatrix : ViewProjection;
float gViewPortHeight = 720.0f;
float gParticleSize;


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
   VS_OUTPUT Output;
   
   pos = mul(Input.mPosition, gViewMatrix);   
   Output.mPosition = mul(Input.mPosition, gViewProjectionMatrix);   

   Output.mTexCoord = 0.5 * Input.mPosition.xy + 0.5;
   float fLen = length(pos);
   Output.mSize = gViewPortHeight * gParticleSize * sqrt(1.0f / (1.0f +  fLen * fLen));
   //ViewportHeight · size · sqrt( 1 / A + B(D) + C(D)^2 )
   //ViewportHeight : 뷰 포트의 높이
   //A, B, C : DRDRS_POINTSCALE_A, B, C 를 통해 지정한 SCALE 값
   //D : 뷰 스페이스내의 포인트 스프라이트와 카메라 위치와의 거리값
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
   //return gColor;
   float4 color = tex2D(DiffuseSampler, Input.mTexCoord);
   clip(color.a < 0.3f ? -1: color.a);
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

