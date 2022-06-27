#include "color_utility.hlsli"

RWTexture2D<float4> gHdrTexture : register(u0);
RWStructuredBuffer<float> AverageLumin : register(u1);

groupshared float4 gHdrColorCache[256];

[numthreads(256, 1, 1)]
void main(int3 _groupId : SV_GroupThreadID, int3 _dispatchId : SV_DispatchThreadID)
{
    gHdrColorCache[_groupId.x] = gHdrTexture[min(_dispatchId.xy, int2(1280, 720) - 1)];

    float4 color = gHdrColorCache[_groupId.x];
    // TEMP EXPOSURE
    // color.rgb *= 0.2f;
    float expo = 0;
    for (int i = 0; i < 3600; ++i)
    {
        expo += AverageLumin[i];
    }
    expo /= 3600.f;
    color.rgb *= expo;
    // color.rgb /= expo;
    // color.rgb *= 0.2f;
    // TEMP EXPOSURE
    color.rgb = LinearToSRGB(ACESTonemapping(color.rgb));

    gHdrTexture[_dispatchId.xy] = color;
}
