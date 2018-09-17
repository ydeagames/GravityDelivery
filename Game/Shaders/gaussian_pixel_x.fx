// <グローバル変数>

float MAP_WIDTH;
float MAP_HEIGHT;
float weight[8];

// <テクスチャ>
texture SrcMap;
sampler SrcSamp = sampler_state
{
	Texture = <SrcMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};
// <頂点シェーダからピクセルシェーダに渡すデータ>
struct VS_OUTPUT
{
	float4 Pos          : POSITION;
	float2 Tex          : TEXCOORD0;
};

// <Xぼかす>

// <頂点シェーダプログラム>
VS_OUTPUT VS_pass1(
	float4 Pos    : POSITION,          // モデルの頂点
	float4 Tex : TEXCOORD0          // テクスチャ座標
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;        // 出力データ

										 // 位置座標
	Out.Pos = Pos;

	Out.Tex = Tex + float2(0, 0.5f / MAP_HEIGHT);

	return Out;
}

// <ピクセルシェーダプログラム>
float4 PS_pass1(VS_OUTPUT In) : COLOR
{
	float4 Color;

Color = weight[0] * tex2D(SrcSamp, In.Tex);
Color += weight[1]
* (tex2D(SrcSamp, In.Tex + float2(+2.0f / MAP_WIDTH, 0))
	+ tex2D(SrcSamp, In.Tex + float2(-2.0f / MAP_WIDTH, 0)));
Color += weight[2]
* (tex2D(SrcSamp, In.Tex + float2(+4.0f / MAP_WIDTH, 0))
	+ tex2D(SrcSamp, In.Tex + float2(-4.0f / MAP_WIDTH, 0)));
Color += weight[3]
* (tex2D(SrcSamp, In.Tex + float2(+6.0f / MAP_WIDTH, 0))
	+ tex2D(SrcSamp, In.Tex + float2(-6.0f / MAP_WIDTH, 0)));
Color += weight[4]
* (tex2D(SrcSamp, In.Tex + float2(+8.0f / MAP_WIDTH, 0))
	+ tex2D(SrcSamp, In.Tex + float2(-8.0f / MAP_WIDTH, 0)));
Color += weight[5]
* (tex2D(SrcSamp, In.Tex + float2(+10.0f / MAP_WIDTH, 0))
	+ tex2D(SrcSamp, In.Tex + float2(-10.0f / MAP_WIDTH, 0)));
Color += weight[6]
* (tex2D(SrcSamp, In.Tex + float2(+12.0f / MAP_WIDTH, 0))
	+ tex2D(SrcSamp, In.Tex + float2(-12.0f / MAP_WIDTH, 0)));
Color += weight[7]
* (tex2D(SrcSamp, In.Tex + float2(+14.0f / MAP_WIDTH, 0))
	+ tex2D(SrcSamp, In.Tex + float2(-14.0f / MAP_WIDTH, 0)));

return Color;
}

// <Yぼかす>

// <頂点シェーダプログラム>
VS_OUTPUT VS_pass2(
	float4 Pos    : POSITION,          // モデルの頂点
	float4 Tex : TEXCOORD0          // テクスチャ座標
) {
	VS_OUTPUT Out = (VS_OUTPUT)0;        // 出力データ

										 // 位置座標
	Out.Pos = Pos;

	Out.Tex = Tex + float2(0.5f / MAP_WIDTH, 0);

	return Out;
}

// <ピクセルシェーダプログラム>
float4 PS_pass2(VS_OUTPUT In) : COLOR
{
	float4 Color;

Color = weight[0] * tex2D(SrcSamp, In.Tex);
Color += weight[1]
* (tex2D(SrcSamp, In.Tex + float2(0, +2.0f / MAP_HEIGHT))
	+ tex2D(SrcSamp, In.Tex + float2(0, -2.0f / MAP_HEIGHT)));
Color += weight[2]
* (tex2D(SrcSamp, In.Tex + float2(0, +4.0f / MAP_HEIGHT))
	+ tex2D(SrcSamp, In.Tex + float2(0, -4.0f / MAP_HEIGHT)));
Color += weight[3]
* (tex2D(SrcSamp, In.Tex + float2(0, +6.0f / MAP_HEIGHT))
	+ tex2D(SrcSamp, In.Tex + float2(0, -6.0f / MAP_HEIGHT)));
Color += weight[4]
* (tex2D(SrcSamp, In.Tex + float2(0, +8.0f / MAP_HEIGHT))
	+ tex2D(SrcSamp, In.Tex + float2(0, -8.0f / MAP_HEIGHT)));
Color += weight[5]
* (tex2D(SrcSamp, In.Tex + float2(0, +10.0f / MAP_HEIGHT))
	+ tex2D(SrcSamp, In.Tex + float2(0, -10.0f / MAP_HEIGHT)));
Color += weight[6]
* (tex2D(SrcSamp, In.Tex + float2(0, +12.0f / MAP_HEIGHT))
	+ tex2D(SrcSamp, In.Tex + float2(0, -12.0f / MAP_HEIGHT)));
Color += weight[7]
* (tex2D(SrcSamp, In.Tex + float2(0, +14.0f / MAP_HEIGHT))
	+ tex2D(SrcSamp, In.Tex + float2(0, -14.0f / MAP_HEIGHT)));

return Color;
}
