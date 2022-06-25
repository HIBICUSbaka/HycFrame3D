#ifndef KERNEL_SIZE
#define KERNEL_SIZE (5)
#endif

#ifndef KERNEL_HALF
#define KERNEL_HALF (2)
#endif

#ifndef WEIGHT_ARRAY
#define WEIGHT_ARRAY static const float gBlurWeight[] = { 0.0545f, 0.2442f, 0.4026f, 0.2442f, 0.0545f }
#endif

cbuffer BLUR_INFO : register(b0)
{
    uint gTexWidth;
    uint gTexHeight;
    uint gPads[2];
}

RWTexture2D<float4> gMipUav : register(u0);

groupshared float4 gColorCache[256 + 2 * KERNEL_HALF];

WEIGHT_ARRAY;

[numthreads(256, 1, 1)]
void HMain(int3 _groupId : SV_GroupThreadID, int3 _dispatchId : SV_DispatchThreadID)
{
    if (_groupId.x < KERNEL_HALF)
    {
        int x = max(_dispatchId.x - KERNEL_HALF, 0);
        gColorCache[_groupId.x] = gMipUav[int2(x, _dispatchId.y)];
    }
    if (_groupId.x >= 256 - KERNEL_HALF)
    {
        int x = min(_dispatchId.x + KERNEL_HALF, gTexWidth - 1);
        gColorCache[_groupId.x + 2 * KERNEL_HALF] = gMipUav[int2(x, _dispatchId.y)];
    }
    gColorCache[_groupId.x + KERNEL_HALF] = gMipUav[min(_dispatchId.xy, int2(gTexWidth, gTexHeight) - 1)];

    GroupMemoryBarrierWithGroupSync();

    float4 blur = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float totalWeight = 0.0f;

    for (int i = -KERNEL_HALF; i <= KERNEL_HALF; ++i)
    {
        int texIndex = _groupId.x + KERNEL_HALF + i;
        blur += gBlurWeight[i + KERNEL_HALF] * gColorCache[texIndex];
        totalWeight += gBlurWeight[i + KERNEL_HALF];       
    }

    blur /= totalWeight;

    gMipUav[_dispatchId.xy] = blur;
}

[numthreads(1, 256, 1)]
void VMain(int3 _groupId : SV_GroupThreadID, int3 _dispatchId : SV_DispatchThreadID)
{
    if (_groupId.y < KERNEL_HALF)
    {
        int y = max(_dispatchId.y - KERNEL_HALF, 0);
        gColorCache[_groupId.y] = gMipUav[int2(_dispatchId.x, y)];
    }
    if (_groupId.y >= 256 - KERNEL_HALF)
    {
        int y = min(_dispatchId.y + KERNEL_HALF, gTexHeight - 1);
        gColorCache[_groupId.y + 2 * KERNEL_HALF] = gMipUav[int2(_dispatchId.x, y)];
    }
    gColorCache[_groupId.y + KERNEL_HALF] = gMipUav[min(_dispatchId.xy, int2(gTexWidth, gTexHeight) - 1)];

    GroupMemoryBarrierWithGroupSync();

    float4 blur = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float totalWeight = 0.0f;

    for (int i = -KERNEL_HALF; i <= KERNEL_HALF; ++i)
    {
        int texIndex = _groupId.y + KERNEL_HALF + i;
        blur += gBlurWeight[i + KERNEL_HALF] * gColorCache[texIndex];
        totalWeight += gBlurWeight[i + KERNEL_HALF];
    }

    blur /= totalWeight;

    gMipUav[_dispatchId.xy] = blur;
}
