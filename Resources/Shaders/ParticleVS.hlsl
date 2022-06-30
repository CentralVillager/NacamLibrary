#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float uv : TEXCOORD)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    output.pos = pos;
    output.uv = uv;
    //output.scale = scale;
    return output;
}
