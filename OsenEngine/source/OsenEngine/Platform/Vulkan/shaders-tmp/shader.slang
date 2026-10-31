
static float3 vertices[3] = {
    float3(0.0, 0.0, 0.5),
    float3(0.5, 0.0, 0.5),
    float3(0.0, 0.5, 0.5),
};

[shader("vertex")]
float4 vertMain(int vid : SV_VertexID) : SV_Position
{
    return float4(vertices[vid], 1.0);
}

[shader("fragment")]
float4 fragMain() : SV_Target
{
    return float4(1.0, 0.0, 0.0, 1.0);
}
