
float2 pos[3] =
{
    float2(0.0f, -0.5),
	float2(0.5f, 0.0),
	float2(-0.5, 0.0)
};


struct VSout
{
    float4 pos : SV_Position;
};

VSout main(uint vertexId : SV_VertexID )
{
    VSout o;
    o.pos = float4(pos[vertexId], 0.0f, 1.0f);
    return o
}