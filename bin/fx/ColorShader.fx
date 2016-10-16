
float4x4 gWorldMatrix : World;
float4x4 gViewProjectionMatrix : ViewProjection;

struct VS_INPUT
{
   float4 mPosition : POSITION;
   float4 mColor : COLOR0;
};

struct VS_OUTPUT
{
   float4 mPosition : POSITION;
   float4 mColor : COLOR0;
};

VS_OUTPUT ColorMapping_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   Output.mPosition = mul(Input.mPosition, gWorldMatrix);
   Output.mPosition = mul(Output.mPosition, gViewProjectionMatrix);
   Output.mColor = Input.mColor;
   return Output;
}

struct PS_INPUT
{
   float4 mColor : COLOR0;
};

float4 ColorMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
   //return gColor;
   return Input.mColor;
}

technique ColorMapping
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 ColorMapping_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 ColorMapping_Pass_0_Pixel_Shader_ps_main();
   }

}

