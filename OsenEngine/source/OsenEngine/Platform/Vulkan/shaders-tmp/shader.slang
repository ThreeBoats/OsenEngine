static float2 positions[3] = float2[](
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5)
);

static float screenSizeTMP = 600;

[shader("vertex")]
float4 vertMain(uint vid : SV_VertexID) : SV_Position
{
    return float4(10*positions[vid], 0.0, 1.0);
}

[shader("fragment")]
float4 fragMain(float2 pixelPos : SV_Position) : SV_Target
{
    //convert to [-1, 1] space, and tile that.
    float2 uv = frac(pixelPos / screenSizeTMP) * 2 - 1;

    float d = sqrt(uv.x * uv.x + uv.y * uv.y);

    d = sin(d*12);

    float3 color = float3(d, d, d);

    return float4(color,1.0);
}