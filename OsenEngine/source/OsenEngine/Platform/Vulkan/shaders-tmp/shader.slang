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
float4 fragMain(float2 pixelPos : SV_Position, uniform float time) : SV_Target
{
    //convert to [-1, 1] space, and tile that.
    float2 uv = frac(pixelPos / screenSizeTMP) * 2 - 1;

    float d = sqrt(uv.x * uv.x + uv.y * uv.y);

    float speed = 0.4;
    d = 0.8 * sin(12 * (d-2)+time*speed);

    d = fract(d);

    if (d < 0.15)
        d += 0.4;

    d -= 0.15;

    d = atan2(d, abs(uv.x));

    d = fract(d + time * speed);

    float3 color = float3(d, d * 0.95 + sin(time) * 0.05, d);

    return float4(color,1.0);
}